#include "glwvbo.h"
#include "glwimageloader.h"
#include "glwreturncode.h"
#include "glwshaderhandle.h"
#include "glwvertex.h"
#include "logger.h"
#include "stringformat.h"
#include <string>

LDEFINECLASS("GLW", "GLWVBO")

using glw::engine::GLWTextureManager;
using glw::engine::buffers::GLWVBO;
using glw::engine::buffers::GLWVertex;
using glw::engine::glsl::GLWShaderHandle;
using glw::GLWReturnCode::GLW_FAIL;
using glw::GLWReturnCode::GLW_SUCCESS;
using glw::util::ImageLoader;
using glw::util::StringFormat;

GLWVBO::GLWVBO() {}

// Texture filename, Vertex data pack, world position,
// dynamic axis of rotation, and amount, static axis of rotation,
// and amount, scale vector.
GLWVBO::GLWVBO(std::vector<GLWVertex> &data, glm::vec3 pos, glm::vec3 rotation,
               GLfloat theta, glm::vec3 preRotation, GLfloat preTheta,
               glm::vec3 scale, const char *texfilename)
    : m_pos(pos), m_rotation(rotation), m_theta(theta), m_scale(scale),
      m_preRotation(preRotation), m_preTheta(preTheta), m_data(data)
{
  loadTextures(texfilename);

  if (GLW_SUCCESS != init())
  {
    LERROR("Failed it initialise GVertexBufferObject.");
  }
}

GLWVBO::~GLWVBO() {}

// Buffers Vertex data into the VBO
GLWReturnCode GLWVBO::init()
{
  m_dataSize = m_data.size();
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, m_dataSize * sizeof(struct GLWVertex),
               m_data.data(), GL_STATIC_DRAW);
  glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(struct GLWVertex),
                        (const GLvoid *)offsetof(struct GLWVertex, position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE,
                        sizeof(struct GLWVertex),
                        (const GLvoid *)offsetof(struct GLWVertex, color));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE,
                        sizeof(struct GLWVertex),
                        (const GLvoid *)offsetof(struct GLWVertex, normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer((GLuint)3, 2, GL_FLOAT, GL_FALSE,
                        sizeof(struct GLWVertex),
                        (const GLvoid *)offsetof(struct GLWVertex, uv));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE,
                        sizeof(struct GLWVertex),
                        (const GLvoid *)offsetof(struct GLWVertex, tangent));
  glEnableVertexAttribArray(4);
  glBindVertexArray(0);

  return GLW_SUCCESS;
}

// Loads image file into a texture
void GLWVBO::loadTextures(const char *texfilename)
{
  const std::string filename(texfilename);
  if (filename != NULL_FILE)
  {
    m_tex = GLWTextureManager::getInstance().getTextureId(filename);

    if (m_tex == NULL_TEX)
    {
      LERROR(StringFormat("Texture file '%0' could not be loaded")
                 .arg(filename)
                 .str());
    }
  }
}

// Draws the mesh including linking the model matrix
void GLWVBO::draw(GLWShaderHandle handles, bool wireFrame)
{
  draw(glm::mat4(1), handles, wireFrame);
}

// Draws the vbo relative to a parent matrix including linking the model matrix
void GLWVBO::draw(glm::mat4 parentMatrix, GLWShaderHandle handles,
                  bool wireFrame)
{
  if (NULL != handles.modelMatHandle)
  {
    handles.modelMatHandle->load(parentMatrix * getModelMat());
  }
  if (NULL != handles.textureHandle)
  {
    handles.textureHandle->load(m_tex);
  }
  drawArray(wireFrame);
}

// Draws just the VBO and activating the texture
void GLWVBO::drawArray(bool wireFrame)
{
  // load the texture
  glActiveTexture(GL_TEXTURE0 + m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);

  // draw the data
  glBindVertexArray(m_vao);
  glDrawArrays(wireFrame ? GL_LINE_LOOP : GL_TRIANGLES, 0, m_dataSize);
  glBindVertexArray(0);

  // unload the texture
  glActiveTexture(GL_TEXTURE0 + m_tex);
  glBindTexture(GL_TEXTURE_2D, 0);
}

// Draws just the VBO and activating the texture
void GLWVBO::drawArray(int offset, int interval)
{
  // load the texture
  glActiveTexture(GL_TEXTURE0 + m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);

  // draw the data
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, offset, interval);
  glBindVertexArray(0);

  // unload the texture
  glActiveTexture(GL_TEXTURE0 + m_tex);
  glBindTexture(GL_TEXTURE_2D, 0);
}

// Get the model matrix
glm::mat4 GLWVBO::getModelMat()
{
  return glm::translate(glm::mat4(1.), m_pos) *
         glm::rotate(glm::mat4(1.), m_theta, m_rotation) *
         glm::rotate(glm::mat4(1.), m_preTheta, m_preRotation) *
         glm::scale(glm::mat4(1.), m_scale);
}

void GLWVBO::setTexId(GLuint tex) { m_tex = tex; }
