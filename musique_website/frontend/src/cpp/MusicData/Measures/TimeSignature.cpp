#include "TimeSignature.h"

#include "../../RenderMeasurement.h"

void TimeSignature::Render(RenderData& renderData, bool showTimeSignature, float positionX, float measurePositionY, float lineSpacing, int lines, float offsetX, float offsetY) const
{
    if (printObject && showTimeSignature)
    {
        int spaces = lines - 1;
        float staffMiddle = ((float)spaces/2.0f) * lineSpacing;
        float timeDigitHeight = 10.0f;


        switch (displayType)
        {
            case TimeSignatureDisplayType::Common:
            {
                renderData.AddGlyph(SMuFLGlyph(SMuFLID::timeSigCommon, positionX + offsetX, staffMiddle + measurePositionY + offsetY, Paint(color.color)));
                break;
            }
            case TimeSignatureDisplayType::Cut:
            {
                renderData.AddGlyph(SMuFLGlyph(SMuFLID::timeSigCutCommon, positionX + offsetX, staffMiddle + measurePositionY + offsetY, Paint(color.color)));
                break;
            }
            case TimeSignatureDisplayType::Standard:
            {
                float positionY = (staffMiddle - timeDigitHeight) + measurePositionY + offsetY;
                renderData.AddGlyph(SMuFLGlyph(GetTimeSignatureSMuFLID(notes), positionX + offsetX, positionY,Paint(color.color)));
                positionY += 20.0f;
                renderData.AddGlyph(SMuFLGlyph(GetTimeSignatureSMuFLID(noteType), positionX + offsetX, positionY,Paint(color.color)));
                break;
            }
            default:
            {
                LOGW("Time signature display type is not handled");
                break;
            }
        }
    }
}

void TimeSignature::Render(RenderData& renderData, Vec2<float> parentPosition, float height) const
{
    if (printObject)
    {
        const float timeDigitHeightDefault = 20.0f;

        Paint paint = Paint(selectedColor.color);
        paint.glyphSizeFactor = height / (timeDigitHeightDefault * 2.0f);

        switch (displayType)
        {
            case TimeSignatureDisplayType::Common:
            {
                renderData.AddGlyph(SMuFLGlyph(SMuFLID::timeSigCommon, parentPosition + position, paint));
                break;
            }
            case TimeSignatureDisplayType::Cut:
            {
                renderData.AddGlyph(SMuFLGlyph(SMuFLID::timeSigCutCommon, parentPosition + position, paint));
                break;
            }
            case TimeSignatureDisplayType::Standard:
            {
                float timeDigitHeight = height / 2.0f;

                renderData.AddGlyph(SMuFLGlyph(GetTimeSignatureSMuFLID(notes), position + Vec2<float>{ parentPosition.x, parentPosition.y - (timeDigitHeight / 2.0f) }, paint));
                renderData.AddGlyph(SMuFLGlyph(GetTimeSignatureSMuFLID(noteType), position + Vec2<float>{ parentPosition.x, parentPosition.y + (timeDigitHeight / 2.0f) }, paint));
                break;
            }
            default:
            {
                LOGW_TAG("TimeSignature", "Time signature display type is not handled");
                break;
            }
        }
    }
}

float TimeSignature::GetWidth(float height) const
{
    const float timeDigitHeightDefault = 20.0f;

    Paint paint = Paint(color.color);
    paint.glyphSizeFactor = height / (timeDigitHeightDefault * 2.0f);

    switch (displayType)
    {
        case TimeSignatureDisplayType::Standard:
        {
            float timeDigitHeight = height / 2.0f;
            BoundingBox bb = RenderMeasurement::GetGlyphBoundingBox(SMuFLGlyph(GetTimeSignatureSMuFLID(noteType), { 0.0f, 0.0f }, paint));
            return bb.size.x;
        }
        default:
        {
            LOGW_TAG("TimeSignature", "Time signature display type is not handled");
            break;
        }
    }
}

void TimeSignature::CalculatePositionAsPaged(const MusicDisplayConstants& displayConstants)
{

}

SMuFLID TimeSignature::GetTimeSignatureSMuFLID(int time)
{
    switch (time)
    {
        case 0: return SMuFLID::timeSig0;
        case 1: return SMuFLID::timeSig1;
        case 2: return SMuFLID::timeSig2;
        case 3: return SMuFLID::timeSig3;
        case 4: return SMuFLID::timeSig4;
        case 5: return SMuFLID::timeSig5;
        case 6: return SMuFLID::timeSig6;
        case 7: return SMuFLID::timeSig7;
        case 8: return SMuFLID::timeSig8;
        case 9: return SMuFLID::timeSig9;
    }

    return SMuFLID::None;
}
