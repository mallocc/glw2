#include "glwmanager.h"
#include "logger.h"
#include "stringformat.h"
#include "glwkeyboard.h"
#include "glwmouse.h"

LDEFINECLASS("GLW", "GLWManager")


using glw::GLWManager;
using glw::GLWReturnCode;
using glw::GLW_SUCCESS;
using glw::GLW_FAIL;
using glw::GLWLoop;
using glw::GLWInit;
using glw::util::StringFormat;
using glw::engine::glsl::GLWShaderProgramManager;
using glw::engine::GKeyboard;
using glw::engine::GMouse;
using glw::engine::GLWContext;


//Error callback
void glw::error_callback(int error, const char* description)
{
  LERROR(description);
}


GLWManager::GLWManager()
{
  LINFO("Constructing new GLWManager");
}

GLWManager::~GLWManager() {}

GLWReturnCode GLWManager::run(
    GLWLoop loop,
    GLWInit init,
    GLFWkeyfun key_callback,
    GLFWmousebuttonfun mouse_button_callback)
{
  GLWReturnCode success = GLW_SUCCESS;

  if (NULL == loop)
  {
    success = GLW_FAIL;
    LCRITICAL("GLWLoop is NULL - GLW can not run without specifing a loop function");
  }

  if (NULL == init)
  {
    LWARNING("GLWInit is NULL");
  }

  if (NULL == (m_keyfunc = key_callback))
  {
    LWARNING("GLFWkeyfun is NULL.");
  }
  if (NULL == (m_mousebuttonfunc = mouse_button_callback))
  {
    LWARNING("GLFWmousebuttonfun is NULL.");
  }

  if (GLW_SUCCESS == success)
  {
    if (GLW_SUCCESS == initWindow(init))
    {
      LINFO("Initialised content succesfully");
    }
    else
    {
      success = GLW_FAIL;
      LCRITICAL("Failed to initialise content");
    }
  }

  if (GLW_SUCCESS == success)
  {
    if (GLW_SUCCESS != glLoop(loop))
    {
      success = GLW_FAIL;
    }
  }

  if (GLW_SUCCESS == success)
  {
    LINFO("GLWManager terminated with no errors");
  }
  else
  {
    LWARNING("GLWManager terminated with errors - check logs");
  }

  return success;
}

void GLWManager::stop()
{
  m_context.exit();
}


//GL graphics loop
GLWReturnCode GLWManager::glLoop(GLWLoop loop)
{
  LINFO("Starting GL loop...");

  //Main Loop
  do
  {
    m_context.startFrameTimer();

    glfwPollEvents();
    m_mouse.updateMouse();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (NULL != loop)
    {
      loop(m_context, m_shaderProgramManager, m_vboManager, m_fboManager);
    }

    //Swap buffers
    m_context.swapBuffers();
    m_context.finishFrameTimer();

  } //Check if the ESC or Q key had been pressed or if the window had been closed
  while (!m_context.shouldExit());

  LINFO("Window has closed. Application will now exit.");

  //Close OpenGL window and terminate GLFW
  m_context.destroyWindow();
  //Finalize and clean up GLFW
  glfwTerminate();

  return GLW_SUCCESS;
}

//GL window initialise
GLWReturnCode GLWManager::initWindow(GLWInit init)
{
  GLWReturnCode success = GLW_SUCCESS;

  //Set the error callback
  glfwSetErrorCallback(error_callback);

  //Initialize GLFW
  if (!glfwInit())
  {
    LCRITICAL("Failed to init GLFW");
    success = GLW_FAIL;
  }
  else
  {
    LINFO("GLFW Initialised.");
    int VERISION_MAJOR = 4;
    int VERISION_MINOR = 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERISION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERISION_MINOR);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    LINFO(StringFormat("GLFW Context Verision: Major %0 : Minor %1").arg(VERISION_MAJOR).arg(VERISION_MINOR).str());
  }

  if(GLW_SUCCESS == success)
  {
    success = m_context.createWindow();

    if (NULL != m_context.getWindow())
    {
      m_keyboard = GKeyboard(glfwGetCurrentContext(), KEY_CALLBACK, CHARACTER_CALLBACK);
      m_mouse = GMouse(glfwGetCurrentContext(), MOUSE_BUTTON_CALLBACK, CURSOR_POSITION_CALLBACK, SCROLL_CALLBACK);
    }
    else
    {
      LCRITICAL("GLFWwindow is NULL.");
    }
  }

  if (GLW_SUCCESS == success)
  {
    //Initialize GLEW
    GLenum err = glewInit();
    //If GLEW hasn't initialized
    if (err != GLEW_OK)
    {
      LCRITICAL("Failed to to init GLEW.");
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      glfwTerminate();
      success = GLW_FAIL;
    }
    else
    {
      LINFO("GLEW Initialised.");
      // Enable depth test
      glEnable(GL_DEPTH_TEST);
      // Accept fragment if it closer to the camera than the former one
      glDepthFunc(GL_LESS);
      // Cull triangles which normal is not towards the camera
      glEnable(GL_CULL_FACE);
      // enable texturineg
      glEnable(GL_TEXTURE_2D);

      glEnable(GL_BLEND);

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glEnable(GL_MULTISAMPLE);

      glClearDepth(1.0f);

    }
  }

  if (GLW_SUCCESS == success)
  {
    // init
    if (NULL != init)
    {
      if (GLW_SUCCESS == init(m_context, m_shaderProgramManager, m_vboManager, m_fboManager))
      {
        LINFO("GLW initialisation succesful.");
      }
      else
      {
        success = GLW_FAIL;
        LCRITICAL("GLW initialisation failed.");
      }
    }
  }

  return success;
}

GLWShaderProgramManager& GLWManager::getShaderProgramManager()
{
  return m_shaderProgramManager;
}

GLWContext& GLWManager::getContext()
{
  return m_context;
}

GKeyboard& GLWManager::getKeyboard()
{
  return m_keyboard;
}

GMouse& GLWManager::getMouse()
{
  return m_mouse;
}

GLFWkeyfun& GLWManager::getKeyCallback()
{
  return m_keyfunc;
}

GLFWmousebuttonfun& GLWManager::getMouseCallback()
{
  return m_mousebuttonfunc;
}


