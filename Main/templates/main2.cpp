#include <stdio.h>

#include "logger.h"
#include "stringformat.h"
#include "glwmanager.h"
#include "glwvbo.h"
#include "glwvbofactory.h"
#include "glwcolor.h"
#include "glwfbo.h"
#include "glwfbomanager.h"

LDEFINECLASS("MAIN", "main")

using glw::util::Logger;
using glw::util::StringFormat;
using glw::GLWManager;
using glw::GLWReturnCode;
using glw::GLW_SUCCESS;
using glw::GLW_FAIL;
using glw::engine::GLWContext;
using glw::engine::glsl::GLWShaderProgramManager;
using glw::engine::GLWVBOManager;
using glw::engine::buffers::GLWFBOManager;
using glw::engine::buffers::GLWVBO;
using glw::engine::GLWCamera;
using glw::engine::buffers::GLWVertex;
using glw::engine::buffers::GLWVertexArray;
using glw::engine::buffers::GLWVec3Array;
using glw::engine::buffers::GLWVec2Array;
using glw::engine::buffers::GLWVBOArray;
using glw::engine::buffers::GLWVBOFactory;
using glw::engine::buffers::GLWFBO;
using glw::engine::buffers::GLWFBOId;


namespace
{
  GLWShaderProgramId BASIC_PROGRAM, GUI_PROGRAM;
  GLWFBOId TEST_FBO;

  GLWVBO sphere;
  GLWVBO pane;
  GLWVBO whitePane;
  GLWVBO mouseBox;

  GLWCamera camera(glm::vec3(0, 0, 5),  // Position
                   glm::vec3(),         // Velocity
                   glm::vec3(0,0,-1),   // Direction
                   glm::vec3(0, 1, 0)); // Up Vector
}


GLWReturnCode loop(GLWContext& context,
                   GLWShaderProgramManager& shaderProgramManager,
                   GLWVBOManager& vboManager,
                   GLWFBOManager& fboManager)
{
  if(GLWManager::getInstance().getKeyboard().isKeyDown(GLFW_KEY_LEFT_SHIFT))
  {
    // Spin the sphere
    sphere.m_theta += GLWManager::getInstance().getMouse().popScrollDelta() * 0.1f;
  }
  else
  {
    sphere.m_theta += 0.01f;
  }

  mouseBox.m_pos = glm::vec3(GLWManager::getInstance().getMouse().getMousePosition(), 0);


  // Update context camera
  camera.update(0.1f, 0.9f);
  context.setCamera(camera);



  // Set clear colour for context
  context.setClearColor(glw::GREY / 4.0f);

  // Load shader to draw with
  shaderProgramManager.loadProgram(BASIC_PROGRAM);

  // Bind and draw to FBO
  fboManager.bind(TEST_FBO);
  {
    // Clear screen and load ortho view
    context.clearAll();
    context.loadExternalOrtho();

    // Draw white background, this will show up when drawing the FBO texture
    shaderProgramManager.drawVBO(whitePane);

    // Clear depth buffer only to retain the white background and load 3D view
    context.clearDepthBuffer();
    context.load3DPerspective();

    // Draw the sphere
    shaderProgramManager.drawVBO(sphere);
  }
fboManager.unbind(TEST_FBO); // Stop drawing to FBO by unbinding it

  // Clear screen read to draw scene with predrawn FBO texture
  context.clearAll();

  // Draw sphere
  shaderProgramManager.drawVBO(sphere);
  // Set the 3D pane texture as the FBO texture we have just drawn
  pane.setTexId(fboManager.getFBOTexId(TEST_FBO));
  // Draw the FBO pane
  shaderProgramManager.drawVBO(pane);

//  shaderProgramManager.loadProgram(BASIC_PROGRAM);

  context.clearDepthBuffer();
  context.loadExternalOrtho();

  shaderProgramManager.drawVBO(mouseBox);

  return GLW_SUCCESS;
}

GLWReturnCode init(GLWContext& context,
                   GLWShaderProgramManager& shaderProgramManager,
                   GLWVBOManager& vboManager,
                   GLWFBOManager& fboManager)
{
  GLWReturnCode success = GLW_SUCCESS;

  // Set window size
  context.setWindowSize(glm::vec2(1280,720));

  LINFO("Initialising GLSL shader programs...");

  // Add a new program to the manager
  if (GLW_SUCCESS == shaderProgramManager.addNewProgram(
        "shaders/basic.vert",  // Vertex shader
        "shaders/basic.frag",  // Fragment shader
        context.getModelMat(),
        context.getViewMat(),
        context.getProjMat(),
        BASIC_PROGRAM          // Supply the id container
        ))
  {
    // Get the shader program we have just created
    GLWShaderProgram * shaderProgram = shaderProgramManager.getProgram(BASIC_PROGRAM);

    // Check the program has been created
    if (NULL != shaderProgram)
    {
      shaderProgram->setTexHandle();
    }
    else
    {
      success = GLW_FAIL;
      LERROR("BASIC_PROGRAM is NULL");
    }
  }
  else
  {
    success = GLW_FAIL;
    LERROR("Failed to add BASIC_PROGRAM");
  }

  // INIT VBOS //

  if (GLW_SUCCESS == success)
  {
    LINFO("Initialising VBOs...");

    // Create array container for vertices
    GLWVertexArray o;

    LINFO("Generating Sphere...");

    GLWVBOFactory::glwSphere(o);

    // Create a new VBO with our new vertex array
    sphere = GLWVBO(
          o,                      // Supply the vertex array
          glm::vec3(),            // World position
          glm::vec3(0, 1, 0),     // Rotation axis
          glm::radians(0.0f),     // Rotation angle
          glm::vec3(1, 0, 0),     // Pre-rotation axis
          glm::radians(0.0f),     // Pre-rotation angle
          glm::vec3(1),           // Scale vector
          "textures/cubemap.jpg");

    GLWVBOFactory::glwRectangle(o);

    // Create a new VBO with our new vertex array
    pane = GLWVBO(
          o,                      // Supply the vertex array
          glm::vec3(2,0,0),            // World position
          glm::vec3(0, 1, 0),     // Rotation axis
          glm::radians(0.0f),     // Rotation angle
          glm::vec3(1, 0, 0),     // Pre-rotation axis
          glm::radians(0.0f),     // Pre-rotation angle
          glm::vec3(1,0.9,1.6)           // Scale vector
          );

    GLWVBOFactory::glwCenteredRectangle(o, glw::WHITE);

    // Create a new VBO with our new vertex array
    whitePane = GLWVBO(
          o,                      // Supply the vertex array
          glm::vec3(),            // World position
          glm::vec3(0, 1, 0),     // Rotation axis
          glm::radians(0.0f),     // Rotation angle
          glm::vec3(1, 0, 0),     // Pre-rotation axis
          glm::radians(0.0f),     // Pre-rotation angle
          glm::vec3(1280,720,0)           // Scale vector
          );

     GLWVBOFactory::glwCenteredRectangle(o, glw::BLACK);

    // Create a new VBO with our new vertex array
    mouseBox = GLWVBO(
          o,                      // Supply the vertex array
          glm::vec3(),            // World position
          glm::vec3(0, 1, 0),     // Rotation axis
          glm::radians(0.0f),     // Rotation angle
          glm::vec3(1, 0, 0),     // Pre-rotation axis
          glm::radians(0.0f),     // Pre-rotation angle
          glm::vec3(10,10,0)           // Scale vector
          );
  }

  // Add new FBO to manager, save the Id for later use
  fboManager.addFBO(TEST_FBO, context.getWindowSize());

  return success;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
      GLWManager::getInstance().stop();
      break;
    }
  }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_RIGHT:

      break;
    }
  }
}

int main()
{
  LSTARTLOGGER("logs/GLW");

  LINFO("Program started.");


  GLWManager::getInstance().run(loop, init, key_callback, mouse_button_callback);


  LINFO("Program exit.");

  LENDLOGGER();

  return 0;
}
