#include "TextElement.h"

#include "../../RenderMeasurement.h"

void TextElement::Render(RenderData& renderData, Vec2<float> parentPosition, Paint parentPaint) const
{
    uint32_t originalColor = parentPaint.color;
    TextualElement::ModifyPaint(parentPaint);

    if (originalColor == 0x000000FF)
        parentPaint.color = color;
    else
        parentPaint.color = originalColor;
    
#if DEBUG
    std::string t = text;
    if (t == "")
        t = "e";
    renderData.AddText(Text(t, position.x + parentPosition.x, position.y + parentPosition.y, parentPaint));
#else
    renderData.AddText(Text(text, position.x + parentPosition.x, position.y + parentPosition.y, parentPaint));
#endif
}

BoundingBox TextElement::GetBoundingBox(Paint parentPaint) const
{
    TextualElement::ModifyPaint(parentPaint);

    BoundingBox bb = RenderMeasurement::GetTextBoundingBox(Text(text, 0.0f, 0.0f, parentPaint));

    return bb;
}

Vec2<float> TextElement::GetDimensions(Paint parentPaint) const
{
    return GetBoundingBox(parentPaint).size;
}