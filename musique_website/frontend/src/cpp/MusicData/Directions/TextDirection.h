#pragma once

#include <functional>

#include "../BaseElements/TextElement.h"

class TextDirection : public VisibleElement
{
public:
    
    TextDirection()
        : BaseElement(BaseElement::ElementType::TextDirection) {}

    /**
     * Renders this object.
     *
     * @param[out] renderData The RenderData object to render to.
     * @param[in] measurePosition The position of the parent measure.
     */
    virtual void Render(RenderData& renderData, Vec2<float> measurePosition) const {}
    virtual void RenderDebug(RenderData& renderData, Vec2<float> measurePosition) const {}

    virtual BoundingBox GetBoundingBoxRelativeToParent() const { return {}; }

    virtual void Delete() override { onDeleteCallback(); }
    void SetOnDeleteCallback(std::function<void()> callback) { onDeleteCallback = callback; }

    /**
     * Updates the position and size of this object's bounding box.
     *
     * @param parentPosition The position of the parent.
     */
    //virtual void UpdateBoundingBox(const Vec2<float> &parentPosition);

    //virtual Vec2<float> GetDimensions() const;

    //void Move(Vec2<float> positionOffset, Vec2<float> sizeOffset = { 0.0f, 0.0f }, float rotationOffset = 0.0f) override;

    //void CalculatePositionAsPaged(const MusicDisplayConstants& displayConstants, float defaultX, float defaultY);

    //std::shared_ptr<SoundEvent> GetSoundEvent() const;

    //void OnTranspose(const TranspositionRequest& transposeRequest);

public:
    TextElement text;

    float beatPosition = 0.0f;

    enum class DirectionType
    {
        None = 0, Rehearsal
    } directionType = DirectionType::None;

    BaseElement* parent = nullptr;

    std::function<void()> onDeleteCallback = []() {};
};