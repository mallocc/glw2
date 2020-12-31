#include "layoutwidget.h"
#include "Utilities.h"
#include "guimanager.h"
#include <algorithm>

using gui::GUIManager;
using gui::LayoutWidget;
using gui::Widget;

LayoutWidget::LayoutWidget(GUIManager* manager, nlohmann::json j) : Widget(manager)
{
  init(j);
}

void gui::LayoutWidget::clearChildren()
{
  for (Widget* c : children)
  {
    if (c != nullptr)
    {
      manager->removeWidget(c->widgetId);
    }
  }
  children.clear();
}

std::vector<Widget*> gui::LayoutWidget::getVisibleChildren()
{
  std::vector<Widget*> visibleChildren;
  for (Widget* c : children)
    if (c->visible)
      visibleChildren.push_back(c);
  return visibleChildren;
}

void gui::LayoutWidget::addChild(Widget* widget)
{
  if (widget != nullptr)
  {
    widget->parent = this;
    children.push_back(widget);
  }
}

Widget* gui::LayoutWidget::onMouseEvent(MouseEvent mouseEvent, bool process, bool focus)
{
  std::vector<Widget*> visibleChildren = getVisibleChildren();

  Widget* oldLastLocalWidgetHandled = lastLocalWidgetHandled;
  lastLocalWidgetHandled            = nullptr;
  // Handle children widgets
  if (oldLastLocalWidgetHandled != this)
  {
    for (Widget* widget : visibleChildren)
    {
      Widget* handledWidget = widget->onMouseEvent(mouseEvent, process, focus);
      if (lastLocalWidgetHandled == nullptr)
      {
        lastLocalWidgetHandled = handledWidget;
        if (lastLocalWidgetHandled != nullptr)
        {
          return lastLocalWidgetHandled;
        }
      }
    }
  }
  if (lastLocalWidgetHandled == nullptr)
  {
    lastLocalWidgetHandled = Widget::onMouseEvent(mouseEvent, process, focus);
  }
  return lastLocalWidgetHandled;
}

bool gui::LayoutWidget::onLeaveEvent(MouseEvent mouseEvent, bool process)
{
  Widget::onLeaveEvent(mouseEvent, process);
  std::vector<Widget*> visibleChildren = getVisibleChildren();
  for (Widget* widget : visibleChildren)
  {
    widget->onLeaveEvent(mouseEvent, process);
  }
  return true;
}

void LayoutWidget::draw(glm::vec3 translate)
{
  Widget::draw(translate);
  translate += pos;

  std::vector<Widget*> visibleChildren = getVisibleChildren();
  for (int i = visibleChildren.size() - 1; i >= 0; --i)
  {
    visibleChildren[i]->draw(translate);
  }

  bool widgetHandledDown = false;
  for (Widget* widget : visibleChildren)
    if (widget->down)
      widgetHandledDown = true;
}

bool gui::LayoutWidget::init(nlohmann::json j, bool ignoreType)
{
  std::string tmp;

  if (Widget::init(j, ignoreType))
  {
    if (type.find("layout"))
    {
      if (checkJSON(j, "children"))
      {
        nlohmann::json jc = j.at("children");
        for (auto& i : jc)
        {
          addChild(manager->createWidget(i));
        }
      }

      addToManifestList(j,
                        {
                            {"padding",
                             {"0",
                              [&](std::string value) { padding = std::atoi(value.c_str()); },
                              [&](std::string fieldName) { return nlohmann::json({{fieldName, padding}}); }}},

                            {"align",
                             {"NONE",
                              [&](std::string value) { for (int i = 0; i < ALIGN_NUMBER; ++i)	if (ALIGN_STRINGS[i] == value) alignment = (ALIGNMENT)i; },
                              [&](std::string fieldName) { return nlohmann::json({{fieldName, ALIGN_STRINGS[alignment]}}); }}},
                            {"size",
                             {"INHERIT",
                              [&](std::string value) { for (int i = 0; i < SIZE_NUMBER; ++i) if (SIZE_STRINGS[i] == value) sizing = (SIZING)i; },
                              [&](std::string fieldName) { return nlohmann::json({{fieldName, SIZE_STRINGS[sizing]}}); }}},
                            {"spacing",
                             {"0",
                              [&](std::string value) { spacing = std::atoi(value.c_str()); },
                              [&](std::string fieldName) { return nlohmann::json({{fieldName, spacing}}); }}},
                            {"radio",
                             {"false",
                              [&](std::string value) { radio = "true" == value; },
                              [&](std::string fieldName) { return nlohmann::json({{fieldName, radio}}); }}},
                        });
    }
  }

  return true;
}

void gui::LayoutWidget::revalidate()
{
  if (sizing == SIZE_INHERIT)
  {
    if (size.length == 0)
    {
      if (parent != nullptr)
      {
        size = parent->size;
      }
      else
      {
        size = manager->size;
      }
    }
  }

  std::vector<Widget*> visibleChildren = getVisibleChildren();
  for (Widget* widget : visibleChildren)
  {
    widget->revalidate();
  }

  expand();
}

void gui::LayoutWidget::expand()
{
  std::vector<Widget*> visibleChildren = getVisibleChildren();
  switch (sizing)
  {
  case SIZE_EXPAND:
  {
    float maxx = 0.0f;
    float maxy = 0.0f;
    float minx = manager->size.x;
    float miny = manager->size.y;
    for (Widget* widget : visibleChildren)
    {
      maxx = std::max(maxx, widget->pos.x + widget->size.x);
      maxy = std::max(maxy, widget->pos.y + widget->size.y);
      minx = std::min(minx, widget->pos.x);
      miny = std::min(miny, widget->pos.y);
    }
    size.x = maxx - minx;
    size.y = maxy - miny;
    break;
  }
  case SIZE_EXPAND_WIDTH:
  {
    float maxx = 0.0f;
    float minx = manager->size.x;
    for (Widget* widget : visibleChildren)
    {
      maxx = std::max(maxx, widget->pos.x + widget->size.x);
      minx = std::min(minx, widget->pos.x);
    }

    size.x = maxx - minx;
    if (parent != nullptr)
    {
      size.y = parent->size.y;
    }
    else
    {
      size.y = manager->size.y;
    }
    break;
  }
  case SIZE_EXPAND_HEIGHT:
  {
    float maxy = 0.0f;
    float miny = manager->size.y;
    for (Widget* widget : visibleChildren)
    {
      maxy = std::max(maxy, widget->pos.y + widget->size.y);
      miny = std::min(miny, widget->pos.y);
    }

    if (parent != nullptr)
    {
      size.x = parent->size.x;
    }
    else
    {
      size.x = manager->size.x;
    }
    size.y = maxy - miny;
    break;
  }
  }
}

void gui::LayoutWidget::onIntent(std::string intentChoice)
{

}
