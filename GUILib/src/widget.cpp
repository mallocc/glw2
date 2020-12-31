
#include "widget.h"
#include "guimanager.h"
#include "Utilities.h"
#include "guidrawutilities.h"

using gui::Widget;

Widget* gui::Widget::onMouseEvent(MouseEvent mouseEvent, bool process, bool focus)
{
	bool handled = false;

	if (clickable)
	{
		if (onOverEvent(mouseEvent, process) || focus)
		{
			if (process)
			{
				onOver(manager, mouseEvent); 
			}

			if (onDownEvent(mouseEvent, process))
			{
				if (process)
				{
					onDown(manager, mouseEvent);
				}
				handled = true;
			}

			if (onRightClickEvent(mouseEvent, process))
			{
				if (process)
				{
					onRightClick(manager, mouseEvent);
				}
				handled = true;
			}

			if (onClickEvent(mouseEvent, process))
			{
				if (process)
				{
					onClick(manager, mouseEvent);
				}
				handled = true;
			} 
			
			if (onDragEvent(mouseEvent, process))
			{
				if (process)
				{
					onDrag(manager, mouseEvent);
				}
				handled = true;
			}

			if (onDoubleClickEvent(mouseEvent, process))
			{
				if (process)
				{
					onDoubleClick(manager, mouseEvent);
				}
				handled = true;
			}

			if (onReleaseEvent(mouseEvent, process))
			{
				if (process)
				{
					onRelease(manager, mouseEvent);
				}
				handled = true;
			}
			
			if (onUpEvent(mouseEvent, process))
			{
				if (process)
				{
					onUp(manager, mouseEvent);
				}
			}


			if (onMiddleClickEvent(mouseEvent, process))
			{
				if (process)
				{
					onMiddleClick(manager, mouseEvent);
				}
				handled = true;
			}
		}
		else if (onLeaveEvent(mouseEvent, process))
		{
			if (process)
			{
				onLeave(manager, mouseEvent);
			}
		}

		if (process)
		{
			oldMouseEvent = mouseEvent;
		}
	}

	return handled && !clickThrough ? this : nullptr;
}

void gui::Widget::draw(glm::vec3 translation)
{
  translation += pos;
  gui::drawRectangle(translation, size, color, 1.0, "", NULL);
}

void gui::Widget::revalidate()
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

bool gui::Widget::init(nlohmann::json j, bool ignoreType)
{
	config = j;
	bool success = true;
	if (checkJSON(j, "widget"))
	{
		addToManifestList(j,
			{
				{
					"widget",
					{"",
					[&](std::string value) { type = value; },
					[&](std::string fieldName) { return nlohmann::json({{fieldName, type }}); }}
				},
				{
					"color",
					{"#ffffff",
					[&](std::string value) { manager->getColor(value, color); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, manager->getColorName(color) }}); }}
				},
			
				{
					"x",
					{"0",
					[&](std::string value) { pos.x = std::atoi(value.c_str()); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, pos.x }}); }}
				},
				{
					"y",
					{"0",
					[&](std::string value) { pos.y = std::atoi(value.c_str()); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, pos.y }}); }}
				},
				{
					"w",
					{"0",
					[&](std::string value) { size.x = std::atoi(value.c_str()); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, size.z }}); }}
				},
				{
					"h",
					{"0",
					[&](std::string value) { size.y = std::atoi(value.c_str()); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, size.y }}); }}
				},

				{
					"text",
					{"---",
					[&](std::string value) { text = value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, text }}); }}
				},
				{
					"visible",
					{"true",
					[&](std::string value) { visible = "true" == value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, visible }}); }}
				},
				{
					"clickable",
					{"true",
					[&](std::string value) { clickable = "true" == value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, clickable }}); }}
				},
				{
					"click-through",
					{"false",
					[&](std::string value) { clickThrough = "true" == value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, clickThrough }}); }}
				},
				{
					"scaled",
					{"false",
					[&](std::string value) { scaled = "true" == value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, scaled }}); }}
				},
				{
					"id",
					{"",
					[&](std::string value) { widgetId = value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, widgetId }}); }}
				},

				{
					"mouse-over",
					{"{}",
					[&](std::string value) { nlohmann::json tmp = nlohmann::json::parse(value); mouseOverJson = config; globProperties(mouseOverJson, tmp); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, mouseOverJson.dump(4) }}); }}
				},
				{
					"mouse-down",
					{"{}",
					[&](std::string value) { nlohmann::json tmp = nlohmann::json::parse(value); mouseDownJson = mouseOverJson; globProperties(mouseDownJson, tmp); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, mouseDownJson.dump(4) }}); }}
				}, 
				{
					"on-toggled",
					{"{}",
					[&](std::string value) { nlohmann::json tmp = nlohmann::json::parse(value); onToggledJson = config; globProperties(onToggledJson, tmp); },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, onToggledJson.dump(4) }}); }}
				},

				{
					"toggled",
					{"false",
					[&](std::string value) { toggled = "true" == value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, toggled }}); }}
				},
				{
					"togglable",
					{"false",
					[&](std::string value) { togglable = "true" == value; },
					[&](std::string fieldName) { return nlohmann::json({{ fieldName, togglable }}); }}
				},
			}
		);
	}
	else
	{
		success = false;
		std::cout << "There were errors in reading config for Component" << std::endl;
	}
	return success;
}

gui::Widget::Widget(GUIManager* manager) : manager(manager)
{
	onUp = [](gui::GUIManager* manager, MouseEvent mouseEvent) {/* std::cout << "up" << std::endl;*/ };
	onDown = [](gui::GUIManager* manager, MouseEvent mouseEvent) {/* std::cout << "down" << std::endl;*/ };
	onOver = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /* std::cout << "over" << std::endl;*/ };
	onLeave = [](gui::GUIManager* manager, MouseEvent mouseEvent) {/* std::cout << "leave" << std::endl;*/ };
	onClick = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /*std::cout << "click" << std::endl;*/ };
	onRelease = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /*std::cout << "release" << std::endl;*/ };
	onDoubleClick = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /*std::cout << "double clicked" << std::endl;*/ };
	onDrag = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /*std::cout << "dragged" << std::endl; */};
	onMiddleClick = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /*std::cout << "dragged" << std::endl; */};
	onRightClick = [](gui::GUIManager* manager, MouseEvent mouseEvent) { /*std::cout << "dragged" << std::endl; */};
}

bool gui::Widget::onClickEvent(MouseEvent mouseEvent, bool process)
{
	// Handle clicked event
	if (mouseEvent.leftDown && !oldMouseEvent.leftDown)
	{
		if (process)
		{
			initialMouseEvent = mouseEvent;

			loadManifest(mouseDownJson, manifestList, 0, true);
			if (togglable)
			{
				toggled = !toggled;
				loadManifest(onToggledJson, manifestList, 0, true);
			}
		}
		return true;
	}
	return false;
}

bool gui::Widget::onRightClickEvent(MouseEvent mouseEvent, bool process)
{
	if (mouseEvent.rightDown && !oldMouseEvent.rightDown)
	{
		return true;
	}
	return false;
}

bool gui::Widget::onDoubleClickEvent(MouseEvent mouseEvent, bool process)
{
	// Handle click event
	if (mouseEvent.leftDown && !oldMouseEvent.leftDown)
	{
		// Handle double click event
		auto timeNow = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - oldClickTime).count();
		if (process)
		{
			oldClickTime = timeNow;
		}
		if (elapsed < manager->getDoubleClickTime())
		{
			// Do stuff for double click
			return true;
		}
	}
	return false;
}

bool gui::Widget::onDragEvent(MouseEvent mouseEvent, bool process)
{
	// Handle drag event
	if (mouseEvent.leftDown && oldMouseEvent.leftDown &&
		(mouseEvent.pos.x != oldMouseEvent.pos.x || mouseEvent.pos.x != oldMouseEvent.pos.x))
	{
		return true;
	}
	return false;
}

bool gui::Widget::onReleaseEvent(MouseEvent mouseEvent, bool process)
{
	// Handle released event
	if (!mouseEvent.leftDown && oldMouseEvent.leftDown)
	{
		if (process)
		{			
			if (toggled)
				loadManifest(onToggledJson, manifestList, 0, true);
			else
				loadManifest(mouseOverJson, manifestList, 0, true);
		}
		return true;
	}
	return false;
}

bool gui::Widget::onOverEvent(MouseEvent mouseEvent, bool process)
{
	int32_t tx = 0, ty = 0;
	getContextPosition(tx, ty);

	// if over component
	if (mouseEvent.pos.x >= tx && mouseEvent.pos.x <= tx + size.x &&
		mouseEvent.pos.x >= ty && mouseEvent.pos.x <= ty + size.y)
	{
		if (process)
		{
			if (!over)
			{
				loadManifest(mouseOverJson, manifestList, 0, true);
			}
			over = true;
		}
		return true;
	}
	return false;
}

bool gui::Widget::onLeaveEvent(MouseEvent mouseEvent, bool process)
{
	int32_t tx = 0, ty = 0;
	getContextPosition(tx, ty);

	// if over component
	if (mouseEvent.pos.x < tx || mouseEvent.pos.x > tx + size.x ||
		mouseEvent.pos.x < ty || mouseEvent.pos.x > ty + size.y)
	{
		if (process)
		{
			if (over)
			{
				if (toggled)
					loadManifest(onToggledJson, manifestList, 0, true);
				else
					loadManifest(config, manifestList, 0, true);
			}
			over = false;
		}
		return true;
	}
	return false;
}

bool gui::Widget::onDownEvent(MouseEvent mouseEvent, bool process)
{
	// Handle down event
	if (mouseEvent.leftDown)
	{
		if (process)
		{
			down = true;
		}
		return true;
	}
	return false;
}

bool gui::Widget::onUpEvent(MouseEvent mouseEvent, bool process)
{	
	// Handle up event
	if (!mouseEvent.leftDown)
	{
		if (process)
		{
			down = false;
		}
		return true;
	}
	return false;
}

bool gui::Widget::onMiddleClickEvent(MouseEvent mouseEvent, bool process)
{
	// Handle middle clicked event
	if (mouseEvent.middleDown && !oldMouseEvent.middleDown)
	{
		return true;
	}
	return false;
}

void gui::Widget::onIntent(std::string intentChoice)
{

}

void gui::Widget::getContextPosition(int32_t& tx, int32_t& ty)
{
	tx += pos.x;
	ty += pos.y;
	if (parent != nullptr)
		parent->getContextPosition(tx, ty);
}

void gui::Widget::toggleOn()
{
	toggled = true;
	if (over)
		loadManifest(mouseOverJson, manifestList, 0, true);
	else
		loadManifest(onToggledJson, manifestList, 0, true);
}

void gui::Widget::toggleOff()
{
	toggled = false;
	if (over)
		loadManifest(mouseOverJson, manifestList, 0, true);
	else
		loadManifest(config, manifestList, 0, true);
}

void gui::Widget::addToManifestList(nlohmann::json j, std::map<std::string, ManifestTuple> manifest)
{
	manifestList.insert(manifest.begin(), manifest.end());
	loadManifest(j, manifest);
}