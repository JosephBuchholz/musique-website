#include "RenderMeasurement.h"

#include "Rendering/Renderer.h"

float RenderMeasurement::MeasureGlyph(const SMuFLGlyph& glyph)
{
    return GetGlyphBoundingBox(glyph).size.x;
}

float RenderMeasurement::MeasureGlyph(SMuFLID codePoint)
{
    SMuFLGlyph glyph = SMuFLGlyph(codePoint, { 0.0f, 0.0f });
    return MeasureGlyph(glyph);
}

BoundingBox RenderMeasurement::GetTextBoundingBox(const Text& text)
{
    BoundingBox boundingBox = BoundingBox();

    Renderer& renderer = Renderer::GetInstance();

    boundingBox = renderer.MeasureText(text.text, text.paint);

    return boundingBox;
}

BoundingBox RenderMeasurement::GetGlyphBoundingBox(const SMuFLGlyph& glyph)
{
    BoundingBox boundingBox = BoundingBox();

    Renderer& renderer = Renderer::GetInstance();

    boundingBox = renderer.MeasureGlyph(glyph.codePoint, glyph.paint);

    return boundingBox;
}

float RenderMeasurement::MeasureSpannableText(const SpannableText& text)
{
    return 0.0f;
}