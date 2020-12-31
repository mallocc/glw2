#include "glm.h"
#include "glwreturncode.h"
#include "glwvbo.h"
#include "lock.h"
#include "logger.h"
#include "opengl.h"
#include "stringformat.h"
#include "glwvbomanager.h"
#include "glwshaderprogrammanager.h"
#include <map>
#include <thread>
#include <chrono>

using glw::GLW_FAIL;
using glw::GLW_SUCCESS;
using glw::GLW_DATA_FULL;
using glw::GLWReturnCode;
using glw::engine::buffers::GLWVBO;
using glw::engine::buffers::GLWVBOId;
using glw::util::StringFormat;
using glw::engine::buffers::GLWVBOManager;
using glw::engine::glsl::GLWShaderProgramManager;

#define DEFAULT_MAX_ENTITIES 10000U
#define NULL_ENTITY_ID 0U
#define DEFAULT_UPDATE_INTERVAL_MS 16U
#define DEFAULT_TIMESTEP 1.0f

namespace ent
{

  LDEFINECLASS("GLW", "EntityStuff")

  class DynamicParticle
  {
    public:
    glm::vec2 m_pos, m_vel;

    DynamicParticle()
      : m_pos(),
        m_vel() {}

    DynamicParticle(glm::vec2 pos, glm::vec2 vel)
      : m_pos(pos),
        m_vel(vel) {}

    void update(float dt)
    {
      m_pos += m_vel * dt;
    }

    void force(glm::vec2 acc, float dt)
    {
      m_vel += acc * dt;
    }
  };

  typedef uint32_t EntityId;

  class Entity : public DynamicParticle
  {
  public:
    Entity()
      : DynamicParticle(),
        m_id(NULL_ENTITY_ID),
        m_vbo(NULL_GLWVBO_ID) {}

    Entity(glm::vec2 pos, glm::vec2 vel, GLWVBOId id)
      : DynamicParticle(pos, vel),
        m_id(NULL_ENTITY_ID),
        m_vbo(id) {}

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

  typedef std::pair<EntityId, Entity*> EntityMapPair;
  typedef std::map<EntityMapPair::first_type, EntityMapPair::second_type> EntityMap;

  typedef void(*EntityManagerExternalLoopFunc)(EntityMap& entities, float dt);

  class EntityManager
  {
  public:

    static EntityManager& getInstance()
    {
      static EntityManager manager;

      return manager;
    }

    EntityManager()
      : m_entities(),
        m_maxEntities(DEFAULT_MAX_ENTITIES),
        m_mutex(),
        m_updateInterval(DEFAULT_UPDATE_INTERVAL_MS),
        m_thread(&EntityManager::threadFunction, this),
        m_timeStep(DEFAULT_TIMESTEP),
        m_func(NULL)
    {
      m_thread.detach();
      LINFO("EntityManager thread detached");
    }

    void setExternalLoop(EntityManagerExternalLoopFunc func)
    {
      m_func = func;
    }

    GLWReturnCode addEntity(Entity* entity)
    {
      GLWReturnCode success = GLW_FAIL;

      Lock lock(m_mutex);

      if (m_entities.size() < DEFAULT_MAX_ENTITIES)
      {
        if (NULL != entity)
        {
          EntityId id = getNextFreeId();
          if (NULL_ENTITY_ID != id)
          {
            entity->setId(id);
            m_entities[id] = entity;
            success        = GLW_SUCCESS;
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
        LERROR("Failed to add entity - reached max entities");
        success = GLW_DATA_FULL;
      }
      return success;
    }

    GLWReturnCode removeEntity(EntityId id)
    {
      GLWReturnCode success = GLW_FAIL;

      Lock lock(m_mutex);

      EntityMap::iterator found = m_entities.find(id);
      if (found != m_entities.end())
      {
        delete found->second;
        m_entities.erase(found);
        success = GLW_SUCCESS;
        LINFO(StringFormat("Successfully removed Entity - Id: %0").arg(id).str());
      }
      else
      {
        LERROR(StringFormat("Could not remove Entity with Id: %0 - not found").arg(id).str());
      }

      return success;
    }

    void draw(GLWShaderProgramManager& shaderProgramManager, GLWVBOManager& vboManager)
    {
      Lock lock(m_swapMutex);

      for (EntityMapPair const& e : m_swapEntities)
      {
        Entity* entity = e.second;
        if (NULL != entity)
        {
          GLWVBO* vbo = vboManager.getVBO(entity->getVBO());
          if (NULL != vbo)
          {
            vbo->setPosVec2(entity->m_pos);
            shaderProgramManager.drawVBO(vbo);
          }
        }
      }
    }

  private:
    EntityMap m_entities;
    EntityMap m_swapEntities;

    uint32_t m_maxEntities;

    std::mutex m_mutex;
    std::mutex m_swapMutex;

    std::thread m_thread;

    uint32_t m_updateInterval;

    EntityManagerExternalLoopFunc m_func;

    float m_timeStep;

    void threadFunction()
    {
      bool abort = false;
      while (!abort)
      {
        m_mutex.lock();

        if (NULL != m_func)
        {
          m_func(m_entities, m_timeStep);
        }

        for (EntityMapPair const& e : m_entities)
        {
          if (NULL != e.second)
          {
            e.second->update(m_timeStep);
          }
          else
          {
            LERROR(StringFormat("Entity Id: %0 is NULL - aborting event thread").arg(e.first).str());
            abort = true;
          }
        }

        m_swapMutex.lock();

        m_swapEntities.clear();
        m_swapEntities.insert(m_entities.begin(), m_entities.end());

        m_swapMutex.unlock();

        m_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(m_updateInterval));
      }
    }

    EntityId getNextFreeId()
    {
      EntityId id = 1U;
      for (EntityMapPair const& entity : m_entities)
      {
        if (entity.first != id)
        {
          break;
        }
        ++id;
      }
      return id;
    }
  };
} // namespace ent
