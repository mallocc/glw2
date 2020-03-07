#include "glwcolor.h"
#include "glwfbo.h"
#include "glwfbomanager.h"
#include "glwmanager.h"
#include "glwvbo.h"
#include "glwvbofactory.h"
#include "glwvbomanager.h"
#include "logger.h"
#include "stringformat.h"
#include <mutex>
#include <stdio.h>

LDEFINECLASS("MAIN", "main")

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

typedef std::lock_guard<std::mutex> Lock;

namespace
{
  GLWShaderProgramId BASIC_PROGRAM;

  GLWVBOId mouseBox;

  GLWCamera camera(glm::vec3(0, 0, 5),  // Position
                   glm::vec3(),         // Velocity
                   glm::vec3(0, 0, -1), // Direction
                   glm::vec3(0, 1, 0)); // Up Vector
} // namespace

class DynamicParticle
{
public:
  glm::vec3 m_pos, m_vel;

  DynamicParticle()
      : m_pos(),
        m_vel() {}

  DynamicParticle(glm::vec3 pos, glm::vec3 vel)
      : m_pos(pos),
        m_vel(vel) {}

  void update(float dt)
  {
    m_pos += m_vel * dt;
  }

  void force(glm::vec3 acc, float dt)
  {
    m_vel += acc * dt;
  }
};

#define DEFAULT_MAX_ENTITIES 100U
#define NULL_ENTITY_ID 0U
typedef uint32_t EntityId;

class Entity : public DynamicParticle
{
public:
  Entity()
      : DynamicParticle(),
        m_id(NULL_ENTITY_ID),
        m_vbo(NULL_GLWVBO_ID) {}

  Entity(glm::vec3 pos, glm::vec3 vel)
      : DynamicParticle(pos, vel),
        m_id(NULL_ENTITY_ID),
        m_vbo(NULL_GLWVBO_ID) {}

  EntityId getId()
  {
    return m_id;
  }

  void setId(EntityId id)
  {
    m_id = id;
  }

  GLWVBOId getVBO()
  {
    return m_vbo;
  }

private:
  EntityId m_id;
  GLWVBOId m_vbo;
};

class EntityManager
{
public:
  EntityManager()
      : m_entites(),
        m_maxEntities(DEFAULT_MAX_ENTITIES) {}

  GLWReturnCode addEntity(Entity* entity)
  {
    GLWReturnCode success = GLW_FAIL;

    Lock lock(m_mutex);

    if (m_entites.size() < DEFAULT_MAX_ENTITIES)
    {
      if (NULL != entity)
      {
        EntityId id = getNextFreeId();
        if (NULL_ENTITY_ID != id)
        {
          entity->setId(id);
          m_entites[id] = entity;
          success       = GLW_SUCCESS;
          LINFO(StringFormat("Successfully added Entity - Id: %0").arg(id).str());
        }
        else
        {
          LERROR("Failed to add entity - unable to get next free Id");
        }
      }
      else
      {
        LERROR("Failed to add entity - entity is NULL");
      }
    }
    else
    {
      LERROR("Failed to add entity - reached max entites");
    }
    return success;
  }

  GLWReturnCode removeEntity(EntityId id)
  {
    GLWReturnCode success = GLW_FAIL;

    Lock lock(m_mutex);

    std::map<EntityId, Entity*>::iterator found = m_entites.find(id);
    if (found != m_entities.end())
    {
      delete found->second;
      m_entities.erase(found);
      success = GLW_SUCCESS;
    }
    else
    {
      LERROR(StringFormat("Could not remove Entity with Id: %0").arg(id).str());
    }
  }

private:
  std::map<EntityId, Entity*> m_entites;

  uint32_t m_maxEntities;

  EntityId getNextFreeId()
  {
    EntityId id = 1U;
    for (auto const& entity : m_entites)
    {
      if (entity.first != id)
      {
        break;
      }
      ++id;
    }
    return id;
  }

  std::mutex m_mutex;
};

GLWReturnCode loop(GLWContext& context,
                   GLWShaderProgramManager& shaderProgramManager,
                   GLWVBOManager& vboManager,
                   GLWFBOManager& fboManager)
{

  // Update context camera
  camera.update(0.1f, 0.9f);
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
  }

  return GLW_SUCCESS;
}

GLWReturnCode init(GLWContext& context,
                   GLWShaderProgramManager& shaderProgramManager,
                   GLWVBOManager& vboManager,
                   GLWFBOManager& fboManager)
{
  GLWReturnCode success = GLW_SUCCESS;

  // Set window size
  context.setWindowSize(glm::vec2(1280, 720));

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

    GLWVBOFactory::glwCenteredRectangle(o, glw::RED);

    // Create a new VBO with our new vertex array
    if (!vboManager
             .addVBO(new GLWVBO(
                         o,                   // Supply the vertex array
                         glm::vec3(),         // World position
                         glm::vec3(0, 1, 0),  // Rotation axis
                         glm::radians(0.0f),  // Rotation angle
                         glm::vec3(1, 0, 0),  // Pre-rotation axis
                         glm::radians(0.0f),  // Pre-rotation angle
                         glm::vec3(10, 10, 0) // Scale vector
                         ),
                     mouseBox))
    {
      success = GLW_FAIL;
      LERROR("Failed to add GLWVBO to GLWVBOManager");
    }
  }

  EntityManager manager;

  if (GLW_FAIL == manager.addEntity(new Entity()))
  {
    success = GLW_FAIL;
  }


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
