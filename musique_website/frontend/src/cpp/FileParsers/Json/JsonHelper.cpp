#include "JsonHelper.h"

#include "../../Utils/Converters.h"

void JsonHelper::AddValueToJson(std::string& json, const std::string& key, const std::string& value)
{
    json += "\"";
    json += key;
    json += "\":";
    json += value;
}

void JsonHelper::AddIntValueToJson(std::string& json, const std::string& key, uint32_t value)
{
    AddValueToJson(json, key, ToString(value));
}

void JsonHelper::AddFloatValueToJson(std::string& json, const std::string& key, float value)
{
    AddValueToJson(json, key, ToString(value));
}

void JsonHelper::AddBooleanValueToJson(std::string& json, const std::string& key, bool value)
{
    AddValueToJson(json, key, (value ? "true" : "false"));
}

void JsonHelper::AddStringValueToJson(std::string& json, const std::string& key, const std::string& value)
{
    AddValueToJson(json, key, "\"" + value + "\"");
}