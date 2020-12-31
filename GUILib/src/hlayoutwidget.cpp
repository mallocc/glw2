#include "hlayoutwidget.h"
#include "guimanager.h"

using gui::HLayoutWidget;

HLayoutWidget::HLayoutWidget(GUIManager* manager, nlohmann::json j) : gui::LayoutWidget(manager, j)
{
}

void HLayoutWidget::revalidate()
{
  LayoutWidget::revalidate();

  std::vector<Widget*> visibleChildren = getVisibleChildren();
  if (visibleChildren.size() > 0)
  {
    float midx  = size.x / 2.0f;
    float midy  = size.y / 2.0f;
    float sizeM = 0.0f;
    float maxy  = 0.0f;
    for (Widget* widget : visibleChildren)
    {
      sizeM += widget->size.x;
      maxy = std::max(maxy, widget->size.y);
    }

    switch (alignment)
    {
    case ALIGN_MID:
    {
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->size.y = midy - visibleChildren[i]->size.y / 2.0f;
      }
      break;
    }
    case ALIGN_CENTROID:
    {
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
      }
      break;
    }
    case ALIGN_TOP:
    {
      float start = midx - sizeM / 2.0f;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = padding;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_BOTTOM:
    {
      float start = midx - sizeM / 2.0f - padding;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = size.y - visibleChildren[i]->size.y;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_LEFT:
    {
      float start = padding;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_RIGHT:
    {
      float start = size.x - sizeM - padding;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_CENTER:
    {
      float start = midx - sizeM / 2.0f;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_START:
    {
      float start = 0.0f;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = 0.0f;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_END:
    {
      float leftOver = size.x - sizeM;
      float start    = leftOver;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = size.y - visibleChildren[i]->size.y;
        start += visibleChildren[i]->size.x;
      }
      break;
    }
    case ALIGN_SPACE_BETWEEN:
    {
      float leftOver = size.x - sizeM;
      float dx       = leftOver / (visibleChildren.size() - 1);
      float start    = 0.0f;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        start += visibleChildren[i]->size.x + dx;
      }
      break;
    }
    case ALIGN_SPACE_AROUND:
    {
      float leftOver = size.x - sizeM - padding * 2.0f;
      float dx       = leftOver / (visibleChildren.size() - 1);
      float start    = padding;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        visibleChildren[i]->pos.x = start;
        start += visibleChildren[i]->size.x + dx;
      }
      break;
    }
    case ALIGN_MANUAL_SPACE:
    {
      float start = padding;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        start += spacing;
      }
      break;
    }
    case ALIGN_MANUAL_SPACE_CENTER:
    {
      float start = padding;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        visibleChildren[i]->pos.x = start - visibleChildren[i]->size.x / 2.0f;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        start += spacing;
      }
      break;
    }
    case ALIGN_WEIGHTED:
    {
      float start = 0.0f;
      for (int i = 0; i < visibleChildren.size(); ++i)
      {
        float dx                  = size.x * visibleChildren[i]->weight;
        visibleChildren[i]->pos.y = midy - visibleChildren[i]->size.y / 2.0f;
        visibleChildren[i]->pos.x = start + dx / 2.0f - visibleChildren[i]->size.x / 2.0f;
        start += dx;
      }
      break;
    }
    case ALIGN_NONE:
    default:
      break;
    }
  }

  expand();
}
