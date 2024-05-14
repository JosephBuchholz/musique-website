#include "CSChord.h"

void CSChord::Render(RenderData& renderData, const Settings& settings, Vec2<float> measurePosition) const
{
    //LOGI_TAG("CSChord.cpp", "Rendering chord!!!!: mpos: %s | pos: %s", measurePosition.GetPrintableString().c_str(), position.GetPrintableString().c_str());
    chordSymbol.Render(renderData, settings, measurePosition + position);
}

void CSChord::Init(Vec2<float> pos)
{
    position = pos;

    chordSymbol.justify = Justify::Left;
}

BoundingBox CSChord::GetBoundingBox() const
{
    BoundingBox bb = chordSymbol.GetBoundingBoxRelativeToParent();
    bb.position += position;
    return bb;
}

Vec2<float> CSChord::GetDimensions() const
{
    return GetBoundingBox().size;
}

void CSChord::Delete()
{
    onDeleteCallback();
}
