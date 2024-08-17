#include "Articulation.h"

void Articulation::RenderDebug(RenderData& renderData, Vec2<float> notePosition) const
{
    BoundingBox bb = GetBoundingBox();

    bb.position += notePosition;

    bb.Render(renderData, 0xFFFF00FF);
}
