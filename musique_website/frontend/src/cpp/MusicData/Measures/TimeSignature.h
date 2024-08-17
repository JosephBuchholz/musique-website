/**
 * This file contains the definition of the `TimeSignature` class.
 */

#ifndef MUSIQUE_TIMESIGNATURE_H
#define MUSIQUE_TIMESIGNATURE_H

#include "../../Rendering/RenderData.h"
#include "../BaseElements/VisibleElement.h"
#include "../../Collisions/Vec2.h"
#include "../../MusicDisplayConstants.h"

/**
 * This class represents a musical time signature.
 */
class TimeSignature : public VisibleElement
{
    friend class Measure;

public:
    
    TimeSignature()
        : BaseElement(BaseElement::ElementType::TimeSignature) {}

    /**
     * Renders this class.
     *
     * @param[out] renderData The `RenderData` object to render to.
     * @param[in] showTimeSignature Whether the time signature should be shown.
     * @param[in] positionX The x position of the time signature.
     * @param[in] measurePositionY The y position of the measure.
     * @param[in] lineSpacing The space between staff lines.
     * @param[in] lines The number of staff lines.
     * @param[in] offsetX The offset in the x direction.
     * @param[in] offsetY The offset in the y direction.
     */
    void Render(RenderData& renderData, bool showTimeSignature, float positionX, float measurePositionY, float lineSpacing, int lines, float offsetX = 0.0f, float offsetY = 0.0f) const;

    /**
     * Renders this class.
     *
     * @param[out] renderData The `RenderData` object to render to.
     * @param[in] parentPosition The position of the time signature's parent (position of time signature originates from the left for x and the center for y).
     * @param[in] height The height of the time signature (width will be scaled accordingly).
     */
    void Render(RenderData& renderData, Vec2<float> parentPosition, float height) const;

    float GetWidth(float height) const;

    static SMuFLID GetTimeSignatureSMuFLID(int time);

protected:
    void CalculatePositionAsPaged(const MusicDisplayConstants& displayConstants);

public:
    enum class TimeSignatureDisplayType {
        None = 0, Standard, Common, Cut
    };

    static TimeSignatureDisplayType GetDisplayTypeFromString(const std::string& string) {
        if (string == "common") {
            return TimeSignatureDisplayType::Common;
        } else if (string == "cut") {
            return TimeSignatureDisplayType::Cut;
        }
        return TimeSignatureDisplayType::Standard;
    }

    TimeSignatureDisplayType displayType = TimeSignatureDisplayType::Standard;

    int notes = 4;
    int noteType = 4;

    // x is relative to the start of the measure
    // y is relative to the top staff line
    Vec2<float> position = { 0.0f, 0.0f };
};

#endif // MUSIQUE_TIMESIGNATURE_H