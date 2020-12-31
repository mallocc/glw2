#pragma once

#include "glm.h"

namespace gui
{
  struct MouseEvent
  {
    glm::vec3 pos;
    bool leftDown   = false;
    bool rightDown  = false;
    bool middleDown = false;
  };

} // namespace gui