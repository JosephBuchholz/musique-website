/**
 * This file contains the definition of the `Rehearsal` class.
 */

#pragma once

#include "TextDirection.h"

/**
 * This class represents a rehearsal mark.
 */
class Rehearsal : public TextDirection
{
public:
    Rehearsal()
        : BaseElement(BaseElement::ElementType::TextDirection) { directionType = TextDirection::DirectionType::Rehearsal; }

    void Render(RenderData& renderData, Vec2<float> measurePosition) const override;

    BoundingBox GetBoundingBoxRelativeToParent() const override;

public:

    enum class EnclosureShape {
        None = 0, Rectangle, Square, Oval, Circle, Bracket, InvertedBracket, Triangle, Diamond, Pentagon, Hexagon, Heptagon, Octagon, Nonagon, Decagon
    } enclosureShape = EnclosureShape::None;
};