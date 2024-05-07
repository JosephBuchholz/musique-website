#include "Credit.h"

#include "../../RenderMeasurement.h"

void Credit::Render(RenderData& renderData, Vec2<float> pagePosition, Vec2<float> pageDimensions, Vec2<float> offset) const
{
    if (pageNumber == 1)
    {
        float positionX = words.positionX + pagePosition.x;
        float positionY = (pageDimensions.y - words.positionY) + pagePosition.y; // get coordinate from top instead of bottom of page

        Paint paint = Paint();
        if (words.fontStyle == FontStyle::Italic)
            paint.isItalic = true;
        if (words.fontWeight == FontWeight::Bold)
            paint.isBold = true;

        paint.textSize = words.fontSize.size;

        if (words.justify == Justify::Center)
            paint.align = Paint::Align::Center;
        else if (words.justify == Justify::Left)
            paint.align = Paint::Align::Left;
        else if (words.justify == Justify::Right)
            paint.align = Paint::Align::Right;

        renderData.AddText(Text(words.text, { positionX, positionY }, paint));
    }
}

BoundingBox Credit::GetBoundingBox() const
{
    Paint paint;
    return words.GetBoundingBox(paint);
}

BoundingBox CreditWords::GetBoundingBox(Paint parentPaint) const
{
    TextualElement::ModifyPaint(parentPaint);

    BoundingBox bb = RenderMeasurement::GetTextBoundingBox(Text(text, 0.0f, 0.0f, parentPaint));
    bb.position.x += positionX;
    bb.position.y += positionY;

    return bb;
}