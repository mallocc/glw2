#pragma once

#include "opengl.h"
#include "glm.h"

#include "glwshaderhandle.h"
#include "glwvertex.h"
#include "glwshadervariablehandle.h"
#include "glwreturncode.h"

#include "glwtexturemanager.h"

#include <vector>


using glw::engine::glsl::GLWShaderHandle;
using glw::engine::buffers::GLWVertex;
using glw::engine::glsl::GLWShaderVariableHandle;
using glw::GLWReturnCode;


namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      class GLWVBO
      {
      public:
        
        GLWVBO();
        
        // Texture filename, Vertex data pack, world position, 
        // dynamic axis of rotation, and amount, static axis of rotation, 
        // and amount, scale vector. 
        GLWVBO(
            std::vector<GLWVertex>& data,
				    glm::vec3 pos,
				    glm::vec3 rotation,
				    GLfloat theta,
				    glm::vec3 preRotation,
				    GLfloat preTheta,
            glm::vec3 scale = glm::vec3(1),
            const char * texfilename = NULL_FILE);
        
        ~GLWVBO();
        
        // Buffers Vertex data into the VBO
        GLWReturnCode init();

        // Loads image file into a texture
        void loadTextures(const char * texfilename);

        // Draws the vbo including linking the model matrix
        void draw(GLWShaderHandle handles, bool wireFrame = false);

        // Draws the vbo relative to a parent matrix including linking the model matrix
        void draw(glm::mat4 parentMatrix, GLWShaderHandle handles, bool wireFrame = false);

			  // Draws just the VBO and activating the texture
        void drawArray(bool wireFrame = false);

        // Draws just the VBO and activating the texture
        void drawArray(int offset, int interval);

        void setTexId(GLuint tex);


			  // Get the model matrix
			  glm::mat4 getModelMat();
        
        std::vector<GLWVertex> m_data;
      	GLuint
				  m_vao,
          m_buffer,
          m_tex = 0;
			  int
				  m_dataSize = 0;

			  glm::vec3
				  m_rotation = glm::vec3(0, 1, 0),
				  m_preRotation = glm::vec3(0, 1, 0),
				  m_scale = glm::vec3(1, 1, 1),
				  m_pos, m_vel;
			  GLfloat
				  m_theta,
				  m_preTheta;
      };

      typedef std::vector<GLWVBO> GLWVBOArray;
      typedef std::vector<GLWVBO*> GLWVBOPtrArray;
    }
  }
}
