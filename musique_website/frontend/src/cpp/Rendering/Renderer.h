#pragma once

#include "SMuFLGlyph.h"
#include "../Collisions/Vec2.h"
#include "Paint.h"

// singleton reference: https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern

class Renderer
{
public:
	static Renderer& GetInstance();

private:
	Renderer() {}

	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;

public:

	void DrawLine(Vec2<float> start, Vec2<float> end, const Paint& paint = Paint());
	void DrawText(const std::string& text, Vec2<float> position, const Paint& paint = Paint());
	void DrawGlyph(SMuFLID glyph, Vec2<float> position, const Paint& paint = Paint());

private:

	std::string EncodePaintObject(const Paint& paint);

private:

	void (*DrawLineCallback)(int startX, int startY, int endX, int endY, const char* paint);
	void (*DrawTextCallback)(const char* text, int posX, int posY, const char* paint);
	void (*DrawGlyphCallback)(uint16_t codePoint, int posX, int posY, const char* paint);

public:

	friend void AddFunctionsToCpp(int, int, int);
};