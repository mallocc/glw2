#include <sstream>
#include <thread>

#include "glwcontext.h"
#include "logger.h"
#include "stringformat.h"


LDEFINECLASS("GLW", "GLWContext")


using glw::engine::GLWContext;
using glw::GLWReturnCode;
using glw::GLW_SUCCESS;
using glw::GLW_FAIL;
using glw::engine::GLWCamera;
using glw::util::StringFormat;


GLWContext::GLWContext()
        :
        m_windowSize(glm::vec2(800, 800)),
        m_eyePos(glm::vec3(0, 0, -5)),
        m_eyeLookPos(glm::vec3()),
        m_up(glm::vec3(0, 1, 0)),
        m_fov(45.0f),
        m_aspectRatio(m_windowSize.x / m_windowSize.y),
        m_nearZ(0.1f),
        m_farZ(1000.0f),
        m_isometricDepth(1.0f),
        m_frames(0),
        m_window(NULL)
{
  LINFO("Constructing default GContent.");
}

GLWContext::~GLWContext() {}

GLWReturnCode GLWContext::createWindow()
{
  GLWReturnCode success = GLW_SUCCESS;

  if (NULL == m_window)
  {
    //Create a window and create its OpenGL context
    m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, "Test Window", NULL, NULL);
    //If the window couldn't be created
    if (NULL == m_window)
    {
      LERROR("Failed to open GLFW");
      glfwTerminate();
      success = GLW_FAIL;
    }
    else
    {
      LINFO("Created GLFWwindow.");

      //This function makes the context of the specified window current on the calling thread.
      glfwMakeContextCurrent(m_window);
      LINFO("GLFWwindow context set.");
    }
  }
  else
  {
    LERROR("GLFWwindow already exists - not creating a new GLFWwindow");
    success = GLW_FAIL;
  }

  if (GLW_SUCCESS == success)
  {
    glfwSetFramebufferSizeCallback(m_window, FRAMEBUFFER_SIZE_CALLBACK);
  }

  return success;
}

GLFWwindow* GLWContext::getWindow()
{

}

void GLWContext::swapBuffers()
{
  glfwSwapBuffers(m_window);
}

int GLWContext::shouldExit()
{
  return glfwWindowShouldClose(m_window);
}

void GLWContext::destroyWindow()
{
  glfwDestroyWindow(m_window);
}

void GLWContext::load3DPerspective()
{
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  m_view = getPerspectiveView();
  m_projection = getPerspective();
}

void GLWContext::loadExternalOrtho()
{
  // Enable depth test
  glDisable(GL_DEPTH_TEST);
  // Cull triangles which normal is not towards the camera
  glDisable(GL_CULL_FACE);

  m_view = getExternalOrthoView();
  m_projection = getExternalOrtho();
}

void GLWContext::loadOrtho()
{
  // Enable depth test
  glDisable(GL_DEPTH_TEST);
  // Cull triangles which normal is not towards the camera
  glDisable(GL_CULL_FACE);

  m_view = getOrthoView();
  m_projection = getOrtho();
}

void GLWContext::loadPseudoIsometric()
{
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  m_view = getPseudoIsometricView();
  m_projection = getPseudoIsometric();
}

void GLWContext::loadHyperPerspective()
{
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  m_view = getHyperPerspectiveView();
  m_projection = getHyperPerspective();
}

glm::vec3* GLWContext::getEyePos()
{
  return &m_eyePos;
}

glm::vec3* GLWContext::getEyeLookPos()
{
  return &m_eyeLookPos;
}

glm::vec3* GLWContext::getUp()
{
  return &m_up;
}

glm::mat4* GLWContext::getModelMat()
{
  return &m_model;
}

glm::mat4* GLWContext::getViewMat()
{
  return &m_view;
}

glm::mat4* GLWContext::getProjMat()
{
  return &m_projection;
}

void GLWContext::setClearColor(const glm::vec3& color)
{
  m_clearColor = glm::vec4(color, 1.0f);
}

void GLWContext::setClearColor(const glm::vec4& color)
{
  m_clearColor = color;
}

void GLWContext::clearAlpha()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GLWContext::clearDepthBuffer()
{
  glClear(GL_DEPTH_BUFFER_BIT);
}

void GLWContext::clearColorBuffer()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void GLWContext::clearColor()
{
  glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}

void GLWContext::clearBuffers()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWContext::clearAll()
{
  clearColor();
  clearBuffers();
}

void GLWContext::getWindowSize(glm::vec2& windowSize) const
{
  windowSize = m_windowSize;
}

glm::vec2& GLWContext::getWindowSize()
{
  return m_windowSize;
}

void GLWContext::setWindowSize(glm::vec2 windowSize)
{
  m_windowSize = windowSize;
  m_aspectRatio = windowSize.x/windowSize.y;

  glViewport(0, 0, windowSize.x, windowSize.y);
  glfwSetWindowSize(m_window, windowSize.x, windowSize.y);
}

void GLWContext::setEyePos(const glm::vec3& pos)
{
  m_eyePos = pos;
}

void GLWContext::setEyeLookPos(const glm::vec3& pos)
{
  m_eyeLookPos = pos;
}

void GLWContext::setUp(const glm::vec3& up)
{
  m_up = up;
}

void GLWContext::setCamera(GLWCamera& camera)
{
  m_eyePos = camera.getPosition();
  m_eyeLookPos = camera.getLookPosition();
  m_up = camera.getUp();
}

float GLWContext::getIsometricDepth() const
{
  return m_isometricDepth;
}

void GLWContext::setIsometricDepth(const float depth)
{
  m_isometricDepth = depth;
}


void GLWContext::startFrameTimer()
{
  ++m_frames;
  // start clock for this tick
  m_time = std::chrono::high_resolution_clock::now();
}

void GLWContext::finishFrameTimer()
{
  // stop clock
  std::chrono::system_clock::time_point finish = std::chrono::high_resolution_clock::now();
  int frameMs = std::chrono::duration_cast<std::chrono::milliseconds>(finish - m_time).count();

  int timeCapMs = 1000/m_fpsCap;
  long leftOverWaitMs = timeCapMs - frameMs;
  leftOverWaitMs = leftOverWaitMs < 0 ? 0 : leftOverWaitMs;

  // throttle the graphics loop to cap at a certain fps
  std::this_thread::sleep_for(std::chrono::milliseconds(leftOverWaitMs));

  // log fps
  int totalFrameMs = frameMs+leftOverWaitMs+1;
  m_fps = 1000/totalFrameMs;
  if(m_printFps && !(m_frames % (int)m_fps))
  {
    LINFO(StringFormat("FPS %0").arg(m_fps).str());
  }
}

int GLWContext::getFrames() const
{
  return m_frames;
}

void GLWContext::setPrintFps(bool print)
{
  m_printFps = print;
}

void GLWContext::setFpsCap(int fpsCap)
{
  m_fpsCap = fpsCap;
}

int GLWContext::getFpsCap()
{
  return m_fpsCap;
}

float GLWContext::getFps()
{
  return m_fps;
}

void GLWContext::exit()
{
  glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool GLWContext::isValid()
{
  return m_isValid;
}


glm::mat4 GLWContext::getExternalOrtho() const
{
  return glm::ortho(
    0.0f,
    m_windowSize.x,
    m_windowSize.y,
    0.0f,
    -m_farZ, m_farZ);
  //return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GLWContext::getExternalOrthoView() const
{
  return glm::mat4(1.0f);
  /*return glm::lookAt(
  glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
  glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
  glm::vec3(0, 1, 0));*/
}

glm::mat4 GLWContext::getOrtho() const
{
  return glm::ortho(
    0.0f + m_eyePos.x - m_windowSize.x / 2.0f,
    m_windowSize.x + m_eyePos.x - m_windowSize.x / 2.0f,
    0.0f + m_eyePos.y - m_windowSize.y / 2.0f,
    m_windowSize.y + m_eyePos.y - m_windowSize.y / 2.0f,
    -m_farZ, m_farZ);
  //return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GLWContext::getOrthoView() const
{
  return glm::mat4(1.0f);
  /*return glm::lookAt(
  glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
  glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
  glm::vec3(0, 1, 0));*/
}

glm::mat4 GLWContext::getOrthoIsometric() const
{
  return glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y, -m_farZ, m_farZ);
  //return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GLWContext::getOrthoIsometricView() const
{
  return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GLWContext::getPseudoIsometric() const
{
  return glm::perspective(glm::radians(2.864f), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GLWContext::getPseudoIsometricView() const
{
  return glm::lookAt(m_eyePos + (m_isometricDepth * 20.0f) * glm::vec3(1, 1, 1), m_eyePos, m_up);
}

glm::mat4 GLWContext::getPerspective() const
{
  return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GLWContext::getPerspectiveView() const
{
  return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GLWContext::getHyperPerspective() const
{
  return glm::perspective(glm::radians(160.0f), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GLWContext::getHyperPerspectiveView() const
{
  return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}
