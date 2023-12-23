#include "Renderer.h"

Renderer& Renderer::GetInstance()
{
    static Renderer instance;
    return instance;
}

void Renderer::DrawLine(Vec2<float> start, Vec2<float> end)
{
    DrawLineCallback((int)start.x, (int)start.y, (int)end.x, (int)end.y);
}

void Renderer::DrawText(const std::string& text, Vec2<float> position)
{
    DrawTextCallback(text.c_str(), (int)position.x, (int)position.y);
}

void Renderer::DrawGlyph(SMuFLID glyph, Vec2<float> position)
{
    DrawGlyphCallback((uint16_t)glyph, (int)position.x, (int)position.y);
}