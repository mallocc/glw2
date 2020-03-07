#include "glwshadervariablehandle.h"
#include "logger.h"
#include "stringformat.h"

LDEFINECLASS("GLW", "GLWShaderVariableHandle")

using glw::engine::glsl::GLWShaderVariableHandle;
using glw::engine::glsl::GLWShaderVariableHandleId;
using glw::util::StringFormat;

namespace
{ 
  enum
	{
		NO_HANDLE,
		MAT4_HANDLE,
		VEC3_HANDLE,
		VEC4_HANDLE,
		FLOAT_HANDLE,
		GLUINT_HANDLE,
		INT_HANDLE
	};
	
	const GLuint NULL_UNIFORM_LOCATION = -1;
}


GLWShaderVariableHandle::GLWShaderVariableHandle()
{ 
}

GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier)
    :
    m_type(NO_HANDLE),
    m_variableIdentifier(variableIdentifier)
{
}

GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier, glm::mat4 * data)
    :
    m_type(MAT4_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_m4(data)
{
}

GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier, glm::vec3 * data)
    :
    m_type(VEC3_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_v3(data)
{
}
GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier, glm::vec4 * data)
    :
    m_type(VEC4_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_v4(data)
{
}

GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier, GLfloat   * data)
    :
    m_type(FLOAT_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_glf(data)
{
}

GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier, GLuint    * data)
    :
    m_type(GLUINT_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_gli(data)
{
}

GLWShaderVariableHandle::GLWShaderVariableHandle(const char * variableIdentifier, int       * data)
    :
    m_type(INT_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_i(data)
{
}

GLWShaderVariableHandle::~GLWShaderVariableHandle()
{
}

GLWReturnCode GLWShaderVariableHandle::init(const GLuint program)
{
  GLWReturnCode success = GLW_SUCCESS;

	m_id = glGetUniformLocation(program, m_variableIdentifier);
	if (NULL_UNIFORM_LOCATION == m_id)
	{
	  success = GLW_FAIL;
    LERROR(StringFormat("GLWShaderProgram Id %0, failed linking %1")
      .arg(program).arg(m_variableIdentifier).str());
	}
	else
	{
    LINFO(StringFormat("GLWShaderProgram Id %0, linking %1 to GLWShaderVariableHandleId %2")
        .arg(program).arg(m_variableIdentifier).arg(m_id).str());
  }
  
  return success;
}

void GLWShaderVariableHandle::load()
{
  switch (m_type)
  {
  case MAT4_HANDLE:
			glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(*m_data_m4));
			break;
	case VEC3_HANDLE:
			glUniform3f(m_id, m_data_v3->x, m_data_v3->y, m_data_v3->z);
			break;
	case VEC4_HANDLE:
			glUniform4f(m_id, m_data_v4->x, m_data_v4->y, m_data_v4->z, m_data_v4->w);
	  break;
	case FLOAT_HANDLE:
			glUniform1f(m_id, *m_data_glf);
	  break;
	case GLUINT_HANDLE:
			glUniform1i(m_id, *m_data_gli);
	  break;
	case INT_HANDLE:
			glUniform1i(m_id, *m_data_i);
	  break;
	case NO_HANDLE:
      break;
  default:
    LERROR(StringFormat("GLWShaderVariableHandle %0 has not been constructed properly.")
        .arg(m_variableIdentifier).str());
    break;
  }
}

void GLWShaderVariableHandle::load(glm::mat4 data)
{
	glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(data));
}

void GLWShaderVariableHandle::load(glm::vec3 data)
{
	glUniform3f(m_id, data.x, data.y, data.z);
}

void GLWShaderVariableHandle::load(glm::vec4 data)
{
	glUniform4f(m_id, data.x, data.y, data.z, data.w);
}

void GLWShaderVariableHandle::load(GLfloat data)
{
	glUniform1f(m_id, data);
}

void GLWShaderVariableHandle::load(GLuint data)
{
	glUniform1i(m_id, data);
}

void GLWShaderVariableHandle::load(int data)
{
	glUniform1i(m_id, data);
}

glm::mat4 * GLWShaderVariableHandle::getMat4()
{
  return m_data_m4;
}

glm::vec3 * GLWShaderVariableHandle::getVec3()
{
  return m_data_v3;
}

glm::vec4 * GLWShaderVariableHandle::getVec4()
{
  return m_data_v4;
}

GLfloat   * GLWShaderVariableHandle::getGLfloat()
{
  return m_data_glf;
}

GLuint    * GLWShaderVariableHandle::getGLuint()
{
  return m_data_gli;
}

int       * GLWShaderVariableHandle::getInt()
{
  return m_data_i;
}

const char * GLWShaderVariableHandle::getVariableIdentifier()
{
  return m_variableIdentifier;
}	    

GLWShaderVariableHandleId GLWShaderVariableHandle::getHandleId()
{
  return m_id;
}
