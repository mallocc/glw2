
#include "guimanager.h"

#include "Utilities.h"
#include <sstream>
#include <fstream>
#include "widget.h"
#include "layoutwidget.h"
#include "vlayoutwidget.h"
#include "hlayoutwidget.h"

using gui::GUIManager;
using gui::Widget;
using gui::Color;

void GUIManager::draw()
{

}

bool gui::GUIManager::getClass(std::string classname, nlohmann::json& j)
{
	std::cout << "Retrieving class: " << classname << std::endl;
	// Search in classes
	for (auto& c : classes)
	{
		std::string tmp;
		// Read for classname
		if (readJSON(c, "class-name", tmp))
		{
			// If we find the correct classname
			if (tmp == classname)
			{
				//std::cout << "Found class: " << classname << std::endl;
				// Store the recursive class properties as a glob
				nlohmann::json currentGlob;
				// If it inherits other classes
				nlohmann::json inheritClasses;
				if (readJSON(c, "inherits", inheritClasses))
				{
					//std::cout << "Class has inherits" << std::endl;
					// For each inherited classes
					for (auto& ic : inheritClasses)
					{
						// Retrieve inherit class as a normal class
						nlohmann::json k;
						if (getClass(ic, k))
						{
							// Glob the inherited class to the current class
							globProperties(currentGlob, k);
						}
					}
				}
				//std::cout << "Printing after inherits" << std::endl;
				//printGlob(currentGlob);

				//std::cout << "Class has properties" << std::endl;
				// Override from dervied
				globProperties(currentGlob, c);				
				globProperties(j, currentGlob);
				
				//printGlob(j);
				// We have found the root class, exit
				return true;
			}
		}
	}
	std::cout << "Unable to find class: " << classname << std::endl;
	return false;
}

std::vector<std::string> splitWithCharacters(const std::string& str, int splitLength) 
{
	int NumSubstrings = str.length() / splitLength;
	std::vector<std::string> ret;

	for (int i = 0; i < NumSubstrings; i++) 
	{
		ret.push_back(str.substr(i * splitLength, splitLength));
	}

	if (str.length() % splitLength != 0)
	{
		ret.push_back(str.substr(splitLength * NumSubstrings));
	}

	return ret;
}

Color hex2rgb(std::string hex) 
{
	Color color = { 0, 0, 0 };

	if (!hex.empty())
	{
		if (hex.at(0) == '#')
		{
			hex.erase(0, 1);
		}

		while (hex.length() != 6)
		{
			hex += "0";
		}

		std::vector<std::string> colori = splitWithCharacters(hex, 2);

		color.r = stoi(colori[0], nullptr, 16) / (float)255;
		color.g = stoi(colori[1], nullptr, 16) / (float)255;
		color.b = stoi(colori[2], nullptr, 16) / (float)255;
	}	

	return color;
}

bool gui::GUIManager::getColor(std::string colorName, Color& color)
{
	if (!colorName.empty())
	{
		if (colorName[0] == '#')
		{
			color = hex2rgb(colorName);
			return true;
		}
		else
		{
			for (auto& i : colors)
			{
				std::string tmp;
				if (readJSON(i, colorName, tmp))
				{
					color = hex2rgb(tmp);
					return true;
				}
			}
		}		
	}
	color = { 0,0,0 };
	std::cout << "Unable to find color: " << colorName << std::endl;
	return false;
}

std::string gui::GUIManager::getColorName(Color& color)
{
	std::string colorName = colorToString(color.r, color.g, color.b);
	for (const auto& j : colors.items())
		for (const auto& i : j.value().items())
			if (i.value() == colorName)
				return i.key();
	return colorName;
}

typedef std::map<std::string, std::string> localVariables_t;

bool isVariable(std::string key)
{
	if (key.size() > 0)
	{
		if (key[0] == '@' && key[key.size() - 1] == '@')
			return true;
	}
	return 0;
}

void replaceVariables(nlohmann::json &j, std::string key, std::string val)
{
	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		
		if (it.value().is_structured())
		{
			replaceVariables(it.value(), key, val);
		}
		else if (it.value() == key)
		{
			*it = val;
		}
	}
}

Widget* gui::GUIManager::createWidget(nlohmann::json& j)
{
	Widget* widget = nullptr;
	std::string tmp;

	if (readJSON(j, "class", tmp))
	{
		/*
			Look for defined variables with the @...@ syntax, 
			If we find one, add it to our map
		*/

		localVariables_t localVariables;

		for (auto& search: j.items())
		{
			if (isVariable(search.key()))
			{
				localVariables[search.key()] = search.value();
			}
		}

		std::cout << "Substituting for " << tmp << std::endl;
		nlohmann::json i;
		if (getClass(tmp, i))
		{
			globProperties(i, j);

			for (auto& lv : localVariables)
				replaceVariables(i, lv.first, lv.second);

			widget = createWidget(i);
		}
	}
	else if (readJSON(j, "widget", tmp))
	{	
    if (tmp == "hlayout")
		{
			widget = new gui::HLayoutWidget(this, j);
		}
		else if (tmp == "vlayout")
		{
			widget = new gui::VLayoutWidget(this, j);
		}
    else
		{
			std::cout << "Failed to find widget type field" << std::endl;
		}
	}
	else
	{
		std::cout << "Failed to find widget field" << std::endl;
	}

	if (widget != nullptr)
	{
		// Reset widget id to default if one already exists
		if (findWidget(widget->widgetId) != nullptr)
		{
			widget->widgetId = "";
		}

		// If default id, generate a unique id
		if (widget->widgetId == "")
		{
			uint32_t id = 0U;
			bool found = false;
			while (!found)
			{
				std::stringstream newId;
				newId << widget->type << id;
				if (findWidget(newId.str()) == nullptr)
				{
					widget->widgetId = newId.str();
					found = true;
				}
				id++;
			}			
		}

		// Add to map
		widgetMap[widget->widgetId] = widget;
	}

	return widget;
}

bool gui::GUIManager::removeWidget(std::string id)
{
	bool removed = false;
	std::map<std::string, Widget*>::iterator itor = widgetMap.find(id);
	if (itor != widgetMap.end())
	{
		delete itor->second;
		widgetMap.erase(itor);
		removed = true;
	}
	return removed;
}

Widget* gui::GUIManager::findWidget(std::string id)
{
	std::map<std::string, Widget*>::iterator itor = widgetMap.find(id);
	if  (itor != widgetMap.end())
		return itor->second;
	return nullptr;
}

void gatherInstances(std::string fieldName, const nlohmann::json& j, std::vector<nlohmann::json>& instances)
{
	for (const auto& i : j.items())
	{
		if (i.value().is_structured())
		{
			gatherInstances(fieldName, i.value(), instances);
		}
		else
		{
			//std::cout << i.key() << " : " << i.value() << std::endl;
			if (i.key() == fieldName)
			{
				instances.push_back({ {i.key(), i.value()} });
			}
		}
	}
}

void GUIManager::onMouseEvent(MouseEvent mouseEvent)
{
		// Look for a new focused widget in children
		if (focusedWidget == nullptr)
		{
			// Check for event
			if (mainLayout != nullptr)
				focusedWidget = mainLayout->onMouseEvent(mouseEvent, true, false);
		}
		else
		{
			focusedWidget->onMouseEvent(mouseEvent, true, true);
		}

		// Reset focused widget when mouse released
		if ((!mouseEvent.leftDown) && focusedWidget != nullptr)
		{	
			focusedWidget = nullptr;
		}

		oldMouseEvent = mouseEvent;
}

void GUIManager::init()
{
	if (mainLayout != nullptr)
	{
		delete mainLayout;
	}

	try
	{
		std::ifstream t("guiconfig.json");
		std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

		nlohmann::json j = nlohmann::json::parse(str);
		std::string tmp;
		bool success = true;

		success &= readJSON(j, "w", size.x);
		success &= readJSON(j, "h", size.y);

		if (success &= checkJSON(j, "colors"))
		{
			colors = j.at("colors");
		}		

		if (success &= checkJSON(j, "classes"))
		{
			classes = j.at("classes");
		}

		if (success &= checkJSON(j, "mainLayout"))
		{			
			mainLayout = createWidget(j.at("mainLayout"));
			mainLayout->revalidate();
		}
		else
		{
			std::cout << "There were errors in reading config for GUI" << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (nlohmann::json::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	/*catch (...)
	{
		std::cout << "unknown exception" << std::endl;
	}*/
}

int64_t GUIManager::getDoubleClickTime()
{
  return 250L;
}

GUIManager::GUIManager(glm::vec3 size)
	:
	mainLayout(nullptr),
  size(size),
	oldClickTime(),
	focusedWidget(nullptr),
	oldMouseEvent(),
	focusedMouseEvent()
{
}