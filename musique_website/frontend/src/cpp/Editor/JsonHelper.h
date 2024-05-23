#pragma once

#include "../libs/nlohmann/json.hpp"
using json = nlohmann::json;

class JsonHelper
{
public:
    static void AddBoolToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, bool value)
    {
        jsonObject[heading][id]["name"] = name;
        jsonObject[heading][id]["type"] = "bool";
        jsonObject[heading][id]["value"] = value;
    }

    static void AddTextToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, const std::string& value)
    {
        jsonObject[heading][id]["name"] = name;
        jsonObject[heading][id]["type"] = "text";
        jsonObject[heading][id]["value"] = value;
    }

    static void AddFloatToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, float value)
    {
        jsonObject[heading][id]["name"] = name;
        jsonObject[heading][id]["type"] = "float";
        jsonObject[heading][id]["value"] = value;
    }

    static void AddIntToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, int value)
    {
        jsonObject[heading][id]["name"] = name;
        jsonObject[heading][id]["type"] = "int";
        jsonObject[heading][id]["value"] = value;
    }

    static void AddColorToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, uint32_t value)
    {
        jsonObject[heading][id]["name"] = name;
        jsonObject[heading][id]["type"] = "color";
        jsonObject[heading][id]["value"] = value;
    }
};