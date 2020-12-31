#pragma once

#include "Color.h"
#include "glm.h"
#include <string>

namespace gui
{
  extern void drawRectangle(
      glm::vec3 translation,
      glm::vec3 scale,
      gui::Color color,
      float opacity,
      std::string blendMode,
      unsigned int glTextureId);

}
