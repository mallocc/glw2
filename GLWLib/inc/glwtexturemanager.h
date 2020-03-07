#pragma once

#include "opengl.h"
#include <map>


#define NULL_FILE ""
#define NULL_TEX 0U


namespace glw
{
  namespace engine
  {
    class GLWTextureManager
    {
    public:

      static GLWTextureManager& getInstance()
      {
          static GLWTextureManager instance;
          return instance;
      }

      GLWTextureManager();
      
      ~GLWTextureManager();

      GLuint importTexture(std::string filename);

      void addTexture(std::string filename, GLuint id);

      void releaseTexture(std::string filename);
      void releaseTexture(GLuint id);

      GLuint getTextureId(std::string filename);
      std::string  getTextureFilename(GLuint id);

    private:
      
      std::map<std::string, GLuint> m_textureMap;

    };
  }
}
