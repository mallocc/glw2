#pragma once

#include "Color.h"
#include "widget.h"

#include <nlohmann/json.hpp>

namespace gui
{
  class GUIManager
  {
  public:
    glm::vec3 size;

    GUIManager(glm::vec3 size);

    void init();

    void draw();

    void onMouseEvent(MouseEvent mouseEvent);

    bool getColor(std::string colorName, Color& color);
    std::string getColorName(Color& color);
    bool getClass(std::string classname, nlohmann::json& j);

    Widget* createWidget(nlohmann::json& j);
    bool removeWidget(std::string id);
    Widget* findWidget(std::string id);

    int64_t getDoubleClickTime();

  private:
    std::map<std::string, Widget*> widgetMap;
    
    Widget* mainLayout    = nullptr;
    Widget* focusedWidget = nullptr;

    MouseEvent oldMouseEvent;
    MouseEvent focusedMouseEvent;

    nlohmann::json colors;
		nlohmann::json classes;
		nlohmann::json fonts;

		std::chrono::time_point<std::chrono::steady_clock> oldClickTime;
  };
} // namespace gui