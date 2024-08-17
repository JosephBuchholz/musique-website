#pragma once

#include "Rendering/SMuFLGlyph.h"
#include "Rendering/Text.h"
#include "Rendering/SpannableText.h"
#include "Collisions/BoundingBox.h"

/**
 * This class handles the measuring of different renderable elements, such as SMuFL glyphs or text.
 */
class RenderMeasurement
{
private:
    RenderMeasurement() {}

public:
    static float MeasureGlyph(const SMuFLGlyph& glyph);
    static float MeasureGlyph(SMuFLID codePoint);
    static BoundingBox GetTextBoundingBox(const Text& text);
    static BoundingBox GetGlyphBoundingBox(const SMuFLGlyph& glyph);
    static float MeasureSpannableText(const SpannableText& text);
};
