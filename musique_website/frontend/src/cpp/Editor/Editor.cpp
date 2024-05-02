#include "Editor.h"

bool Editor::OnPointerEvent(const PointerEvent& event)
{
    switch (event.eventType)
    {
        case PointerEvent::PointerEventType::Down:
        {
            LOGD("Down: %f", musicRenderer->zoom);
            //selectedMeasure->c = 0xFF0000FF;

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
                                        lyric->lyricText.color = 0x3333EEFF;
                                        selectedElement = lyric;
                                    }
                                }

                                measurePositionX += measure->width;
                            }
                        }

                        staffIndex++;
                    }

                    instrumentIndex++;
                }

                systemIndex++;
            }

            if (selectedElement)
            {
                for (int i = selectedElements.size() - 1; i >= 0; i--)
                {
                    std::shared_ptr<BaseElement> element = selectedElements[i];
                    std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(element);
                    lyric->lyricText.color = 0x000000FF;
                    selectedElements.erase(selectedElements.begin() + i);
                }

                selectedElements.push_back(selectedElement);
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
            LOGD("Up");
            break;
        }
        case PointerEvent::PointerEventType::Move:
        {
            //LOGD("Move");

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