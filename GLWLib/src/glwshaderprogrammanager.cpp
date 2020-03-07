#include "glwshaderprogrammanager.h"
#include "logger.h"

LDEFINECLASS("GLW", "GLWShaderProgramManager")

using glw::engine::buffers::GLWFBO;
using glw::engine::glsl::GLWShaderProgram;
using glw::engine::glsl::GLWShaderProgramManager;
using glw::GLWReturnCode::GLW_FAIL;
using glw::GLWReturnCode::GLW_SUCCESS;

GLWShaderProgramManager::GLWShaderProgramManager()
{
}

GLWShaderProgramManager::~GLWShaderProgramManager()
{
}

GLWShaderProgram* GLWShaderProgramManager::loadProgram(GLWShaderProgramId id)
{
  m_shaderPrograms[id].load();
  m_currentProgram = id;
  return getProgram(id);
}

GLWReturnCode GLWShaderProgramManager::addNewProgram(
    GLWShaderProgramId& shaderProgramId,
    const char* vertexFilePath,
    const char* fragmentFilePath,
    const char* geometryFilePath)
{
  GLWShaderProgram program = GLWShaderProgram(vertexFilePath, fragmentFilePath, geometryFilePath);
  GLWReturnCode success    = program.isValid() ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS != success)
  {
    shaderProgramId = NULL_SHADER_PROGRAM_ID;
    LERROR("GLWShaderProgram not added to GLWShaderProgramManager.");
  }
  else
  {
    m_shaderPrograms.insert({program.getId(), program});
    shaderProgramId = program.getId();
    LINFO("GLWShaderProgram added to GLWShaderProgramManager.");
  }
  return success;
}

GLWReturnCode GLWShaderProgramManager::addNewProgram(
    const char* vertexFilePath,
    const char* fragmentFilePath,
    glm::mat4* model_data,
    glm::mat4* view_data,
    glm::mat4* proj_data,
    GLWShaderProgramId& shaderProgramId,
    const char* geometry_file_path)
{
  GLWShaderProgram program = GLWShaderProgram(vertexFilePath, fragmentFilePath, geometry_file_path);
  GLWReturnCode success    = program.isValid() ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS != success)
  {
    shaderProgramId = NULL_SHADER_PROGRAM_ID;
    LERROR("GLWShaderProgram not added to GLWShaderProgramManager.");
  }
  else
  {
    if (GLW_SUCCESS != program.setModelMat4Handle(model_data))
    {
      success = GLW_FAIL;
    }
    if (GLW_SUCCESS != program.setViewMat4Handle(view_data))
    {
      success = GLW_FAIL;
    }
    if (GLW_SUCCESS != program.setProjMat4Handle(proj_data))
    {
      success = GLW_FAIL;
    }
    if (GLW_SUCCESS == success)
    {
      m_shaderPrograms.insert({program.getId(), program});
      shaderProgramId = program.getId();
      LINFO("GLWShaderProgram added to GLWShaderProgramManager.");
    }
  }
  return success;
}

GLWReturnCode GLWShaderProgramManager::addNewProgram(GLWShaderProgram program)
{
  GLWReturnCode success = program.isValid() ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS != success)
  {
    LERROR("GLWShaderProgram not added to GLWShaderProgramManager.");
  }
  else
  {
    m_shaderPrograms.insert({program.getId(), program});
    LINFO("GLWShaderProgram added to GLWShaderProgramManager.");
  }
  return success;
}

GLWShaderProgram* GLWShaderProgramManager::getProgram(GLWShaderProgramId id)
{
  return &m_shaderPrograms[id];
}

GLWShaderProgram* GLWShaderProgramManager::getCurrentProgram()
{
  return &m_shaderPrograms[m_currentProgram];
}

void GLWShaderProgramManager::drawVBO(GLWVBO& vbo)
{
  GLWShaderProgram* shaderProgram = getCurrentProgram();
  if (NULL != shaderProgram)
  {
    vbo.draw(shaderProgram->getShaderHandle());
  }
  else
  {
    LERROR("GLWShaderProgram is NULL");
  }
}

void GLWShaderProgramManager::drawVBO(GLWVBO* vbo)
{
  if (NULL != vbo)
  {
    GLWShaderProgram* shaderProgram = getCurrentProgram();
    if (NULL != shaderProgram)
    {
      vbo->draw(shaderProgram->getShaderHandle());
    }
    else
    {
      LERROR("GLWShaderProgram is NULL");
    }
  }
  else
  {
    LERROR("GLWVBO is NULL");
  }
}

void GLWShaderProgramManager::drawVBOs(std::vector<GLWVBO*>& vbos)
{
  GLWShaderProgram* shaderProgram = getCurrentProgram();
  if (NULL != shaderProgram)
  {
    for (GLWVBO* vbo : vbos)
    {
      if (NULL != vbo)
      {
        vbo->draw(shaderProgram->getShaderHandle());
      }
      else
      {
        LERROR("VBO is NULL");
      }
    }
  }
  else
  {
    LERROR("GLWShaderProgram is NULL");
  }
}

void GLWShaderProgramManager::drawVBOs(std::vector<GLWVBO>& vbos)
{
  GLWShaderProgram* shaderProgram = getCurrentProgram();
  if (NULL != shaderProgram)
  {
    for (GLWVBO& vbo : vbos)
    {
      vbo.draw(shaderProgram->getShaderHandle());
    }
  }
  else
  {
    LERROR("GLWShaderProgram is NULL");
  }
}

void GLWShaderProgramManager::drawFBO(GLWFBO& fbo)
{
  GLWShaderProgram* shaderProgram = getCurrentProgram();
  if (NULL != shaderProgram)
  {
    fbo.draw(shaderProgram->getShaderHandle());
  }
  else
  {
    LERROR("GLWShaderProgram is NULL");
  }
}
