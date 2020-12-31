#pragma once

#include "layoutwidget.h"

namespace gui
{
  struct VLayoutWidget : LayoutWidget
  {
    VLayoutWidget(GUIManager* manager, nlohmann::json j);
    virtual void revalidate();
  };
} // namespace gui