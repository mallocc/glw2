#pragma once

#include "layoutwidget.h"

namespace gui
{
  struct HLayoutWidget : LayoutWidget
  {
    HLayoutWidget(GUIManager* manager, nlohmann::json j);
    virtual void revalidate();
  };
} // namespace gui