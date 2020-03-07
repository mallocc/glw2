#pragma once

#include "glwshadervariablehandle.h"

using glw::engine::glsl::GLWShaderVariableHandle;

namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      struct GLWShaderHandle
      {
        GLWShaderVariableHandle * textureHandle;
        GLWShaderVariableHandle * modelMatHandle;
        GLWShaderVariableHandle * viewMatHandle;
        GLWShaderVariableHandle * projMatHandle;
      };
    }
  } 
}

