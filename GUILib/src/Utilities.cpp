#include "Utilities.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include <fcntl.h>
#include <iostream>
#include <thread>

#include <sstream>
#include <iomanip>
#include <filesystem>
#include <fstream>

bool checkJSON(nlohmann::json j, std::string fieldName)
{
	//std::cout << "Looking for " << fieldName << std::endl;
	//std::cout << j.dump(4) << std::endl; 
	return j.contains(fieldName);
}

bool readJSONAsString(nlohmann::json j, std::string fieldName, std::string& field, bool debugPrint)
{
	if (debugPrint)
	{
		std::cout << "Attempting field name " << fieldName << std::endl;
		std::cout << "JSON " << j.dump(4) << std::endl;
	}
	if (checkJSON(j, fieldName))
	{
		if (j.at(fieldName).is_string())
			field = j.at(fieldName).get<std::string>();
		else
			field = j.at(fieldName).dump();

		if (debugPrint)
			std::cout << "JSON value found: '" << field << "'" << std::endl;
		return true;
	}
	else
	{
		if (debugPrint)
			std::cout << "JSON doesnt contain field name " << fieldName << std::endl;
	}
	return false;
}

void printGlob(nlohmann::json& j1)
{
	for (const auto& j : nlohmann::json::iterator_wrapper(j1))
	{
		std::cout << "Glob contains property " << j.key() << ": " << j.value() << std::endl;
	}
}

void globProperties(nlohmann::json& j1, const nlohmann::json& j2)
{
	for (const auto& j : nlohmann::json::iterator_wrapper(j2))
	{
		if (j.key() != "class")
			j1[j.key()] = j.value();
	}
}

void loadManifest(nlohmann::json &j, std::map<std::string, ManifestTuple> &manifestList, bool debugPrint, bool onlyOverrides)
{
	for (auto& i : manifestList)
	{
		ManifestTuple& manifest = i.second;
		if (manifest.initCallback != nullptr)
		{
			std::string tmp;
			if (!readJSONAsString(j, i.first, tmp, debugPrint))
			{
				tmp = manifest.fieldValue;
				if (onlyOverrides)
					continue;
			}

			manifest.initCallback(tmp);

			if (debugPrint)
				std::cout << "Loaded value: '" << tmp << "'" << std::endl;
		}
	}
}

nlohmann::json manifestToJson(std::map<std::string, ManifestTuple> manifestList)
{
	nlohmann::json j;
	for (auto& i : manifestList)
	{
		ManifestTuple& manifest = i.second;
		if (manifest.jsonCallback != nullptr)
		{
			nlohmann::json k = manifest.jsonCallback(i.first);
			std::string tmp;
			if (readJSONAsString(k, i.first, tmp))
			{
				if (manifest.fieldValue != tmp)
				{
					globProperties(j, k);
				}
			}			
		}
	}
	return j;
}

std::string colorToString(float r, float g, float b)
{
	int R = (r * 255);
	int G = (g * 255);
	int B = (b * 255);
	std::stringstream ss, rs, gs, bs;
	rs << std::setfill('0') << std::setw(2) << std::right << std::hex << R;
	gs << std::setfill('0') << std::setw(2) << std::right << std::hex << G;
	bs << std::setfill('0') << std::setw(2) << std::right << std::hex << B;
	ss << "#" << rs.str() << gs.str() << bs.str();
	return ss.str();
}
