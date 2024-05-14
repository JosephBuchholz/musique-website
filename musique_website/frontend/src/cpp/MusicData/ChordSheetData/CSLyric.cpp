#include "CSLyric.h"

void CSLyric::Init(const Settings& settings, Vec2<float> pos)
{
    lyricText.fontSize = settings.displayConstants.lyricFontSize;
    lyricText.align = TextAlign::Left;

    position = pos;
}

void CSLyric::Delete()
{
    onDeleteCallback();

    if (parentSyllableGroup)
    {
        int i = 0;
        for (const auto& lyric : parentSyllableGroup->lyrics)
        {
            if (lyric.get() == this)
            {
                parentSyllableGroup->lyrics.erase(parentSyllableGroup->lyrics.begin() + i);
                break;
            }

            i++;
        }
    }

    if (parentLyricalPhrase)
    {
        int i = 0;
        for (const auto& lyric : parentLyricalPhrase->lyrics)
        {
            if (lyric.get() == this)
            {
                parentLyricalPhrase->lyrics.erase(parentLyricalPhrase->lyrics.begin() + i);
                break;
            }

            i++;
        }
    }
}

void CSLyric::Render(RenderData& renderData, const Settings& settings, Vec2<float> parentPosition, bool renderPickup) const
{
    Vec2<float> pos = position;
    if (renderPickup)
    {
        pos = pickupPosition;
    }
    
    lyricText.Render(renderData, parentPosition + pos, Paint(selectedColor.color));
/*
#if DEBUG
    if (isPickupToNextMeasure)
        lyricText.GetBoundingBox(Paint()).Render(renderData, Color(0xFFFF0000), parentPosition + position);
    else
        lyricText.GetBoundingBox(Paint()).Render(renderData, Color(0xFF00FF00), parentPosition + position);
#endif*/
}

void CSLyric::UpdateBoundingBox()
{
}
