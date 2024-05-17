#include "CSMeasure.h"

#include <algorithm>

#include "CSStaff.h"

void CSMeasure::Render(RenderData& renderData, const Settings& settings, const std::shared_ptr<SystemMeasure>& systemMeasure, Vec2<float> parentPosition) const
{
    Vec2<float> currentPosition = parentPosition + position;
    currentPosition.x += pickupWidth;

    if (!systemMeasure->isPickupMeasure)
    {
        renderData.AddLine(Line(currentPosition, { currentPosition.x, currentPosition.y + settings.displayConstants.measureBarlineHeight }, Paint()));
        renderData.AddLine(Line({ currentPosition.x + width, currentPosition.y }, { currentPosition.x + width, currentPosition.y + settings.displayConstants.measureBarlineHeight }, Paint()));
    }

    if (timeSignature && showTimeSignature)
    {
        timeSignature->Render(renderData, { currentPosition.x, currentPosition.y + (settings.displayConstants.measureBarlineHeight / 2.0f) }, settings.displayConstants.measureBarlineHeight);
    }

    for (const auto& chord : chords)
    {
        chord->Render(renderData, settings, currentPosition);
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

    if (lyricPickup)
    {
        for (const auto& lyric : lyricPickup->lyrics)
        {
            lyric->Render(renderData, settings, currentPosition, true);
        }
    }

    for (const auto& lyric : lyrics)
    {
        if (!lyric->isPickupToNextMeasure)
            lyric->Render(renderData, settings, currentPosition);
    }

    for (const auto& direction : textDirections)
    {
        direction->Render(renderData, currentPosition);
    }

    if (selectedColor.color != 0x000000FF)
    {
        Paint boxPaint;
        boxPaint.color = selectedColor.color;
        boxPaint.strokeWidth = 2.0f;
        renderData.AddLine(Line(currentPosition, { currentPosition.x, currentPosition.y + settings.displayConstants.measureBarlineHeight }, boxPaint));
        renderData.AddLine(Line({ currentPosition.x, currentPosition.y + settings.displayConstants.measureBarlineHeight }, { currentPosition.x + width, currentPosition.y + settings.displayConstants.measureBarlineHeight }, boxPaint));
        renderData.AddLine(Line({ currentPosition.x + width, currentPosition.y + settings.displayConstants.measureBarlineHeight }, { currentPosition.x + width, currentPosition.y }, boxPaint));
        renderData.AddLine(Line({ currentPosition.x + width, currentPosition.y }, currentPosition, boxPaint));
    }
}

void CSMeasure::Init(const Settings& settings)
{
    float barlineMargin = settings.displayConstants.chordMarginFromBarline;
    float beatWidth = settings.displayConstants.beatWidth;
    float chordPosY = settings.displayConstants.chordPositionY;
    float lyricPosY = settings.displayConstants.lyricPositionY;
    float lyricSpace = settings.displayConstants.lyricSpaceWidth;
    float minMeasureWidth = settings.displayConstants.minimumMeasureWidth;
    bool displayReminderPickupLyrics = settings.displayConstants.displayReminderPickupLyrics;

    if (timeSignature && showTimeSignature)
    {
        timeSignature->position.x = 5.0f;
        barlineMargin += timeSignature->GetWidth(settings.displayConstants.measureBarlineHeight) + 5.0f;
    }

    boundingBox = BoundingBox();
    boundingBox.size.x = width;
    boundingBox.size.y = lyricPosY + (settings.displayConstants.lyricFontSize.size / 2.0f); // only an estimation

    std::sort(chords.begin(), chords.end(), [](std::shared_ptr<CSChord> a, std::shared_ptr<CSChord> b)
        {
            return a->beatPosition < b->beatPosition;
        });

    std::sort(lyrics.begin(), lyrics.end(), [](std::shared_ptr<CSLyric> a, std::shared_ptr<CSLyric> b)
        {
            return a->beatPosition < b->beatPosition;
        });
    
    std::shared_ptr<CSLyric> previousLyric = nullptr;
    bool pickupStarted = false;
    std::shared_ptr<LyricPickup> currentLyricPickup = nullptr;
    for (const auto& lyric : lyrics)
    {
        if (previousLyric != nullptr)
        {
            previousLyric->duration = lyric->beatPosition - previousLyric->beatPosition;
        }

        
        if (lyric->startsPickup || pickupStarted)
        {
            pickupStarted = true;
            lyric->isPickupToNextMeasure = true;

            if (currentLyricPickup == nullptr)
            {
                currentLyricPickup = std::make_shared<LyricPickup>();
            }

            currentLyricPickup->lyrics.push_back(lyric);
            lyric->parentLyricPickup = currentLyricPickup;
        }
        else
        {
            lyric->parentLyricPickup = nullptr;
            lyric->isPickupToNextMeasure = false;
        }
        
        previousLyric = lyric;
    }

    // set this measure's pickup (by using the previous measure)
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

            if (i > 0)
            {
                if (!staff->measures[i - 1]->lyrics.empty())
                {
                    lyricPickup = staff->measures[i - 1]->lyrics.back()->parentLyricPickup;
                }
            }
        }       
    }

    if (lyricPickup)
        pickupWidth = -lyricPickup->lyrics[0]->pickupPosition.x;
    else
        pickupWidth = 0.0f;

    if (previousLyric != nullptr)
        previousLyric->duration = duration - previousLyric->beatPosition;

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
        chord->Init(settings, position);

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

        if (lyric->isPickupToNextMeasure && lyric->parentLyricPickup && !displayReminderPickupLyrics)
        {
            lyric->Init(settings, { 0.0f, 0.0f });
            continue;
        }

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

        lyric->Init(settings, lyricPosition);

        prevPos = { lyricPosition.x + std::max(dimensions.x + lyricSpace, (lyric->duration * beatWidth)), lyricPosY };
    }


    prevPos = { 0.0f, lyricPosY };
    if (!lyrics.empty())
    {
        if (lyrics.back()->parentLyricPickup)
        {
            for (int i = lyrics.back()->parentLyricPickup->lyrics.size() - 1; i >= 0; i--)
            {
                std::shared_ptr<CSLyric> lyric = lyrics.back()->parentLyricPickup->lyrics[i];
                Vec2<float> dimensions = lyric->lyricText.GetDimensions(Paint());

                lyric->pickupPosition = { prevPos.x - (dimensions.x + lyricSpace), prevPos.y };

                prevPos = lyric->pickupPosition;
            }
        }
    }

    float chordPosX = 0.0f;
    float lyricPosX = 0.0f;

    if (!chords.empty())
        chordPosX = chords.back()->GetDimensions().x + chords.back()->position.x + chords.back()->duration * 30.0f;

    for (int i = lyrics.size() - 1; i >= 0; i--)
    {
        if (!lyrics[i]->parentLyricPickup)
        {
            lyricPosX = lyrics[i]->lyricText.GetDimensions(Paint()).x + lyrics[i]->position.x;
            break;
        }
    }
    //if (!lyrics.empty())
    //    lyricPosX = lyrics.back()->lyricText.GetDimensions(Paint()).x + lyrics.back()->position.x;

    width = std::max(std::max(chordPosX, lyricPosX), minMeasureWidth);
    /*if (defaultWidth != 0.0f)
    {
        width = defaultWidth;
    }*/

    for (const auto& direction : textDirections)
    {
        direction->SetOnDeleteCallback([&](){
            int i = 0;
            for (const auto& d : textDirections)
            {
                if (d == direction)
                {
                    textDirections.erase(textDirections.begin() + i);
                    break;
                }

                i++;
            }
        });

        direction->position = { -pickupWidth, -20.0f };
        direction->text.fontSize.size = 16.0f;

        boundingBox = BoundingBox::CombineBoundingBoxes(boundingBox, direction->GetBoundingBoxRelativeToParent());
    }
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
    BoundingBox totalBoundingBox = boundingBox;

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