#include "Renderer.h"

#include "../Utils/Converters.h"

Renderer& Renderer::GetInstance()
{
    static Renderer instance;
    return instance;
}

void AddIntValueToJson(std::string& json, const std::string& key, uint32_t value)
{
    json += "\"";
    json += key;
    json += "\":";
    json += ToString(value);
}

void AddFloatValueToJson(std::string& json, const std::string& key, float value)
{
    json += "\"";
    json += key;
    json += "\":";
    json += ToString(value);
}

void Renderer::DrawLine(Vec2<float> start, Vec2<float> end, const Paint& paint)
{
    DrawLineCallback((int)start.x, (int)start.y, (int)end.x, (int)end.y, EncodePaintObject(paint).c_str());
}

void Renderer::DrawText(const std::string& text, Vec2<float> position, const Paint& paint)
{
    DrawTextCallback(text.c_str(), (int)position.x, (int)position.y, EncodePaintObject(paint).c_str());
}

void Renderer::DrawGlyph(SMuFLID glyph, Vec2<float> position, const Paint& paint)
{
    DrawGlyphCallback((uint16_t)glyph, (int)position.x, (int)position.y, EncodePaintObject(paint).c_str());
}

std::string Renderer::EncodePaintObject(const Paint& paint)
{
	std::string paintString;

	paintString += "{";

	AddIntValueToJson(paintString, "color", paint.color);
	paintString += ",";
	AddFloatValueToJson(paintString, "strokeWidth", paint.strokeWidth);
	paintString += ",";
	AddFloatValueToJson(paintString, "textSize", paint.textSize);

	paintString += "}";

    return paintString;
}
