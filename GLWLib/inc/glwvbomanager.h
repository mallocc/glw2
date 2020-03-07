#ifndef GLWVBOMANAGER_H
#define GLWVBOMANAGER_H

#include "glwvbo.h"
#include "opengl.h"
#include <map>
#include <string>

using glw::engine::buffers::GLWVBO;

namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      #define NULL_GLWVBO_ID 0U
      typedef GLuint GLWVBOId;

      class GLWVBOManager
      {
      public:
        GLWVBOManager() {}

        GLWVBO* getVBO(const GLWVBOId id)
        {
          GLWVBO* vbo = NULL;
          std::map<GLWVBOId, GLWVBO*>::iterator
              found = m_vbos.find(id);
          if (found != m_vbos.end())
          {
            vbo = found->second;
          }
          return vbo;
        }

        bool addVBO(GLWVBO* vbo, GLWVBOId& id)
        {
          id = 0;
          if (NULL != vbo)
          {
            m_vbos[vbo->m_buffer] = vbo;
            id                    = vbo->m_buffer;
          }
          return id != 0U;
        }

        void removeVBO(const GLWVBOId id)
        {
          std::map<GLWVBOId, GLWVBO*>::iterator found = m_vbos.find(id);
          if (found != m_vbos.end())
          {
            delete found->second;
            m_vbos.erase(found);
          }
        }

      private:
        std::map<GLWVBOId, GLWVBO*> m_vbos;
      };
    } // namespace buffers
  }   // namespace engine
} // namespace glw

#endif // GLWVBOMANAGER_H
