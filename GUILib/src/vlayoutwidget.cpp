#include "vlayoutwidget.h"
#include "guimanager.h"

#include <algorithm>

using gui::VLayoutWidget;

VLayoutWidget::VLayoutWidget(GUIManager* manager, nlohmann::json j) : gui::LayoutWidget(manager, j)
{
}

void VLayoutWidget::revalidate()
{
	LayoutWidget::revalidate();

	std::vector<Widget*> visibleChildren = getVisibleChildren();

	if (visibleChildren.size() > 0)
	{
		float midx = size.x / 2.0f;
		float midy = size.y / 2.0f;
		float sizeM = 0.0f;
		float maxx = 0.0f;
		for (Widget* widget : visibleChildren)
		{
			sizeM += widget->size.y;
			maxx = std::max(maxx, (float)widget->size.x);
		}

		switch (alignment)
		{
		case ALIGN_MID:
		{
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midy - visibleChildren[i]->size.x / 2.0f;
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
			float start = padding;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y + spacing;
			}
			break;
		}
		case ALIGN_BOTTOM:
		{
			float start = size.y - sizeM - padding;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y + spacing;
			}
			break;
		}
		case ALIGN_LEFT:
		{
			float start = midy - sizeM / 2.0f;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = padding;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y;
			}
			break;
		}
		case ALIGN_RIGHT:
		{
			float start = midy - sizeM / 2.0f;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = size.x - visibleChildren[i]->size.x - padding;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y;
			}
			break;
		}
		case ALIGN_CENTER:
		{
			float start = midy - sizeM / 2.0f;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y;
			}
			break;
		}
		case ALIGN_START:
		{
			float start = 0.0f;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.y = start;
				visibleChildren[i]->pos.x = 0.0f;
				start += visibleChildren[i]->size.y;
			}
			break;
		}
		case ALIGN_END:
		{
			float leftOver = size.y - sizeM;
			float start = leftOver;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = size.x - visibleChildren[i]->size.x;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y;
			}
			break;
		}
		case ALIGN_SPACE_BETWEEN:
		{
			float leftOver = size.y - sizeM;
			float dy = leftOver / (visibleChildren.size() - 1);
			float start = 0.0f;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y + dy;
			}
			break;
		}
		case ALIGN_SPACE_AROUND:
		{
			float leftOver = size.y - sizeM - padding * 2.0f;
			float dy = leftOver / (visibleChildren.size() - 1);
			float start = padding;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start;
				start += visibleChildren[i]->size.y + dy;
			}
			break;
		}
		case ALIGN_MANUAL_SPACE:
		{
			float start = padding;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start;
				start += spacing;
			}
			break;
		}
		case ALIGN_MANUAL_SPACE_CENTER:
		{
			float start = padding;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start - visibleChildren[i]->size.y / 2.0f;
				start += spacing;
			}
			break;
		}
		case ALIGN_WEIGHTED:
		{
			float start = 0.0f;
			for (int i = 0; i < visibleChildren.size(); ++i)
			{
				float dy = size.y * visibleChildren[i]->weight;
				visibleChildren[i]->pos.x = midx - visibleChildren[i]->size.x / 2.0f;
				visibleChildren[i]->pos.y = start + dy / 2.0f - visibleChildren[i]->size.y / 2.0f;
				start += dy;
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

