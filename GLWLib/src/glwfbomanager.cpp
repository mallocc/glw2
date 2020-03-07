#include "glwfbomanager.h"
#include "logger.h"

LDEFINECLASS("GLW", "GLWFBOManager")

using glw::engine::buffers::GLWFBOManager;



GLWFBOManager::GLWFBOManager() {}

GLWFBOManager::~GLWFBOManager() {}

GLWReturnCode GLWFBOManager::addFBO(GLWFBOId& id, glm::vec2& fboSize)
{
  GLWFBO fbo = GLWFBO(fboSize);
  m_fbos.insert({ fbo.getId(), fbo });
  id = fbo.getId();
  LINFO("GLWFBO added to GLWFBOManager.");
}

GLWFBO& GLWFBOManager::getFBO(GLWFBOId id)
{
  return m_fbos[id];
}

GLuint GLWFBOManager::getFBOTexId(GLWFBOId id)
{
  return m_fbos[id].getTexId();
}

void GLWFBOManager::bind(GLWFBOId id)
{
  m_fbos[id].bind();
}

void GLWFBOManager::unbind(GLWFBOId id)
{
  m_fbos[id].unbind();
}
