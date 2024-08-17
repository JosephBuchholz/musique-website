#pragma once

#include <string>

class JsonHelper
{
public:
    static void AddValueToJson(std::string& json, const std::string& key, const std::string& value);

    static void AddIntValueToJson(std::string& json, const std::string& key, uint32_t value);
    static void AddFloatValueToJson(std::string& json, const std::string& key, float value);

    static void AddBooleanValueToJson(std::string& json, const std::string& key, bool value);

    static void AddStringValueToJson(std::string& json, const std::string& key, const std::string& value);

};