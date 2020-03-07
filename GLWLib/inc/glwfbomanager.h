#ifndef GLWFBOMANAGER_H
#define GLWFBOMANAGER_H

#include "opengl.h"
#include "glwfbo.h"


using glw::engine::buffers::GLWFBO;
using glw::engine::buffers::GLWFBOId;


namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      class GLWFBOManager
      {
      public:
        GLWFBOManager();

        ~GLWFBOManager();

        GLWReturnCode addFBO(GLWFBOId& id, glm::vec2& fboSize);

        GLWFBO& getFBO(GLWFBOId id);

        GLuint getFBOTexId(GLWFBOId id);

        void bind(GLWFBOId id);
        void unbind(GLWFBOId id);

      private:
        std::map<GLWFBOId, GLWFBO> m_fbos;
      };
    }
  }
}


#endif // GLWFBOMANAGER_H
