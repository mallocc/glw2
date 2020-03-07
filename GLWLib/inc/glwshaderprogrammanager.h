#pragma once

#include "glm.h"
#include "glwfbo.h"
#include "glwreturncode.h"
#include "glwshaderprogram.h"
#include "glwvbo.h"
#include "opengl.h"

using glw::GLWReturnCode;
using glw::engine::buffers::GLWFBO;
using glw::engine::buffers::GLWVBO;
using glw::engine::glsl::GLWShaderProgram;
using glw::engine::glsl::GLWShaderProgramId;

namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      class GLWShaderProgramManager
      {
      public:
        GLWShaderProgramManager();

        ~GLWShaderProgramManager();

        GLWShaderProgram* loadProgram(GLWShaderProgramId id);

        GLWReturnCode addNewProgram(
            GLWShaderProgramId& shaderProgramId,
            const char* vertex_file_path,
            const char* fragment_file_path,
            const char* geometryFilePath = NULL);

        GLWReturnCode addNewProgram(
            const char* vertex_file_path,
            const char* fragment_file_path,
            glm::mat4* model_data,
            glm::mat4* view_data,
            glm::mat4* proj_data,
            GLWShaderProgramId& shaderProgramId,
            const char* geometry_file_path = NULL);

        GLWReturnCode addNewProgram(GLWShaderProgram program);

        GLWShaderProgram* getProgram(GLWShaderProgramId id);

        GLWShaderProgram* getCurrentProgram();

        void drawVBO(GLWVBO& vbo);
        void drawVBO(GLWVBO* vbo);
        void drawVBOs(std::vector<GLWVBO*>& vbos);
        void drawVBOs(std::vector<GLWVBO>& vbos);

        void drawFBO(GLWFBO& fbo);

      private:
        std::map<GLWShaderProgramId, GLWShaderProgram> m_shaderPrograms;
        GLWShaderProgramId m_currentProgram;
      };
    } // namespace glsl
  }   // namespace engine
} // namespace glw
