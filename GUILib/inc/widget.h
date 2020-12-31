#pragma once

#include "Color.h"
#include "Utilities.h"
#include "glm.h"
#include "mouseevent.h"
#include <chrono>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>

namespace gui
{
  class GUIManager;

  typedef std::function<void(gui::GUIManager*, MouseEvent)> callback_t;

  struct Widget
  {
    GUIManager* manager;

    std::string type;

    glm::vec3 pos;
    glm::vec3 size;

    std::string text;

    Widget* parent;

    std::string widgetId = "widget";

    callback_t onClick;
    callback_t onRightClick;
    callback_t onRelease;
    callback_t onDoubleClick;
    callback_t onOver;
    callback_t onDown;
    callback_t onUp;
    callback_t onLeave;
    callback_t onDrag;
    callback_t onMiddleClick;

    nlohmann::json config;
    nlohmann::json mouseOverJson;
    nlohmann::json mouseDownJson;
    nlohmann::json onToggledJson;

    std::map<std::string, ManifestTuple> manifestList;

    MouseEvent oldMouseEvent;
    MouseEvent initialMouseEvent;

    Color color = {1, 1, 1};

    bool down = false;
    bool over = false;

    bool visible      = true;
    bool clickable    = true;
    bool clickThrough = true;

    bool scaled = false;

    bool toggled   = false;
    bool togglable = false;

    float weight = 0.0f;

    std::chrono::time_point<std::chrono::steady_clock> oldClickTime;

    Widget(GUIManager* gui);

    virtual Widget* onMouseEvent(MouseEvent mouseEvent, bool process = true, bool focus = false);

    virtual void draw(glm::vec3 translate);
    virtual void revalidate();
    virtual bool init(nlohmann::json j, bool ignoreType);
    virtual void addToManifestList(nlohmann::json j, std::map<std::string, ManifestTuple> manifest);

    virtual bool onClickEvent(MouseEvent mouseEvent, bool process);
    virtual bool onRightClickEvent(MouseEvent mouseEvent, bool process);
    virtual bool onDoubleClickEvent(MouseEvent mouseEvent, bool process);
    virtual bool onDragEvent(MouseEvent mouseEvent, bool process);
    virtual bool onReleaseEvent(MouseEvent mouseEvent, bool process);
    virtual bool onOverEvent(MouseEvent mouseEvent, bool process);
    virtual bool onLeaveEvent(MouseEvent mouseEvent, bool process);
    virtual bool onDownEvent(MouseEvent mouseEvent, bool process);
    virtual bool onUpEvent(MouseEvent mouseEvent, bool process);
    virtual bool onMiddleClickEvent(MouseEvent mouseEvent, bool process);

    virtual void onIntent(std::string intentChoice);

    void getContextPosition(int32_t& tx, int32_t& ty);

    void toggleOn();
    void toggleOff();
  };
} // namespace gui