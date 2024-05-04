#include "Editor.h"

#include "Properties.h"
#include "../Callbacks.h"

#include "../libs/nlohmann/json.hpp"
using json = nlohmann::json;

void AddBoolToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, bool value)
{
    jsonObject[heading][id]["name"] = name;
    jsonObject[heading][id]["type"] = "bool";
    jsonObject[heading][id]["value"] = value;
}

void AddTextToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, const std::string& value)
{
    jsonObject[heading][id]["name"] = name;
    jsonObject[heading][id]["type"] = "text";
    jsonObject[heading][id]["value"] = value;
}

void AddFloatToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, float value)
{
    jsonObject[heading][id]["name"] = name;
    jsonObject[heading][id]["type"] = "float";
    jsonObject[heading][id]["value"] = value;
}

void AddColorToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, uint32_t value)
{
    jsonObject[heading][id]["name"] = name;
    jsonObject[heading][id]["type"] = "color";
    jsonObject[heading][id]["value"] = value;
}


std::shared_ptr<BaseElement> Editor::FindSelectedElement(Vec2<float> point)
{
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
                                return lyric;
                            }
                        }

                        for (auto chord : measure->chords)
                        {
                            BoundingBox bb = chord->chordSymbol.GetBoundingBoxRelativeToParent();
                            bb.position += chord->position;
                            bb.position.x += measurePositionX;
                            bb.position += musicRenderer->systemPositions[systemIndex];

                            if (bb.DoesOverlapWithPoint(point))
                            {
                                return chord;
                            }
                        }

                        BoundingBox bb = measure->GetTotalBoundingBox(MusicDisplayConstants());
                        bb.position.x += measurePositionX;
                        bb.position += musicRenderer->systemPositions[systemIndex];

                        if (bb.DoesOverlapWithPoint(point))
                        {
                            return measure;
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
    
    return nullptr;
}

bool Editor::OnPointerEvent(const PointerEvent& event)
{
    switch (event.eventType)
    {
        case PointerEvent::PointerEventType::Down:
        {
            Vec2<float> point = event.position / musicRenderer->zoom;

            std::shared_ptr<BaseElement> selectedElement = FindSelectedElement(point);

            // deselect elements
            for (int i = selectedElements.size() - 1; i >= 0; i--)
            {
                std::shared_ptr<BaseElement> element = selectedElements[i];
                if (element->elementType == BaseElement::ElementType::CSLyric)
                {
                    std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(element);
                    lyric->lyricText.selectedColor = 0x000000FF;
                }
                else if (element->elementType == BaseElement::ElementType::CSChord)
                {
                    std::shared_ptr<CSChord> chord = std::dynamic_pointer_cast<CSChord>(element);
                    chord->chordSymbol.selectedColor = 0x000000FF;
                }
                element->selectedColor = 0x000000FF;

                selectedElements.erase(selectedElements.begin() + i);
            }

            // select elements
            if (selectedElement)
            {
                selectedElements.push_back(selectedElement);

                selectedElement->selectedColor = 0x3333EEFF;

                if (selectedElement->elementType == BaseElement::ElementType::CSLyric)
                {
                    std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(selectedElement);
                    lyric->lyricText.selectedColor = 0x3333EEFF;

                    nlohmann::ordered_json jsonObject;

                    AddTextToJson(jsonObject, "Text Properties", "text", "Text", lyric->lyricText.text);
                    AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)lyric->lyricText.fontWeight - 1));
                    AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)lyric->lyricText.fontStyle - 1));
                    AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", lyric->lyricText.fontSize.size);
                    AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", lyric->lyricText.color.color);
                    AddFloatToJson(jsonObject, "Position", "posX", "Position X", lyric->position.x);
                    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", lyric->position.y);

                    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
                }
                else if (selectedElement->elementType == BaseElement::ElementType::CSChord)
                {
                    std::shared_ptr<CSChord> chord = std::dynamic_pointer_cast<CSChord>(selectedElement);
                    chord->chordSymbol.selectedColor = 0x3333EEFF;
                    
                    nlohmann::ordered_json jsonObject;

                    AddTextToJson(jsonObject, "Main", "chordName", "Chord Name", chord->chordSymbol.chordName.string);
                    AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", chord->beatPosition);
                    AddFloatToJson(jsonObject, "Position", "posX", "Position X", chord->position.x);
                    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", chord->position.y);

                    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
                }
                else if (selectedElement->elementType == BaseElement::ElementType::CSMeasure)
                {
                    std::shared_ptr<CSMeasure> measure = std::dynamic_pointer_cast<CSMeasure>(selectedElement);
                    
                    nlohmann::ordered_json jsonObject;

                    AddFloatToJson(jsonObject, "Main", "width", "Width", measure->width);

                    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
                }
                
            }
            else
            {
                Callbacks::GetInstance().UpdateProperties("{}");
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
        if (e->elementType == BaseElement::ElementType::CSLyric)
        {
            std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(e);

            LOGD("props: %s", propertiesString.c_str());

            lyric->lyricText.text = jsonObject["Text Properties"]["text"]["value"];
            bool fontWeight = jsonObject["Text Properties"]["isBold"]["value"];
            bool fontStyle = jsonObject["Text Properties"]["isItalic"]["value"];

            lyric->lyricText.fontWeight = (FontWeight)(1 + fontWeight);
            lyric->lyricText.fontStyle = (FontStyle)(1 + fontStyle);
            lyric->lyricText.fontSize.size = jsonObject["Text Properties"]["textSize"]["value"];
            lyric->lyricText.color.color = jsonObject["Text Properties"]["color"]["value"];

            lyric->position.x = jsonObject["Position"]["posX"]["value"];
            lyric->position.y = jsonObject["Position"]["posY"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::CSChord)
        {
            std::shared_ptr<CSChord> chord = std::dynamic_pointer_cast<CSChord>(e);

            chord->chordSymbol.chordName.string = jsonObject["Main"]["chordName"]["value"];
            chord->beatPosition = jsonObject["Main"]["beatPosition"]["value"];

            chord->position.x = jsonObject["Position"]["posX"]["value"];
            chord->position.y = jsonObject["Position"]["posY"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            std::shared_ptr<CSMeasure> measure = std::dynamic_pointer_cast<CSMeasure>(e);

            measure->width = jsonObject["Main"]["width"]["value"];
        }
    }

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);
}