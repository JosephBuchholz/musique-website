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
    /*for (const Credit& credit : song->credits)
    {
        BoundingBox bb = credit.GetBoundingBox();
        bb.position += musicRenderer->pagePositions[credit.pageNumber - 1];

        if (bb.DoesOverlapWithPoint(point))
        {
            return credit;
        }
    }*/

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
            Vec2<float> point = (event.position / musicRenderer->zoom) - musicRenderer->offset;

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

                    AddTextToJson(jsonObject, "Main", "text", "Text", lyric->lyricText.text);
                    AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", lyric->beatPosition);
                    AddFloatToJson(jsonObject, "Main", "duration", "Duration", lyric->duration);

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

                    AddTextToJson(jsonObject, "Main", "chordName", "Chord Name", chord->chordSymbol.GetChordNameAsStandardString());
                    AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", chord->beatPosition);
                    AddFloatToJson(jsonObject, "Main", "duration", "Duration", chord->duration);

                    AddFloatToJson(jsonObject, "Position", "posX", "Position X", chord->position.x);
                    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", chord->position.y);

                    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
                }
                else if (selectedElement->elementType == BaseElement::ElementType::CSMeasure)
                {
                    std::shared_ptr<CSMeasure> measure = std::dynamic_pointer_cast<CSMeasure>(selectedElement);

                    int measureIndex = measure->GetMeasureIndex();
                            
                    nlohmann::ordered_json jsonObject;

                    AddFloatToJson(jsonObject, "Main", "width", "Width", measure->width);
                    AddBoolToJson(jsonObject, "Main", "pageBreak", "Page Break", song->systemMeasures[measureIndex]->pageBreak);
                    AddBoolToJson(jsonObject, "Main", "systemBreak", "System Break", song->systemMeasures[measureIndex]->systemBreak);

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


            pointerIsDown = true;
            pointerDownPosition = event.position;

            break;
        }
        case PointerEvent::PointerEventType::Up:
        {
            if (pointerIsDown)
            {
                Vec2<float> offset = event.position - pointerDownPosition;
                musicRenderer->offset += offset / musicRenderer->zoom;
                pointerDownPosition = event.position;

                musicRenderer->updateRenderData = true;
                musicRenderer->Render(song, song->settings);

                pointerIsDown = false;
            }

            break;
        }
        case PointerEvent::PointerEventType::Move:
        {
            if (pointerIsDown)
            {
                Vec2<float> offset = event.position - pointerDownPosition;
                musicRenderer->offset += offset / musicRenderer->zoom;
                pointerDownPosition = event.position;

                musicRenderer->updateRenderData = true;
                musicRenderer->Render(song, song->settings);
            }

            break;
        }
        default:
        {
            return false;
        }
    }
    
    return true;
}

bool Editor::OnKeyboardEvent(const KeyboardEvent& event)
{
    if (event.eventType == KeyboardEvent::KeyboardEventType::Down)
    {
        if (event.keyCode == KeyboardEvent::KeyCode::Delete)
        {
            OnDeleteSelected();
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

            lyric->lyricText.text = jsonObject["Main"]["text"]["value"];
            lyric->beatPosition = jsonObject["Main"]["beatPosition"]["value"];
            lyric->duration = jsonObject["Main"]["duration"]["value"];

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

            chord->chordSymbol = Chord::CreateChordFromString(jsonObject["Main"]["chordName"]["value"]);
            chord->chordSymbol.CalculateChordName(Settings());
            chord->beatPosition = jsonObject["Main"]["beatPosition"]["value"];
            chord->duration = jsonObject["Main"]["duration"]["value"];

            chord->position.x = jsonObject["Position"]["posX"]["value"];
            chord->position.y = jsonObject["Position"]["posY"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            std::shared_ptr<CSMeasure> measure = std::dynamic_pointer_cast<CSMeasure>(e);
            int measureIndex = measure->GetMeasureIndex();

            measure->width = jsonObject["Main"]["width"]["value"];
            song->systemMeasures[measureIndex]->pageBreak = jsonObject["Main"]["pageBreak"]["value"];
            song->systemMeasures[measureIndex]->systemBreak = jsonObject["Main"]["systemBreak"]["value"];
        }
    }

    song->OnUpdate();
    song->CalculateSystems();
    song->CalculateSystemPositionsAndPageBreaks();

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);
}

void Editor::OnNewElement(int id)
{
    if (!selectedElements.empty())
    {
        std::shared_ptr<BaseElement> e = selectedElements[0];
        if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            std::shared_ptr<CSMeasure> measure = std::dynamic_pointer_cast<CSMeasure>(e);

            if (id == 0)
            {
                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                newLyric->lyricText.text = "none";

                measure->lyrics.push_back(newLyric);
            }
            else if (id == 1)
            {
                std::shared_ptr<CSChord> newChord = std::make_shared<CSChord>();
                newChord->chordSymbol = Chord::CreateChordFromString("C");
                newChord->chordSymbol.CalculateChordName(Settings());

                measure->chords.push_back(newChord);
            }
            else if (id == 2 || id == 3 || id == 4) // insert measure
            {
                std::shared_ptr<CSMeasure> newMeasure = std::make_shared<CSMeasure>();
                if (measure->parent->elementType == BaseElement::ElementType::CSStaff)
                {
                    std::shared_ptr<CSStaff> staff = std::dynamic_pointer_cast<CSStaff>(measure->parent);
                    newMeasure->parent = staff;

                    std::shared_ptr<SystemMeasure> newSystemMeasure = std::make_shared<SystemMeasure>();

                    if (id != 4)
                    {
                        int i = 0;
                        for (const auto& m : staff->measures)
                        {
                            if (m == measure)
                            {
                                break;
                            }

                            i++;
                        }

                        if (id == 3) // insert after, else insert before
                            i++;

                        staff->measures.insert(staff->measures.begin() + i, newMeasure);
                        song->systemMeasures.insert(song->systemMeasures.begin() + i, newSystemMeasure);
                    }
                    else
                    {
                        staff->measures.push_back(newMeasure);
                        song->systemMeasures.push_back(newSystemMeasure);
                    }
                }
            }
        }
    }
    else
    {
        if (id == 4) // append measure
        {
            std::shared_ptr<CSMeasure> newMeasure = std::make_shared<CSMeasure>();
            std::shared_ptr<SystemMeasure> newSystemMeasure = std::make_shared<SystemMeasure>();
            for (auto& instrument : song->instruments)
            {
                for (auto& staff : instrument->staves)
                {
                    if (staff->csStaff)
                    {
                        newMeasure->parent = staff->csStaff;
                        staff->csStaff->measures.push_back(newMeasure);
                        song->systemMeasures.push_back(newSystemMeasure);
                    }
                }
            }
        }
    }

    song->OnUpdate();
    song->CalculateSystems();
    song->CalculateSystemPositionsAndPageBreaks();

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);
}

void Editor::OnDeleteSelected()
{
    for (auto e : selectedElements)
    {
        if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            std::shared_ptr<CSMeasure> measure = std::dynamic_pointer_cast<CSMeasure>(e);
            //measure->Delete();

            if (measure->parent)
            {
                if (measure->parent->elementType == BaseElement::ElementType::CSStaff)
                {
                    std::shared_ptr<CSStaff> staff = std::dynamic_pointer_cast<CSStaff>(measure->parent);

                    int i = 0;
                    for (const auto& m : staff->measures)
                    {
                        if (m == measure)
                        {
                            break;
                        }

                        i++;
                    }

                    staff->measures.erase(staff->measures.begin() + i);
                    song->systemMeasures.erase(song->systemMeasures.begin() + i);
                    int measureIndex = i;
                    int systemIndexToErase = -1;

                    int systemIndex = 0;
                    for (auto& system : song->systems)
                    {
                        if (system->beginningMeasureIndex <= measureIndex && system->endingMeasureIndex >= measureIndex)
                        {
                            system->endingMeasureIndex--;
                            system->systemMeasures.erase(system->systemMeasures.begin() + (measureIndex - system->beginningMeasureIndex));
                            system->systemMeasureData.erase(measureIndex);

                            if (system->endingMeasureIndex < system->beginningMeasureIndex)
                            {
                                systemIndexToErase = systemIndex; 
                            }
                        }
                        else if (system->beginningMeasureIndex > measureIndex)
                        {
                            system->beginningMeasureIndex--;
                            system->endingMeasureIndex--;
                        }

                        systemIndex++;
                    }

                    if (systemIndexToErase != -1)
                    {
                        song->systems.erase(song->systems.begin() + systemIndexToErase);
                    }
                }       
            }
        }
        else
        {
            e->Delete();
        }
    }

    selectedElements.clear();

    song->OnUpdate();
    song->CalculateSystems();
    song->CalculateSystemPositionsAndPageBreaks();

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);
}