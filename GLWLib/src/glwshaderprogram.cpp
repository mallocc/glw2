#include "glwshaderprogram.h"
#include "logger.h"
#include "stringformat.h"
#include "glwreturncode.h"
#include <fstream>

LDEFINECLASS("GLW", "GLWShaderProgram")

using glw::engine::glsl::GLWShaderProgram;
using glw::engine::glsl::GLWShaderProgramId;
using glw::engine::glsl::GLWShaderVariableHandle;
using glw::engine::glsl::GLWShaderVariableHandleId;
using glw::engine::glsl::GLWShaderHandle;
using glw::util::StringFormat;
using glw::GLWReturnCode::GLW_SUCCESS;
using glw::GLWReturnCode::GLW_FAIL;
using glw::GLWReturnCode::GLW_INVALID;


namespace
{
  const char * VAR_NAME_MODEL_MAT = "u_m";
  const char * VAR_NAME_VIEW_MAT  = "u_v";
  const char * VAR_NAME_PROJ_MAT  = "u_p";
  const char * VAR_NAME_COLOR_VEC = "u_c";
  const char * VAR_NAME_TEX0      = "u_tex";
  const char * VAR_NAME_TEX1		  = "u_tex1";
  const char * VAR_NAME_FLAG      = "u_flag";
}


GLWShaderProgram::GLWShaderProgram()
{
}

GLWShaderProgram::GLWShaderProgram(const char * vertexFilePath,	const char * fragmentFilePath, const char * geometryFilePath)
  :
    m_valid(true)
{

  GLWReturnCode returnCode = GLW_SUCCESS;

  this->m_vertexFilePath = vertexFilePath;
  this->m_fragmentFilePath = fragmentFilePath;

  // Create the shaders
  m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::string Line = "";
    while (getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else {
    returnCode = GLW_FAIL;
    LERROR(StringFormat("Failed to open %0.")
           .arg(vertexFilePath).str());
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
  if (FragmentShaderStream.is_open()) {
    std::string Line = "";
    while (getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }
  else {
    returnCode = GLW_FAIL;
    LERROR(StringFormat("Failed to open %0.")
           .arg(fragmentFilePath).str());
  }

  std::string GeometryShaderCode;
  if (geometryFilePath != NULL)
  {
    // Read the Geometry Shader code from the file
    std::ifstream GeometryShaderStream(geometryFilePath, std::ios::in);
    if (GeometryShaderStream.is_open()) {
      std::string Line = "";
      while (getline(GeometryShaderStream, Line))
        GeometryShaderCode += "\n" + Line;
      GeometryShaderStream.close();
    }
    else {
      returnCode = GLW_FAIL;
      LERROR(StringFormat("Failed to open %0.")
             .arg(geometryFilePath).str());
    }
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(m_vertexShaderId, 1, &VertexSourcePointer, NULL);
  glCompileShader(m_vertexShaderId);

  // Check Vertex Shader
  glGetShaderiv(m_vertexShaderId, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(m_vertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    returnCode = GLW_FAIL;
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(m_vertexShaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    LERROR(StringFormat("Vertex shader compilation failed: \n%0")
           .arg(&VertexShaderErrorMessage[0]).str());
  }
  else
    LINFO(StringFormat("Vertex shader %0 compiled succesfully.").arg(vertexFilePath).str());

  // Compile Fragment Shader
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(m_fragmentShaderId, 1, &FragmentSourcePointer, NULL);
  glCompileShader(m_fragmentShaderId);

  // Check Fragment Shader
  glGetShaderiv(m_fragmentShaderId, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(m_fragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    returnCode = GLW_FAIL;
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(m_fragmentShaderId, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    LERROR(StringFormat("Fragment shader compilation failed: \n%0")
           .arg(&FragmentShaderErrorMessage[0]).str());
  }
  else
    LINFO(StringFormat("Fragment shader %0 compiled succesfully.").arg(fragmentFilePath).str());

  if (geometryFilePath != NULL)
  {
    // Compile Geometry Shader
    char const * GeometrySourcePointer = GeometryShaderCode.c_str();
    m_geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(m_geometryShaderId, 1, &GeometrySourcePointer, NULL);
    glCompileShader(m_geometryShaderId);

    // Check Geometry Shader
    glGetShaderiv(m_geometryShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(m_geometryShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
      returnCode = GLW_FAIL;
      std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
      glGetShaderInfoLog(m_geometryShaderId, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
      LERROR(StringFormat("Geometry shader compilation failed: \n%0")
             .arg(&GeometryShaderErrorMessage[0]).str());
    }
    else
      LINFO(StringFormat("Geometry shader %0 compiled succesfully.").arg(geometryFilePath).str());
  }


  // Link the program
  GLuint ProgramId = glCreateProgram();
  glAttachShader(ProgramId, m_vertexShaderId);
  glAttachShader(ProgramId, m_fragmentShaderId);
  if (geometryFilePath != NULL)
  {
    glAttachShader(ProgramId, m_geometryShaderId);
  }
  glLinkProgram(ProgramId);

  // Check the program
  glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    returnCode = GLW_FAIL;
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(ProgramId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    LERROR(StringFormat("Linking failed:\n%0\n")
           .arg(&ProgramErrorMessage[0]).str());
  }
  else
    LINFO("Program linked succesfully.");


  glDetachShader(ProgramId, m_vertexShaderId);
  glDetachShader(ProgramId, m_fragmentShaderId);
  glDetachShader(ProgramId, m_geometryShaderId);

  glDeleteShader(m_vertexShaderId);
  glDeleteShader(m_fragmentShaderId);
  glDeleteShader(m_geometryShaderId);

  this->m_id = ProgramId;

  if (GLW_FAIL == returnCode)
  {
    m_valid = false;
    LERROR(StringFormat("Loaded GShaderProgram %0 unsuccessfully.").arg(ProgramId).str());
  }
  else
  {
    LINFO(StringFormat("Loaded GShaderProgram %0 succesfully.").arg(ProgramId).str());
  }
}

GLWShaderProgram::~GLWShaderProgram()
{
}

GLWReturnCode GLWShaderProgram::addHandle(GLWShaderVariableHandle handle)
{
  GLWReturnCode success = handle.init(m_id);
  if(GLW_SUCCESS == success)
  {
    m_handles.insert({ handle.getHandleId(), handle });
  }
  else
  {
    LERROR(StringFormat("Failed to add handle.").str());
  }
  return success;
}

GLWReturnCode GLWShaderProgram::addHandle(GLWShaderVariableHandle handle, GLWShaderVariableHandleId& id)
{
  GLWReturnCode success = handle.init(m_id);
  if(GLW_SUCCESS == success)
  {
    id = handle.getHandleId();
    m_handles.insert({ handle.getHandleId(), handle });
  }
  else
  {
    LERROR(StringFormat("Failed to add handle.").str());
  }
  return success;
}

void GLWShaderProgram::load()
{
  glUseProgram(m_id);
  for (auto& sm_pair : m_handles)
  {
    sm_pair.second.load();
  }
}


GLWShaderVariableHandle * GLWShaderProgram::getHandle(GLWShaderVariableHandleId id)
{
  GLWShaderVariableHandle * handle = NULL;
  if (m_handles.find(id) != m_handles.end())
  {
    handle = &m_handles[id];
  }
  return handle;
}

GLWShaderProgramId GLWShaderProgram::getId()
{
  return m_id;
}

GLWReturnCode GLWShaderProgram::setModelMat4Handle(glm::mat4 * mat)
{
  GLWReturnCode success = NULL != mat ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS == success)
  {
    GLWShaderVariableHandle handle = GLWShaderVariableHandle(VAR_NAME_MODEL_MAT, mat);
    success = handle.init(m_id);
    if(GLW_SUCCESS == success)
    {
      m_modelMat = handle.getHandleId();
      m_handles.insert({ handle.getHandleId(), handle });
    }
    else
    {
      LERROR(StringFormat("Failed to set ModelMat4Handle.").str());
    }
  }
  else
  {
    LERROR("Matrix is NULL");
  }
  return success;
}

GLWReturnCode GLWShaderProgram::setViewMat4Handle(glm::mat4 * mat)
{
  GLWReturnCode success = NULL != mat ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS == success)
  {
    GLWShaderVariableHandle handle = GLWShaderVariableHandle(VAR_NAME_VIEW_MAT, mat);
    GLWReturnCode success = handle.init(m_id);
    if(GLW_SUCCESS == success)
    {
      m_viewMat = handle.getHandleId();
      m_handles.insert({ handle.getHandleId(), handle });
    }
    else
    {
      LERROR(StringFormat("Failed to set ViewMat4Handle.").str());
    }
  }
  else
  {
    LERROR("Matrix is NULL");
  }
  return success;
}

GLWReturnCode GLWShaderProgram::setProjMat4Handle(glm::mat4 * mat)
{
  GLWReturnCode success = NULL != mat ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS == success)
  {
    GLWShaderVariableHandle handle = GLWShaderVariableHandle(VAR_NAME_PROJ_MAT, mat);
    GLWReturnCode success = handle.init(m_id);
    if(GLW_SUCCESS == success)
    {
      m_projMat = handle.getHandleId();
      m_handles.insert({ handle.getHandleId(), handle });
    }
    else
    {
      LERROR(StringFormat("Failed to set ProjMat4Handle.").str());
    }
  }
  else
  {
    LERROR("Matrix is NULL");
  }
  return success;
}

GLWReturnCode GLWShaderProgram::setTexHandle()
{ 
  GLWReturnCode success = GLW_FAIL;
  if (isValid())
  {
    GLWShaderVariableHandle handle = GLWShaderVariableHandle(VAR_NAME_TEX0);
    success = handle.init(m_id);
    if(GLW_SUCCESS == success)
    {
      m_tex = handle.getHandleId();
      m_handles.insert({ handle.getHandleId(), handle });
    }
    else
    {
      LERROR(StringFormat("Failed to init TexHandle").str());
    }
  }
  else
  {
    success = GLW_INVALID;
    LERROR(StringFormat("GLWShaderProgram not valid - failed to set TexHandle").str());
  }
  return success;
}

GLWShaderVariableHandle * GLWShaderProgram::getModelMat4Handle()
{
  return getHandle(m_modelMat);
}

GLWShaderVariableHandle * GLWShaderProgram::getViewMat4Handle()
{
  return getHandle(m_viewMat);
}

GLWShaderVariableHandle * GLWShaderProgram::getProjMat4Handle()
{
  return getHandle(m_projMat);
}

GLWShaderVariableHandle * GLWShaderProgram::getTexHandle()
{
  return getHandle(m_tex);
}

GLWShaderHandle GLWShaderProgram::getShaderHandle()
{
  return {
    getTexHandle(),
        getModelMat4Handle(),
        getViewMat4Handle(),
        getProjMat4Handle()};
}

void GLWShaderProgram::getShaderHandle(GLWShaderHandle& shaderHandle)
{
  shaderHandle.textureHandle = getTexHandle();
  shaderHandle.modelMatHandle = getModelMat4Handle();
  shaderHandle.viewMatHandle = getViewMat4Handle();
  shaderHandle.projMatHandle = getProjMat4Handle();
}

bool GLWShaderProgram::isValid() const
{
  return m_valid;
}
