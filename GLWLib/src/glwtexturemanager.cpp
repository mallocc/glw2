#include "glwtexturemanager.h"
#include "logger.h"
#include "stringformat.h"
#include "glwimageloader.h"

LDEFINECLASS("GLW", "GLWTextureManager")

using glw::util::StringFormat;
using glw::engine::GLWTextureManager;
using glw::util::ImageLoader;


GLWTextureManager::GLWTextureManager()
  : m_textureMap()
{
}


GLWTextureManager::~GLWTextureManager()
{
}

GLuint GLWTextureManager::importTexture(std::string filename)
{
  GLuint id = NULL_TEX;

  if (filename != NULL_FILE)
  {
     id = ImageLoader::loadTextureFromImage(filename.c_str());

     if (id != NULL_TEX)
     {
       this->addTexture(filename, id);
     }
     else
     {
       LERROR("Failed to load texture");
     }
  }
  else
  {
    LERROR("Invalid file");
  }

  return id;
}

void GLWTextureManager::addTexture(std::string filename, GLuint id)
{
  m_textureMap[filename] = id;
}

void GLWTextureManager::releaseTexture(std::string filename)
{
  m_textureMap.erase(filename);
}

void GLWTextureManager::releaseTexture(GLuint id)
{
  std::map<std::string, GLuint>::iterator itor = m_textureMap.begin();

  bool found = false;

  while (itor != m_textureMap.end() && !found)
  {
    if (itor->second == id)
    {
      m_textureMap.erase(itor);
      found = true;
    }
    else
    {
      ++itor;
    }
  }
}

GLuint GLWTextureManager::getTextureId(std::string filename)
{
  GLuint id = NULL_TEX;

  std::map<std::string, GLuint>::iterator itor = m_textureMap.find(filename);

  if (itor != m_textureMap.end())
  {
    id = itor->second;
  }

  if (id == NULL_TEX)
  {
    id = importTexture(filename);
  }

  return id;
}

std::string  GLWTextureManager::getTextureFilename(GLuint id)
{
  std::string filename = NULL_FILE;

  std::map<std::string, GLuint>::iterator itor = m_textureMap.begin();

  bool found = false;

  while (itor != m_textureMap.end() && !found)
  {
    if (itor->second == id)
    {
      filename = itor->first;
      found = true;
    }
    else
    {
      ++itor;
    }
  }

  return filename;
}
