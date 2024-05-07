#include "CSLyric.h"

void CSLyric::Init(Vec2<float> pos)
{
    lyricText.fontSize.size = 8.0f;
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

void CSLyric::Render(RenderData& renderData, const Settings& settings, Vec2<float> parentPosition) const
{
    lyricText.Render(renderData, parentPosition + position, Paint());
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
