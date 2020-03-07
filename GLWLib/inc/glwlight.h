#pragma once

#include "glm.h"

namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      struct GLWLight
      {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 brightness_specscale_shinniness;
      };
    }
  }
}
