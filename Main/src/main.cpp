#include "entitymanager.h"
#include "glwcolor.h"
#include "glwfbo.h"
#include "glwfbomanager.h"
#include "glwmanager.h"
#include "glwvbo.h"
#include "glwvbofactory.h"
#include "glwvbomanager.h"
#include "guidrawutilities.h"
#include "guimanager.h"
#include "logger.h"
#include "stringformat.h"
#include <chrono>
#include <math.h>
#include <mutex>
#include <stdio.h>

LDEFINECLASS("MAIN", "main")

using ent::Entity;
using ent::EntityManager;
using glw::GLW_DATA_FULL;
using glw::GLW_FAIL;
using glw::GLW_SUCCESS;
using glw::GLWManager;
using glw::GLWReturnCode;
using glw::engine::GLWCamera;
using glw::engine::GLWContext;
using glw::engine::buffers::GLWFBO;
using glw::engine::buffers::GLWFBOId;
using glw::engine::buffers::GLWFBOManager;
using glw::engine::buffers::GLWVBO;
using glw::engine::buffers::GLWVBOArray;
using glw::engine::buffers::GLWVBOFactory;
using glw::engine::buffers::GLWVBOId;
using glw::engine::buffers::GLWVBOManager;
using glw::engine::buffers::GLWVec2Array;
using glw::engine::buffers::GLWVec3Array;
using glw::engine::buffers::GLWVertex;
using glw::engine::buffers::GLWVertexArray;
using glw::engine::glsl::GLWShaderProgramManager;
using glw::util::Logger;
using glw::util::StringFormat;
using gui::GUIManager;

namespace
{
  GLWShaderProgramId BASIC_PROGRAM;

  GLWVBOId mouseBox;

  GLWCamera camera(glm::vec3(0, 0, 5),  // Position
                   glm::vec3(),         // Velocity
                   glm::vec3(0, 0, -1), // Direction
                   glm::vec3(0, 1, 0)); // Up Vector

  float dt       = 0.1f;
  float friction = 0.9f;

  glm::vec2 g_windowSize = glm::vec2(1280, 720);
  GUIManager g_guiManager(glm::vec3(g_windowSize, 0));
  GLWShaderProgramManager* g_shaderProgramManager;
  GLWVBOManager* g_vboManager;
  GLWVBOId GUI_RECTANGLE_VBO_ID;
} // namespace

void gui::drawRectangle(
    glm::vec3 translation,
    glm::vec3 scale,
    gui::Color color,
    float opacity,
    std::string blendMode,
    unsigned int glTextureId)
{
  if (g_vboManager != nullptr)
  {
    GLWVBO* guiRectangleVBO = g_vboManager->getVBO(GUI_RECTANGLE_VBO_ID);
    if (guiRectangleVBO != nullptr)
    {
      guiRectangleVBO->m_pos = translation;
      guiRectangleVBO->m_scale = scale;   
      guiRectangleVBO->m_tex = glTextureId;   
      if (g_shaderProgramManager != nullptr)
      {
        g_shaderProgramManager->drawVBO(guiRectangleVBO);
      }
    }
  }
}

GLWReturnCode loop(GLWContext& context,
                   GLWShaderProgramManager& shaderProgramManager,
                   GLWVBOManager& vboManager,
                   GLWFBOManager& fboManager)
{
  // Update context camera
  camera.update(dt, friction);
  context.setCamera(camera);

  // Set clear colour for context
  context.setClearColor(glw::GREY / 4.0f);

  // Load shader to draw with
  shaderProgramManager.loadProgram(BASIC_PROGRAM);

  // Clear screen read to draw scene with predrawn FBO texture
  context.clearAll();
  context.loadExternalOrtho();

  GLWVBO* mouseBoxVbo = vboManager.getVBO(mouseBox);
  if (NULL != mouseBoxVbo)
  {
    mouseBoxVbo->m_pos = glm::vec3(GLWManager::getInstance().getMouse().getMousePosition(), 0);
    shaderProgramManager.drawVBO(mouseBoxVbo);

    EntityManager::getInstance().draw(shaderProgramManager, vboManager);
  }

  g_guiManager.draw();

  return GLW_SUCCESS;
}

void entityLoop(ent::EntityMap& entities, float dt)
{
  glm::vec2 bounds(GLWManager::getInstance().getContext().getWindowSize());

  for (ent::EntityMapPair const& pair : entities)
  {
    Entity* e = pair.second;
    if (NULL != e)
    {
      glm::vec2 fmodded(fmod(e->m_pos.x, bounds.x), fmod(e->m_pos.y, bounds.y));
      if (fmodded.x < 0.0f)
      {
        fmodded.x += bounds.x;
      }
      if (fmodded.y < 0.0f)
      {
        fmodded.y += bounds.y;
      }
      e->m_pos = fmodded;
    }
  }

  //  glm::vec2 center(GLWManager::getInstance().getContext().getWindowSize() / 2.0f);
  glm::vec2 center(GLWManager::getInstance().getMouse().getMousePosition());

  float mass = 1000;

  for (ent::EntityMapPair const& pair : entities)
  {
    Entity* e = pair.second;
    if (NULL != e)
    {
      glm::vec2 dir = center - e->m_pos;
      float d       = glm::length(dir) + 1.0f;
      dir           = glm::normalize(dir);
      glm::vec2 acc = mass * dir / (d * d);
      e->force(acc, dt);

      e->m_vel *= 0.99f;

      for (ent::EntityMapPair const& pair2 : entities)
      {
        if (pair2.first != pair.first)
        {
          Entity* e2 = pair2.second;

          if (NULL != e2)
          {
            dir = e2->m_pos - e->m_pos;
            d   = glm::length(dir) + 1.0f;
            dir = glm::normalize(dir);
            acc = 1.0f * dir / (d * d);
            e->force(acc, dt);
          }
        }
      }
    }
  }
}

void inputThread()
{
  glm::vec2 windowSize = GLWManager::getInstance().getContext().getWindowSize();
  glm::vec2 center     = windowSize / 2.0f;

  while (true)
  {
    if (GLWManager::getInstance().getMouse().isLeftButtonDown())
    {
      EntityManager::getInstance()
          .addEntity(new Entity(
              glm::vec2(GLWVBOFactory::pg_randf() * windowSize.x, GLWVBOFactory::pg_randf() * windowSize.y),
              glm::vec2(),
              mouseBox));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

GLWReturnCode init(GLWContext& context,
                   GLWShaderProgramManager& shaderProgramManager,
                   GLWVBOManager& vboManager,
                   GLWFBOManager& fboManager)
{
  GLWReturnCode success = GLW_SUCCESS;

  // Set window size
  context.setWindowSize(g_windowSize);

  LINFO("Initialising GLSL shader programs...");

  // Add a new program to the manager
  if (GLW_SUCCESS == shaderProgramManager
                         .addNewProgram(
                             "shaders/basic.vert", // Vertex shader
                             "shaders/basic.frag", // Fragment shader
                             context.getModelMat(),
                             context.getViewMat(),
                             context.getProjMat(),
                             BASIC_PROGRAM // Supply the id container
                             ))
  {
    // Get the shader program we have just created
    GLWShaderProgram* shaderProgram = shaderProgramManager.getProgram(BASIC_PROGRAM);

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

    //    GLWVBOFactory::glwCenteredRectangle(o, glw::RED);
    GLWVBOFactory::glwCenteredRectangle(o, glw::RED);

    // Create a new VBO with our new vertex array
    if (!vboManager
             .addVBO(new GLWVBO(
                         o,                  // Supply the vertex array
                         glm::vec3(),        // World position
                         glm::vec3(0, 1, 0), // Rotation axis
                         glm::radians(0.0f), // Rotation angle
                         glm::vec3(1, 0, 0), // Pre-rotation axis
                         glm::radians(0.0f), // Pre-rotation angle
                         glm::vec3(3, 3, 0)  // Scale vector
                         ),
                     mouseBox))
    {
      success = GLW_FAIL;
      LERROR("Failed to add GLWVBO to GLWVBOManager");
    }

    LINFO("Generating GUI Rectangle...");

    GLWVBOFactory::glwCenteredRectangle(o, glw::GREEN);

    if (!vboManager
             .addVBO(new GLWVBO(
                         o,                  // Supply the vertex array
                         glm::vec3(),        // World position
                         glm::vec3(0, 1, 0), // Rotation axis
                         glm::radians(0.0f), // Rotation angle
                         glm::vec3(1, 0, 0), // Pre-rotation axis
                         glm::radians(0.0f), // Pre-rotation angle
                         glm::vec3(1, 1, 0)  // Scale vector
                         ),
                     GUI_RECTANGLE_VBO_ID))
    {
      success = GLW_FAIL;
      LERROR("Failed to add GLWVBO to GLWVBOManager");
    }
  }

  EntityManager::getInstance().setExternalLoop(entityLoop);

  std::thread(inputThread).detach();

  g_shaderProgramManager = &shaderProgramManager;
  g_vboManager           = &vboManager;

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

  bool success = GLW_SUCCESS == GLWManager::getInstance().run(loop, init, key_callback, mouse_button_callback);

  LINFO("Program exit.");

  LENDLOGGER();

  return !success;
}
