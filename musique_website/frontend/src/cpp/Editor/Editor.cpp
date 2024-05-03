#include "Editor.h"

#include "Properties.h"
#include "../Callbacks.h"

#include "../libs/nlohmann/json.hpp"
using json = nlohmann::json;

bool Editor::OnPointerEvent(const PointerEvent& event)
{
    switch (event.eventType)
    {
        case PointerEvent::PointerEventType::Down:
        {
            Vec2<float> point = event.position / musicRenderer->zoom;

            std::shared_ptr<BaseElement> selectedElement = nullptr;

            int systemIndex = 0;
            for (const auto& system : song->systems)
            {
                int instrumentIndex = 0;
                for (const auto& instrument : song->instruments)
                {
                    int staffIndex = 0;
                    for (const auto& staff : instrument->staves)
                    {
                        int start = system->beginningMeasureIndex;
                        int end = system->endingMeasureIndex;
                        
                        if (staff->csStaff && staff->type == Staff::StaffType::ChordSheet)
                        {
                            float measurePositionX = 0.0f;
                            for (int m = start; m <= end; m++)
                            {
                                auto measure = staff->csStaff->measures[m];

                                for (auto lyric : measure->lyrics)
                                {
                                    BoundingBox bb = lyric->lyricText.GetBoundingBox(Paint());
                                    bb.position += lyric->position;
                                    bb.position.x += measurePositionX;
                                    bb.position += musicRenderer->systemPositions[systemIndex];
                                    //musicRenderer->m_RenderData.AddDebugDot(bb.position);
                                    //bb.Render(musicRenderer->m_RenderData, 0xFF2222FF);

                                    if (bb.DoesOverlapWithPoint(point))
                                    {
                                        musicRenderer->m_RenderData.AddDebugDot(bb.position);
                                        selectedElement = lyric;
                                        break;
                                    }
                                }
                                
                                if (selectedElement)
                                    break;

                                measurePositionX += measure->width;
                            }
                        }

                        if (selectedElement)
                            break;

                        staffIndex++;
                    }

                    if (selectedElement)
                        break;

                    instrumentIndex++;
                }

                if (selectedElement)
                    break;

                systemIndex++;
            }

            // deselect elements
            for (int i = selectedElements.size() - 1; i >= 0; i--)
            {
                std::shared_ptr<BaseElement> element = selectedElements[i];
                std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(element);
                lyric->lyricText.color = 0x000000FF;
                selectedElements.erase(selectedElements.begin() + i);
            }

            // select elements
            if (selectedElement)
            {
                std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(selectedElement);
                lyric->lyricText.color = 0x3333EEFF;

                selectedElements.push_back(selectedElement);

                Properties properties;
                TextProperties text;
                text.text = lyric->lyricText.text;
                text.isBold = (int)lyric->lyricText.fontWeight - 1;
                text.isItalic = (int)lyric->lyricText.fontStyle - 1;
                text.size = lyric->lyricText.fontSize.size;
                text.color = lyric->lyricText.color;
                PositionProperties pos;
                pos.position = lyric->position;
                properties.textProperties.push_back(text);
                properties.positionProperties.push_back(pos);

                Callbacks::GetInstance().UpdateProperties(properties);
            }

            //musicRenderer->m_RenderData.AddDebugDot(event.position / musicRenderer->zoom);
            musicRenderer->updateRenderData = true;
            musicRenderer->layoutCalculated = false;
            //musicRenderer->RenderWithRenderData();
            musicRenderer->Render(song, song->settings);

            break;
        }
        case PointerEvent::PointerEventType::Up:
        {
            break;
        }
        case PointerEvent::PointerEventType::Move:
        {
            break;
        }
        default:
        {
            return false;
        }
    }
    
    return true;
}

bool Editor::OnTextFieldEvent(int id, const std::string& input)
{
    for (auto e : selectedElements)
    {
        std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(e);
        lyric->lyricText.text = input;
    }

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);

    return true;
}

void Editor::OnPropertiesUpdated(const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    for (auto e : selectedElements)
    {
        std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(e);

        LOGW("props: %s", propertiesString.c_str());

        lyric->lyricText.text = jsonObject["text"];
        bool fontWeight = jsonObject["isBold"];
        bool fontStyle = jsonObject["isItalic"];
        LOGW("fontWeight: %d, fontStyle: %d", fontWeight, fontStyle);

        lyric->lyricText.fontWeight = (FontWeight)(1 + fontWeight);
        lyric->lyricText.fontStyle = (FontStyle)(1 + fontStyle);
        lyric->lyricText.fontSize.size = jsonObject["text size"];
        lyric->lyricText.color.color = jsonObject["text color"];

        lyric->position.x = jsonObject["posx"];
        lyric->position.y = jsonObject["posy"];
    }

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);
}