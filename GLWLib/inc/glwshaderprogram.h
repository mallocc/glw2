#pragma once

#include "opengl.h"
#include "glm.h"

#include "glwshadervariablehandle.h"
#include "glwshaderhandle.h"

#include <map>

#define NULL_SHADER_PROGRAM_ID  255
#define VAR_LIGHT_POSITION      "u_light_pos"
#define VAR_LIGHT_COLOR         "u_light_color"
#define VAR_LIGHT_PROPERTIES    "u_light_properties"
#define VAR_EYE_POSITION        "u_eye_pos"
#define VAR_AMBIENT_COLOR       "u_ambient_color"
#define VAR_TEXTURE             "u_tex"
#define VAR_MATRIX_MODEL        "u_m"
#define VAR_TEXTURE_VIEW        "u_v"
#define VAR_TEXTURE_PROJECTION  "u_p"
#define VAR_FAR_PLANE           "u_far_plane"


using glw::engine::glsl::GLWShaderVariableHandle;
using glw::engine::glsl::GLWShaderHandle;


namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      typedef GLuint GLWShaderProgramId;
    
      class GLWShaderProgram
      {
      public:
        
        GLWShaderProgram();
        GLWShaderProgram(const char * vertexFilePath,
                       const char * fragmentFilePath,
                       const char * geometryFilePath = NULL);
        
        ~GLWShaderProgram();
        
        GLWReturnCode addHandle(GLWShaderVariableHandle handle);
        GLWReturnCode addHandle(GLWShaderVariableHandle handle, GLWShaderVariableHandleId& id);

			  void load();

			  GLWShaderVariableHandle * getHandle(GLWShaderVariableHandleId id);

			  GLWShaderProgramId getId();

        GLWReturnCode setModelMat4Handle(glm::mat4 * mat);

        GLWReturnCode setViewMat4Handle(glm::mat4 * mat);

        GLWReturnCode setProjMat4Handle(glm::mat4 * mat);

        GLWReturnCode setTexHandle();

			  GLWShaderVariableHandle * getModelMat4Handle();

			  GLWShaderVariableHandle * getViewMat4Handle();

			  GLWShaderVariableHandle * getProjMat4Handle();

			  GLWShaderVariableHandle * getTexHandle();

        GLWShaderHandle getShaderHandle();
        void getShaderHandle(GLWShaderHandle& shaderHandle);
			  
			  bool isValid() const;

      private:
        bool m_valid;
      
        GLWShaderProgramId m_id;

				GLuint m_vertexShaderId;
				GLuint m_fragmentShaderId;
        GLuint m_geometryShaderId;

				const char * m_vertexFilePath;
				const char * m_fragmentFilePath;

				std::map<GLWShaderVariableHandleId, GLWShaderVariableHandle> m_handles;

				GLWShaderVariableHandleId
            m_modelMat, m_viewMat, m_projMat,
            m_color,
            m_flag,
            m_tex, m_tex1;
      };
    }
  }
}
