#include "RenderMeasurement.h"


float RenderMeasurement::MeasureGlyph(const SMuFLGlyph& glyph)
{
    return 1.0f;
}

float RenderMeasurement::MeasureGlyph(SMuFLID codePoint)
{
    SMuFLGlyph glyph = SMuFLGlyph(codePoint, { 0.0f, 0.0f });
    return MeasureGlyph(glyph);
}

BoundingBox RenderMeasurement::GetTextBoundingBox(const Text& text)
{
    BoundingBox boundingBox = BoundingBox();

    return boundingBox;
}

BoundingBox RenderMeasurement::GetGlyphBoundingBox(const SMuFLGlyph& glyph)
{
    BoundingBox boundingBox = BoundingBox();

    return boundingBox;
}

float RenderMeasurement::MeasureSpannableText(const SpannableText& text)
{
    return 0.0f;
}