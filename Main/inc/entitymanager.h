#include "glm.h"
#include "glwreturncode.h"
#include "glwvbo.h"
#include "lock.h"
#include "logger.h"
#include "opengl.h"
#include "stringformat.h"
#include <map>
#include <thread>

using glw::GLW_FAIL;
using glw::GLW_SUCCESS;
using glw::GLWReturnCode;
using glw::engine::buffers::GLWVBO;
using glw::engine::buffers::GLWVBOId;
using glw::util::StringFormat;

#define DEFAULT_MAX_ENTITIES 100U
#define NULL_ENTITY_ID 0U
#define DEFAULT_UPDATE_INTERVAL 32U

namespace ent
{

  LDEFINECLASS("GLW", "EntityStuff")

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
        : m_entities(),
          m_maxEntities(DEFAULT_MAX_ENTITIES),
          m_mutex(),
          m_thread(),
          m_updateInterval(DEFAULT_UPDATE_INTERVAL) {}

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
      }
      return success;
    }

    GLWReturnCode removeEntity(EntityId id)
    {
      GLWReturnCode success = GLW_FAIL;

      Lock lock(m_mutex);

      std::map<EntityId, Entity*>::iterator found = m_entities.find(id);
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

  private:
    std::map<EntityId, Entity*> m_entities;

    uint32_t m_maxEntities;

    std::mutex m_mutex;

    std::thread m_thread;

    uint32_t m_updateInterval;

    void threadFunction()
    {

    }

    EntityId getNextFreeId()
    {
      EntityId id = 1U;
      for (auto const& entity : m_entities)
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