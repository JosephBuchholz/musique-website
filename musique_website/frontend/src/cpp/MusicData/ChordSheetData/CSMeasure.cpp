#include "CSMeasure.h"

#include <algorithm>

#include "CSStaff.h"

void CSMeasure::Render(RenderData& renderData, const Settings& settings, Vec2<float> parentPosition) const
{
    Vec2<float> currentPosition = parentPosition + position;

    renderData.AddLine(Line(currentPosition, { currentPosition.x, currentPosition.y + 40.0f }, Paint()));
    renderData.AddLine(Line({ currentPosition.x + width, currentPosition.y }, { currentPosition.x + width, currentPosition.y + 40.0f }, Paint()));

    for (const auto& chord : chords)
    {
        chord->Render(renderData, settings, parentPosition + position);
    }

    if (isFirstMeasureOfSystem && !lyrics.empty())
    {
        std::shared_ptr<LyricalPhrase> lyricalPhrase = lyrics[0]->parentLyricalPhrase;

        if (lyricalPhrase)
        {
            for (const auto& lyric : lyricalPhrase->lyrics)
            {
                if (lyric->isPickupToNextMeasure)
                {
                    lyric->Render(renderData, settings, currentPosition);
                }
                else
                    break;
            }
        }
    }

    for (const auto& lyric : lyrics)
    {
        if (!lyric->isPickupToNextMeasure)
            lyric->Render(renderData, settings, currentPosition);
    }
}

void CSMeasure::Init()
{
    const float barlineMargin = 12.0f;
    const float beatWidth = 30.0f;
    const float chordPosY = 25.0f;
    const float lyricPosY = 60.0f;
    const float lyricSpace = 8.0f;
    const float minMeasureWidth = 30.0f;

    std::sort(chords.begin(), chords.end(), [](std::shared_ptr<CSChord> a, std::shared_ptr<CSChord> b)
        {
            return a->beatPosition < b->beatPosition;
        });

    std::sort(lyrics.begin(), lyrics.end(), [](std::shared_ptr<CSLyric> a, std::shared_ptr<CSLyric> b)
        {
            return a->beatPosition < b->beatPosition;
        });
    
    std::shared_ptr<CSLyric> previousLyric = nullptr;
    for (const auto& lyric : lyrics)
    {
        if (previousLyric != nullptr)
        {
            previousLyric->duration = lyric->beatPosition - previousLyric->beatPosition;
        }
        
        previousLyric = lyric;
    }

    if (previousLyric != nullptr)
        previousLyric->duration = 4.0f - previousLyric->beatPosition;

    float defaultWidth = width;
    Vec2<float> previousPosition = { barlineMargin, chordPosY };
    for (const auto& chord : chords)
    {
        chord->SetOnDeleteCallback([&](){
            int i = 0;
            for (const auto& c : chords)
            {
                if (c == chord)
                {
                    chords.erase(chords.begin() + i);
                    break;
                }

                i++;
            }
        });

        Vec2<float> dimensions = chord->GetDimensions();
        Vec2<float> position = previousPosition;
        chord->Init(position);

        previousPosition = position;
        previousPosition.x += dimensions.x + chord->duration * beatWidth;
    }


    /*if (isFirstMeasureOfSystem && !lyrics.empty())
    {
        std::shared_ptr<LyricalPhrase> lyricalPhrase = lyrics[0]->parentLyricalPhrase;

        if (lyricalPhrase)
        {
            Vec2<float> prevPos = { 0.0f, 0.0f };
            for (int i = (int)lyricalPhrase->lyrics.size() - 1; i >= 0; i--)
            {
                std::shared_ptr<CSLyric> lyric = lyricalPhrase->lyrics[i];

                if (lyric->isPickupToNextMeasure)
                {
                    Vec2<float> dimensions = lyric->lyricText.GetDimensions(Paint());

                    Vec2<float> lyricPosition = { prevPos.x - dimensions.x, 55.0f };

                    lyric->Init(lyricPosition);

                    prevPos = lyricPosition;
                }
            }
        }
    }*/

    Vec2<float> prevPos = { 0.0f, lyricPosY };
    for (const auto& lyric : lyrics)
    {
        lyric->SetOnDeleteCallback([&](){
            int i = 0;
            for (const auto& l : lyrics)
            {
                if (l == lyric)
                {
                    lyrics.erase(lyrics.begin() + i);
                    break;
                }

                i++;
            }
        });

        if (lyric->isPickupToNextMeasure)
            continue;

        Vec2<float> dimensions = lyric->lyricText.GetDimensions(Paint());

        float chordPositionX = GetPositionXFromBeatPositionOfChords(lyric->beatPosition);

        Vec2<float> lyricPosition = prevPos;

        if (lyricPosition.x > chordPositionX)
        {
            // change chord position x
            std::shared_ptr<CSChord> chord = GetChordFromBeatPosition(lyric->beatPosition);

            if (chord != nullptr)
                chord->position.x = lyricPosition.x;
        }
        else if (lyricPosition.x < chordPositionX)
        {
            lyricPosition.x = chordPositionX;
        }

        lyric->Init(lyricPosition);

        prevPos = { lyricPosition.x + std::max(dimensions.x + lyricSpace, (lyric->duration * beatWidth)), lyricPosY };
    }

    float chordPosX = 0.0f;
    float lyricPosX = 0.0f;

    if (!chords.empty())
        chordPosX = chords.back()->GetDimensions().x + chords.back()->position.x + chords.back()->duration * 30.0f;

    if (!lyrics.empty())
        lyricPosX = lyrics.back()->lyricText.GetDimensions(Paint()).x + lyrics.back()->position.x;

    width = std::max(std::max(chordPosX, lyricPosX), minMeasureWidth);
    /*if (defaultWidth != 0.0f)
    {
        width = defaultWidth;
    }*/
}

void CSMeasure::Delete()
{
    if (parent)
    {
        if (parent->elementType == BaseElement::ElementType::CSStaff)
        {
            std::shared_ptr<CSStaff> staff = std::dynamic_pointer_cast<CSStaff>(parent);

            int i = 0;
            for (const auto& m : staff->measures)
            {
                if (m.get() == this)
                {
                    break;
                }

                i++;
            }

            staff->measures.erase(staff->measures.begin() + i);
        }       
    }
}

float CSMeasure::GetPositionXFromBeatPositionOfChords(float beatPosition) const
{
    for (const auto& chord : chords)
    {
        if (beatPosition == chord->beatPosition) // found
        {
            return chord->position.x;
        }

        // else beatPos < chord.beatPos: continue
    }

    return 0.0f;
}

std::shared_ptr<CSChord> CSMeasure::GetChordFromBeatPosition(float beatPosition)
{
    float previousChordBeatPosition = 0.0f;
    for (const auto& chord : chords)
    {
        /*if (beatPosition >= previousChordBeatPosition && beatPosition <= chord->beatPosition) // found
        {
            return chord;
        }

        previousChordBeatPosition = chord->beatPosition;*/

        if (beatPosition == chord->beatPosition)
        {
            return chord;
        }

        // else beatPos < chord.beatPos: continue
    }

    return nullptr;
}

BoundingBox CSMeasure::GetTotalBoundingBox(const MusicDisplayConstants& displayConstants) const
{
    BoundingBox totalBoundingBox;

    totalBoundingBox.size.x = width;
    totalBoundingBox.size.y = 100.0f;

    return totalBoundingBox;
}

int CSMeasure::GetMeasureIndex() const
{
    int measureIndex = 0;
    if (parent)
    {
        if (parent->elementType == BaseElement::ElementType::CSStaff)
        {
            std::shared_ptr<CSStaff> staff = std::dynamic_pointer_cast<CSStaff>(parent);

            int measureIndex = 0;
            for (const auto& m : staff->measures)
            {
                if (m.get() == this)
                {
                    break;
                }

                measureIndex++;
            }
            
            return measureIndex;
        }
    }

    ASSERT(false);
    return 0;
}