#include "glwfbo.h"
#include "glwvbofactory.h"

#include "logger.h"
#include "stringformat.h"

#include "glwreturncode.h"

LDEFINECLASS("GLW", "GLWFBO")


using glw::engine::buffers::GLWFBO;
using glw::engine::buffers::GLWFBOId;
using glw::util::StringFormat;
using glw::engine::buffers::GLWVBOFactory;
using glw::engine::buffers::GLWVertexArray;
using glw::GLWReturnCode;
using glw::GLW_SUCCESS;
using glw::GLW_FAIL;


GLWFBO::GLWFBO()
{
}

// Frame size
GLWFBO::GLWFBO(int w, int h, bool shadowmap)
{
  this->m_width = w; this->m_height = h;
  if (shadowmap)
    createSBO(w, h);
  else
    createFBO();
  createVBO();
}
// Frame size
GLWFBO::GLWFBO(glm::vec2 windowSize, bool shadowmap)
{
  this->m_width = windowSize.x; this->m_height = windowSize.y;
  if (shadowmap)
    createSBO(this->m_width, this->m_height);
  else
    createFBO();
  createVBO();
}

GLWFBO::~GLWFBO()
{
}

void GLWFBO::draw(GLWShaderHandle handles)
{
  m_vbo.m_tex = m_tex;
  m_vbo.draw(handles);
}

// Binds FBO for render
void GLWFBO::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  //glClearDepth(1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glViewport(0, 0, m_width, m_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, m_width, 0.0, m_height, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Unbinds all FBOs
void GLWFBO::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLWFBO::setTopLeft(glm::vec2 topLeft)
{
  m_vbo.m_pos.x = topLeft.x;
  m_vbo.m_pos.y = topLeft.y;
}

void GLWFBO::setSize(glm::vec2 size)
{
  m_vbo.m_scale.x = size.x;
  m_vbo.m_scale.y = size.y;
}

void GLWFBO::setZ(float z)
{
  m_vbo.m_pos.z = z;
}

void GLWFBO::rotation(float rotation)
{
  m_vbo.m_theta = rotation;
}

void GLWFBO::setTex(GLuint tex)
{
  m_tex = tex;
}

GLuint GLWFBO::getTexId()
{
  return m_tex;
}

// Gets FBO ID
GLWFBOId GLWFBO::getId()
{
  return m_id;
}

GLWReturnCode GLWFBO::createFBO()
{
  GLWReturnCode retCode = GLW_SUCCESS;

  glGenFramebuffers(1, &m_id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);

  //RGBA8 2D texture, 24 bit depth texture
  glGenTextures(1, &m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //NULL means reserve texture memory, but texels are undefined
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  //Attach 2D texture to this FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

  //RGBA8 2D texture, 24 bit depth texture
  glGenTextures(1, &m_depth);
  glBindTexture(GL_TEXTURE_2D, m_depth);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //NULL means reserve texture memory, but texels are undefined
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, m_depth, 0);

  GLuint depthBuffer;
  // The depth buffer
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
  //Attach depth buffer to FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  //Does the GPU support current FBO configuration?

  GLenum status;
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (status)
  {
    case GL_FRAMEBUFFER_COMPLETE:
      LINFO(StringFormat("Successfully created FBO = %0").arg(m_id).str());
      break;
    default:
      retCode = GLW_FAIL;
      LERROR(StringFormat("Error creating FBO").str());
      break;
  }


  return retCode;
}

bool GLWFBO::createSBO(int WindowWidth, int WindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, &m_id);

    // Create the depth buffer
    glGenTextures(1, &m_depth);
    glBindTexture(GL_TEXTURE_2D, m_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create the cube map
    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (uint i = 0 ; i < 6 ; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, WindowWidth, WindowHeight, 0, GL_RED, GL_FLOAT, NULL);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);

    // Disable reads from the color buffer
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    return true;
}



void GLWFBO::createVBO()
{
  GLWVertexArray o;
  GLWVBOFactory::glwCenteredRectangle(o);

  m_vbo = GLWVBO(o,
                glm::vec3(0, 0, 0),
                glm::vec3(0, 0, 1),	glm::radians(0.0f),
                glm::vec3(0, 1, 0),	glm::radians(0.0f),
                glm::vec3(m_width, m_height, 1));
}
