#pragma once

#include "glm.h"
#include "opengl.h"
#include "glwshaderhandle.h"
#include "glwvbo.h"


using glw::engine::glsl::GLWShaderHandle;
using glw::engine::buffers::GLWVBO;


namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      typedef GLuint GLWFBOId;

      // This is an FBO struct
      class GLWFBO
      {
      public:

        GLWFBO();

        ~GLWFBO();

        // Frame size
        GLWFBO(int w, int h, bool shadowmap = false);

        GLWFBO(glm::vec2 windowSize, bool shadowmap = false);

        void draw(GLWShaderHandle handles);

        // Binds FBO for render
        void bind();

        // Unbinds all FBOs
        static void unbind();

        // Gets FBO ID
        GLWFBOId getId();

        void setTopLeft(glm::vec2 topLeft);
        void setSize(glm::vec2 size);

        void setZ(float z);

        void rotation(float rotation);

        void setTex(GLuint tex);

        GLuint getTexId();

      private:
        GLWFBOId m_id;

        GLuint m_tex = 0, m_depth = 0, m_shadowMap = 0;

        int m_width = 0, m_height = 0;

        GLWVBO m_vbo;

        // Creates a FBO and its render texture and depth buffer
        GLWReturnCode createFBO();

        bool createSBO(int WindowWidth, int WindowHeight);

        void createVBO();
      };
    }
  }
}
