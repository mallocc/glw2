#pragma once

#include "widget.h"
#include <nlohmann/json.hpp>

namespace gui
{
	enum ALIGNMENT
	{
		ALIGN_NONE,
		ALIGN_TOP,
		ALIGN_BOTTOM,
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_START,
		ALIGN_END,
		ALIGN_CRUDE_CENTER,
		ALIGN_CENTER,
		ALIGN_SPACE_BETWEEN,
		ALIGN_SPACE_AROUND,
		ALIGN_CENTROID,
		ALIGN_MANUAL_SPACE,
		ALIGN_MANUAL_SPACE_CENTER,
		ALIGN_WEIGHTED,
		ALIGN_MID,
		ALIGN_NUMBER,
	};

	enum SIZING
	{
		SIZE_INHERIT,
		SIZE_NONE,
		SIZE_EXPAND,
		SIZE_EXPAND_WIDTH,
		SIZE_EXPAND_HEIGHT,
		SIZE_NUMBER
	};

	const static std::string SIZE_STRINGS[SIZE_NUMBER] = {
		"inherit",
		"none",
		"expand",
		"expand-width",
		"expand-height",
	};

	const static std::string ALIGN_STRINGS[ALIGN_NUMBER] = {
		"none",
		"top",
		"bottom",
		"left",
		"right",
		"start",
		"end",
		"crude-center",
		"center",
		"space-between",
		"space-around",
		"centroid",
		"manual-space",
		"manual-space-center",
		"weighted",
		"mid",
	};

	struct LayoutWidget : Widget
	{
		ALIGNMENT alignment = ALIGN_NONE;
		SIZING sizing = SIZE_INHERIT;
		float padding = 0.0f;
		float spacing = 0.0f;
		bool radio = false;
		std::vector<Widget*> children;
		Widget* lastLocalWidgetHandled = nullptr;

		void clearChildren();

		std::vector<Widget*> getVisibleChildren();

		void addChild(Widget* component);

		virtual Widget* onMouseEvent(MouseEvent mouseEvent, bool process, bool focus);

		virtual bool onLeaveEvent(MouseEvent mouseEvent, bool process);

		virtual void draw(glm::vec3 translate);

		virtual bool init(nlohmann::json j, bool ignoreType = false);

		virtual void revalidate();

		virtual void expand();

		virtual void onIntent(std::string intentChoice) override;

		LayoutWidget(GUIManager* manager, nlohmann::json j);
	};
}