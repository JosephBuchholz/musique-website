#pragma once

#include <vector>

#include "SMuFLGlyph.h"
#include "../Collisions/Vec2.h"
#include "Paint.h"
#include "SpannableText.h"
#include "../Collisions/BoundingBox.h"

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
	void DrawRect(Vec2<float> position, Vec2<float> size, const Paint& paint = Paint());
	void DrawText(const std::string& text, Vec2<float> position, const Paint& paint = Paint());
	void DrawSpannableText(const SpannableText& spannableText);
	void DrawGlyph(SMuFLID glyph, Vec2<float> position, const Paint& paint = Paint());
	void DrawCubicCurve(Vec2<float> point1, Vec2<float> point2, Vec2<float> point3, Vec2<float> point4, const Paint& paint = Paint());

	BoundingBox MeasureText(const std::string& text, const Paint& paint = Paint());
	BoundingBox MeasureUTF16Text(const uint16_t* text, const Paint& paint = Paint());
	BoundingBox MeasureGlyph(SMuFLID glyph, const Paint& paint = Paint());

private:

	std::string EncodePaintObject(const Paint& paint);

private:

	void (*DrawLineCallback)(float startX, float startY, float endX, float endY, const char* paint);
	void (*DrawTextCallback)(const char* text, float posX, float posY, const char* paint);
	void (*DrawUTF16TextCallback)(const uint16_t* text, float posX, float posY, const char* paint);
	void (*DrawGlyphCallback)(uint16_t codePoint, float posX, float posY, const char* paint);
	void (*DrawCubicCurveCallback)(float posX1, float posY1, float posX2, float posY2, float posX3, float posY3, float posX4, float posY4, const char* paint);

	float* (*MeasureTextCallback)(const char* text, const char* paint);
	float* (*MeasureUTF16TextCallback)(const uint16_t* text, const char* paint);
	float* (*MeasureGlyphCallback)(uint16_t codePoint, const char* paint);

public:

	friend void AddFunctionsToCpp(int, int, int, int, int, int, int, int);
};