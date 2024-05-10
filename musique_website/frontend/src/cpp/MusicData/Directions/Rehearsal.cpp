#include "Rehearsal.h"

#include "../../RenderMeasurement.h"

void Rehearsal::Render(RenderData& renderData, Vec2<float> measurePosition) const
{
    Paint paint = Paint();
    paint.textSize = 16.0f;
    /*if (fontStyle == FontStyle::Italic)
        paint.isItalic = true;
    if (fontWeight == FontWeight::Bold)
        paint.isBold = true;
    paint.textSize = fontSize.size;*/

    text.Render(renderData, measurePosition + position, Paint(color.color, paint));
    //renderData.AddText(Text(text.string, position.x + measurePosition.x, position.y + measurePosition.y, Paint(color.color, paint)));
    //renderData.AddDebugDot(positionX + measurePosition.x + offset.x, positionY + measurePosition.y + offset.y);
}

BoundingBox Rehearsal::GetBoundingBoxRelativeToParent() const
{
    Paint paint = Paint();
    /*if (fontStyle == FontStyle::Italic)
        paint.isItalic = true;
    if (fontWeight == FontWeight::Bold)
        paint.isBold = true;
    paint.textSize = fontSize.size;*/

    BoundingBox bb = text.GetBoundingBox(paint);

    bb.position += position;

    bb.position.x -= bb.size.x / 2.0f;
    bb.position.y += bb.size.y / 2.0f;

    return bb;
}