#pragma once
#include <iostream>

#include <nlohmann/json.hpp>

extern bool checkJSON(nlohmann::json j, std::string fieldName);

template <typename TYPE>
inline bool readJSON(nlohmann::json j, std::string fieldName, TYPE& field, bool debugPrint = false)
{
	if (checkJSON(j, fieldName))
	{
		field = j.at(fieldName).get<TYPE>();
		if(debugPrint)
			std::cout << "DEGUB read: " << fieldName << " = " << field << std::endl;
		return true;
	}
	else
	{
		if(debugPrint)
			std::cout << "Failed to read: " << fieldName << std::endl;
	}
	return false;
}

extern bool readJSONAsString(nlohmann::json j, std::string fieldName, std::string& field, bool debugPrint = false);

typedef std::function<void(std::string)> initcallback_t;
typedef std::function<nlohmann::json(std::string)> jsoncallback_t;
struct ManifestTuple
{
	std::string fieldValue;
	initcallback_t initCallback = nullptr;
	jsoncallback_t jsonCallback = nullptr;
};

extern void printGlob(nlohmann::json& j1);

extern void globProperties(nlohmann::json& j1, const nlohmann::json& j2);


extern void loadManifest(nlohmann::json &j, std::map<std::string, ManifestTuple> &manifestList, bool debugPrint = false, bool onlyOverrides = false);

extern nlohmann::json manifestToJson(std::map<std::string, ManifestTuple> manifestList);

extern std::string colorToString(float r, float g, float b);