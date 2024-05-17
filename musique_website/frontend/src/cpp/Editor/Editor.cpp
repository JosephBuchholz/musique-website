#include "Editor.h"

#include <sstream>

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

void AddIntToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, int value)
{
    jsonObject[heading][id]["name"] = name;
    jsonObject[heading][id]["type"] = "int";
    jsonObject[heading][id]["value"] = value;
}

void AddColorToJson(nlohmann::ordered_json& jsonObject, const std::string& heading, const std::string& id, const std::string& name, uint32_t value)
{
    jsonObject[heading][id]["name"] = name;
    jsonObject[heading][id]["type"] = "color";
    jsonObject[heading][id]["value"] = value;
}


BaseElement* Editor::FindSelectedElement(Vec2<float> point)
{
    for (const auto& credit : song->credits)
    {
        BoundingBox bb = credit->GetBoundingBox();
        bb.position += musicRenderer->pagePositions[credit->pageNumber - 1];

        if (bb.DoesOverlapWithPoint(point))
        {
            return credit.get();
        }
    }

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
                        CSMeasure* measure = staff->csStaff->measures[m].get();

                        if (measure->timeSignature && measure->showTimeSignature)
                        {
                            BoundingBox bb = BoundingBox();
                            bb.size.y = song->settings.displayConstants.measureBarlineHeight;
                            bb.size.x = measure->timeSignature->GetWidth(bb.size.y);

                            bb.position += measure->timeSignature->position;
                            bb.position.x += measurePositionX + measure->pickupWidth;
                            bb.position += musicRenderer->systemPositions[systemIndex];

                            if (bb.DoesOverlapWithPoint(point))
                            {
                                return measure->timeSignature;
                            }
                        }

                        for (auto lyric : measure->lyrics)
                        {
                            BoundingBox bb = lyric->lyricText.GetBoundingBox(Paint());
                            bb.position += lyric->position;
                            bb.position.x += measurePositionX + measure->pickupWidth;
                            bb.position += musicRenderer->systemPositions[systemIndex];
                            //musicRenderer->m_RenderData.AddDebugDot(bb.position);
                            //bb.Render(musicRenderer->m_RenderData, 0xFF2222FF);

                            if (bb.DoesOverlapWithPoint(point))
                            {
                                return lyric.get();
                            }
                        }

                        if (measure->lyricPickup)
                        {
                            for (auto lyric : measure->lyricPickup->lyrics)
                            {
                                BoundingBox bb = lyric->lyricText.GetBoundingBox(Paint());
                                bb.position += lyric->pickupPosition;
                                bb.position.x += measurePositionX + measure->pickupWidth;
                                bb.position += musicRenderer->systemPositions[systemIndex];
                                //musicRenderer->m_RenderData.AddDebugDot(bb.position);
                                //bb.Render(musicRenderer->m_RenderData, 0xFF2222FF);

                                if (bb.DoesOverlapWithPoint(point))
                                {
                                    return lyric.get();
                                }   
                            }
                        }

                        for (auto chord : measure->chords)
                        {
                            BoundingBox bb = chord->GetBoundingBox();
                            bb.position.x += measurePositionX + measure->pickupWidth;
                            bb.position += musicRenderer->systemPositions[systemIndex];

                            if (bb.DoesOverlapWithPoint(point))
                            {
                                return chord.get();
                            }
                        }

                        for (const auto& direction : measure->textDirections)
                        {
                            BoundingBox bb = direction->GetBoundingBoxRelativeToParent();
                            bb.position.x += measurePositionX + measure->pickupWidth;
                            bb.position += musicRenderer->systemPositions[systemIndex];

                            if (bb.DoesOverlapWithPoint(point))
                            {
                                return direction.get();
                            }
                        }

                        BoundingBox bb = measure->GetTotalBoundingBox(MusicDisplayConstants());
                        bb.position.x += measurePositionX + measure->pickupWidth;
                        bb.position += musicRenderer->systemPositions[systemIndex];

                        if (bb.DoesOverlapWithPoint(point))
                        {
                            return measure;
                        }

                        measurePositionX += measure->GetTotalWidth();
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

            BaseElement* selectedElement = FindSelectedElement(point);

            SetSelection({ selectedElement });

            Update();

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
        CSLyric* lyric = dynamic_cast<CSLyric*>(e);
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
            CSLyric* lyric = dynamic_cast<CSLyric*>(e);

            lyric->beatPosition = jsonObject["Main"]["beatPosition"]["value"];
            lyric->duration = jsonObject["Main"]["duration"]["value"];
            lyric->startsPickup = jsonObject["Main"]["startsPickup"]["value"];

            std::string text = jsonObject["Main"]["text"]["value"];

            if (text.back() == ' ')
            {
                text.pop_back();
                lyric->lyricText.text = text;

                CSMeasure* measure = nullptr;

                for (auto& instrument : song->instruments)
                {
                    for (auto& staff : instrument->staves)
                    {
                        if (staff->csStaff)
                        {
                            for (auto& m : staff->csStaff->measures)
                            {
                                for (auto& l : m->lyrics)
                                {
                                    if (l.get() == lyric)
                                    {
                                        measure = m.get();
                                        break;
                                    }
                                }

                                if (measure) break;
                            }
                        }

                        if (measure) break;
                    }

                    if (measure) break;
                }

                ASSERT(measure);

                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                newLyric->lyricText.text = "";
                newLyric->beatPosition = lyric->beatPosition;
                measure->lyrics.push_back(newLyric);

                SetSelection({ newLyric.get() });
            }
            else
                lyric->lyricText.text = text;

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
            CSChord* chord = dynamic_cast<CSChord*>(e);

            chord->chordSymbol = Chord::CreateChordFromString(jsonObject["Main"]["chordName"]["value"]);
            chord->chordSymbol.CalculateChordName(song->settings);
            chord->beatPosition = jsonObject["Main"]["beatPosition"]["value"];
            chord->duration = jsonObject["Main"]["duration"]["value"];

            chord->position.x = jsonObject["Position"]["posX"]["value"];
            chord->position.y = jsonObject["Position"]["posY"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            CSMeasure* measure = dynamic_cast<CSMeasure*>(e);
            int measureIndex = measure->GetMeasureIndex();

            measure->width = jsonObject["Main"]["width"]["value"];
            song->systemMeasures[measureIndex]->pageBreak = jsonObject["Main"]["pageBreak"]["value"];
            song->systemMeasures[measureIndex]->systemBreak = jsonObject["Main"]["systemBreak"]["value"];
            song->systemMeasures[measureIndex]->isPickupMeasure = jsonObject["Main"]["isPickupMeasure"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::Credit)
        {
            Credit* credit = dynamic_cast<Credit*>(e);

            credit->words.text = jsonObject["Main"]["text"]["value"];
            credit->pageNumber = jsonObject["Main"]["pageNumber"]["value"];

            bool fontWeight = jsonObject["Text Properties"]["isBold"]["value"];
            bool fontStyle = jsonObject["Text Properties"]["isItalic"]["value"];

            credit->words.fontWeight = (FontWeight)(1 + fontWeight);
            credit->words.fontStyle = (FontStyle)(1 + fontStyle);
            credit->words.fontSize.size = jsonObject["Text Properties"]["textSize"]["value"];
            credit->words.color.color = jsonObject["Text Properties"]["color"]["value"];

            credit->words.positionX = jsonObject["Position"]["posX"]["value"];
            credit->words.positionY = jsonObject["Position"]["posY"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::TextDirection)
        {
            TextDirection* direction = dynamic_cast<TextDirection*>(e);

            direction->text.text = jsonObject["Main"]["text"]["value"];

            bool fontWeight = jsonObject["Text Properties"]["isBold"]["value"];
            bool fontStyle = jsonObject["Text Properties"]["isItalic"]["value"];

            direction->text.fontWeight = (FontWeight)(1 + fontWeight);
            direction->text.fontStyle = (FontStyle)(1 + fontStyle);
            direction->text.fontSize.size = jsonObject["Text Properties"]["textSize"]["value"];
            direction->text.color.color = jsonObject["Text Properties"]["color"]["value"];

            direction->position.x = jsonObject["Position"]["posX"]["value"];
            direction->position.y = jsonObject["Position"]["posY"]["value"];
        }
        else if (e->elementType == BaseElement::ElementType::TimeSignature)
        {
            TimeSignature* timeSignature = dynamic_cast<TimeSignature*>(e);
            SetTimeSignatureProperties(timeSignature, propertiesString);
        }
    }

    if (selectedElements.empty())
    {
        song->settings.displayConstants.lyricFontSize.size = jsonObject["Lyrics"]["fontSize"]["value"];
        song->settings.displayConstants.lyricPositionY = jsonObject["Lyrics"]["lyricPositionY"]["value"];
        song->settings.displayConstants.lyricSpaceWidth = jsonObject["Lyrics"]["lyricSpaceWidth"]["value"];

        song->settings.displayConstants.chordMarginFromBarline = jsonObject["Other"]["chordMarginFromBarline"]["value"];
        song->settings.displayConstants.beatWidth = jsonObject["Other"]["beatWidth"]["value"];
        song->settings.displayConstants.chordPositionY = jsonObject["Other"]["chordPositionY"]["value"];
        song->settings.displayConstants.minimumMeasureWidth = jsonObject["Other"]["minimumMeasureWidth"]["value"];
        song->settings.displayConstants.displayReminderPickupLyrics = jsonObject["Other"]["displayReminderPickupLyrics"]["value"];
        song->settings.displayConstants.measureBarlineHeight = jsonObject["Other"]["measureBarlineHeight"]["value"];
        song->settings.displayConstants.chordFontSize.size = jsonObject["Other"]["chordFontSize"]["value"];

        song->settings.displayConstants.systemLayout.systemDistance = jsonObject["Systems"]["systemDistance"]["value"];
        song->settings.displayConstants.systemLayout.topSystemDistance = jsonObject["Systems"]["topSystemDistance"]["value"];
        song->settings.displayConstants.systemLayout.firstPageTopSystemDistance = jsonObject["Systems"]["firstPageTopSystemDistance"]["value"];

        song->settings.displayConstants.topMargin = jsonObject["Pages"]["topMargin"]["value"];
        song->settings.displayConstants.bottomMargin = jsonObject["Pages"]["bottomMargin"]["value"];
        song->settings.displayConstants.leftMargin = jsonObject["Pages"]["leftMargin"]["value"];
        song->settings.displayConstants.rightMargin = jsonObject["Pages"]["rightMargin"]["value"];
    }

    Update();
}

void Editor::OnNewElement(int id)
{
    if (!selectedElements.empty())
    {
        BaseElement* e = selectedElements[0];
        if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            CSMeasure* measure = dynamic_cast<CSMeasure*>(e);

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
                newChord->chordSymbol.CalculateChordName(song->settings);

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
                            if (m.get() == measure)
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
            else if (id == 5) // rehearsal marking
            {
                std::shared_ptr<Rehearsal> newRehearsal = std::make_shared<Rehearsal>();
                newRehearsal->text.text = "A";
                newRehearsal->beatPosition = 0.0f;

                measure->textDirections.push_back(newRehearsal);
            }
            else if (id == 6) // time signature
            {
                std::unique_ptr<TimeSignature> newTimeSignature = std::make_unique<TimeSignature>();

                for (auto instrument : song->instruments)
                {
                    for (auto staff : instrument->staves)
                    {
                        if (staff->csStaff)
                        {
                            //TimeSignature* oldTimeSignature = nullptr;

                            int i = 0;
                            for (auto m : staff->csStaff->measures)
                            {
                                if (m.get() == measure)
                                {
                                    SystemMeasure* systemMeasure = song->systemMeasures[i].get();
                                    systemMeasure->timeSignature = std::move(newTimeSignature);
                                    measure->showTimeSignature = true;
                                    break;
                                }

                                i++;
                            }
                        }
                    }
                }
            }
        }
        else if (e->elementType == BaseElement::ElementType::CSChord)
        {
            CSChord* chord = dynamic_cast<CSChord*>(e);

            if (id == 0) // add lyric
            {
                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                newLyric->lyricText.text = "none";
                newLyric->beatPosition = chord->beatPosition;

                for (auto& instrument : song->instruments)
                {
                    for (auto& staff : instrument->staves)
                    {
                        if (staff->csStaff)
                        {
                            for (auto& measure : staff->csStaff->measures)
                            {
                                for (auto& c : measure->chords)
                                {
                                    if (c.get() == chord)
                                    {
                                        measure->lyrics.push_back(newLyric);
                                    }
                                }
                            }
                        }
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

    Update();
}

void Editor::OnDeleteSelected()
{
    for (auto e : selectedElements)
    {
        if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            CSMeasure* measure = dynamic_cast<CSMeasure*>(e);
            //measure->Delete();

            if (measure->parent)
            {
                if (measure->parent->elementType == BaseElement::ElementType::CSStaff)
                {
                    std::shared_ptr<CSStaff> staff = std::dynamic_pointer_cast<CSStaff>(measure->parent);

                    int i = 0;
                    for (const auto& m : staff->measures)
                    {
                        if (m.get() == measure)
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
                            //system->systemMeasures.erase(system->systemMeasures.begin() + (measureIndex - system->beginningMeasureIndex));
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
        else if (e->elementType == BaseElement::ElementType::TimeSignature)
        {
            TimeSignature* timeSignature = dynamic_cast<TimeSignature*>(e);
            
            for (auto instrument : song->instruments)
            {
                for (auto staff : instrument->staves)
                {
                    if (staff->csStaff)
                    {
                        int i = 0;
                        for (auto measure : staff->csStaff->measures)
                        {
                            SystemMeasure* systemMeasure = song->systemMeasures[i].get();

                            if (systemMeasure->timeSignature.get() == timeSignature)
                            {
                                systemMeasure->timeSignature = nullptr;
                                measure->showTimeSignature = false;
                            }

                            i++;
                        }
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

    Update();
}

void Editor::UpdateLyricProperties(CSLyric* lyric)
{
    nlohmann::ordered_json jsonObject;

    AddTextToJson(jsonObject, "Main", "text", "Text", lyric->lyricText.text);
    AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", lyric->beatPosition);
    AddFloatToJson(jsonObject, "Main", "duration", "Duration", lyric->duration);
    AddBoolToJson(jsonObject, "Main", "startsPickup", "Pickup To Next Measure", lyric->startsPickup);

    AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)lyric->lyricText.fontWeight - 1));
    AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)lyric->lyricText.fontStyle - 1));
    AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", lyric->lyricText.fontSize.size);
    AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", lyric->lyricText.color.color);

    AddFloatToJson(jsonObject, "Position", "posX", "Position X", lyric->position.x);
    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", lyric->position.y);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::UpdateChordProperties(CSChord* chord)
{
    nlohmann::ordered_json jsonObject;

    AddTextToJson(jsonObject, "Main", "chordName", "Chord Name", chord->chordSymbol.GetChordNameAsStandardString());
    AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", chord->beatPosition);
    AddFloatToJson(jsonObject, "Main", "duration", "Duration", chord->duration);

    AddFloatToJson(jsonObject, "Position", "posX", "Position X", chord->position.x);
    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", chord->position.y);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::UpdateMeasureProperties(CSMeasure* measure)
{
    int measureIndex = measure->GetMeasureIndex();
            
    nlohmann::ordered_json jsonObject;

    AddFloatToJson(jsonObject, "Main", "width", "Width", measure->width);
    AddBoolToJson(jsonObject, "Main", "pageBreak", "Page Break", song->systemMeasures[measureIndex]->pageBreak);
    AddBoolToJson(jsonObject, "Main", "systemBreak", "System Break", song->systemMeasures[measureIndex]->systemBreak);
    AddBoolToJson(jsonObject, "Main", "isPickupMeasure", "Is Pickup Measure", song->systemMeasures[measureIndex]->isPickupMeasure);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::UpdateCreditProperties(Credit* credit)
{
    nlohmann::ordered_json jsonObject;

    AddTextToJson(jsonObject, "Main", "text", "Text", credit->words.text);
    AddIntToJson(jsonObject, "Main", "pageNumber", "Page", credit->pageNumber);

    AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)credit->words.fontWeight - 1));
    AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)credit->words.fontStyle - 1));
    AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", credit->words.fontSize.size);
    AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", credit->words.color.color);

    AddFloatToJson(jsonObject, "Position", "posX", "Position X", credit->words.positionX);
    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", credit->words.positionY);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::UpdateTextDirectionProperties(TextDirection* direction)
{
    nlohmann::ordered_json jsonObject;

    AddTextToJson(jsonObject, "Main", "text", "Text", direction->text.text);

    AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)direction->text.fontWeight - 1));
    AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)direction->text.fontStyle - 1));
    AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", direction->text.fontSize.size);
    AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", direction->text.color.color);

    AddFloatToJson(jsonObject, "Position", "posX", "Position X", direction->position.x);
    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", direction->position.y);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::UpdateDisplayConstantsProperties()
{
    nlohmann::ordered_json jsonObject;

    AddFloatToJson(jsonObject, "Lyrics", "fontSize", "Font Size", song->settings.displayConstants.lyricFontSize.size);
    AddFloatToJson(jsonObject, "Lyrics", "lyricPositionY", "Lyric Y Position", song->settings.displayConstants.lyricPositionY);
    AddFloatToJson(jsonObject, "Lyrics", "lyricSpaceWidth", "Lyric Spacing", song->settings.displayConstants.lyricSpaceWidth);

    AddFloatToJson(jsonObject, "Other", "chordMarginFromBarline", "Chord Margin From Barline", song->settings.displayConstants.chordMarginFromBarline);
    AddFloatToJson(jsonObject, "Other", "beatWidth", "Beat Width", song->settings.displayConstants.beatWidth);
    AddFloatToJson(jsonObject, "Other", "chordPositionY", "Chord Y Position", song->settings.displayConstants.chordPositionY);
    AddFloatToJson(jsonObject, "Other", "minimumMeasureWidth", "Minimum Measure Width", song->settings.displayConstants.minimumMeasureWidth);
    AddBoolToJson(jsonObject, "Other", "displayReminderPickupLyrics", "Display Reminder Pickup Lyrics", song->settings.displayConstants.displayReminderPickupLyrics);
    AddFloatToJson(jsonObject, "Other", "measureBarlineHeight", "Measure Height", song->settings.displayConstants.measureBarlineHeight);
    AddFloatToJson(jsonObject, "Other", "chordFontSize", "Chord Font Size", song->settings.displayConstants.chordFontSize.size);

    AddFloatToJson(jsonObject, "Systems", "systemDistance", "System Distance", song->settings.displayConstants.systemLayout.systemDistance);
    AddFloatToJson(jsonObject, "Systems", "firstPageTopSystemDistance", "First Page: System Distance From Top", song->settings.displayConstants.systemLayout.firstPageTopSystemDistance);
    AddFloatToJson(jsonObject, "Systems", "topSystemDistance", "System Distance From Top", song->settings.displayConstants.systemLayout.topSystemDistance);
    AddFloatToJson(jsonObject, "Pages", "topMargin", "Top Margin", song->settings.displayConstants.topMargin);
    AddFloatToJson(jsonObject, "Pages", "bottomMargin", "Bottom Margin", song->settings.displayConstants.bottomMargin);
    AddFloatToJson(jsonObject, "Pages", "leftMargin", "Left Margin", song->settings.displayConstants.leftMargin);
    AddFloatToJson(jsonObject, "Pages", "rightMargin", "Right Margin", song->settings.displayConstants.rightMargin);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::UpdateTimeSignatureProperties(TimeSignature* timeSignature)
{
    nlohmann::ordered_json jsonObject;

    AddIntToJson(jsonObject, "Main", "notes", "Top", timeSignature->notes);
    AddIntToJson(jsonObject, "Main", "noteType", "Bottom", timeSignature->noteType);

    AddFloatToJson(jsonObject, "Position", "posX", "Position X", timeSignature->position.x);
    AddFloatToJson(jsonObject, "Position", "posY", "Position Y", timeSignature->position.y);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::SetTimeSignatureProperties(TimeSignature* timeSignature, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    timeSignature->notes = jsonObject["Main"]["notes"]["value"];
    timeSignature->noteType = jsonObject["Main"]["noteType"]["value"];

    timeSignature->position.x = jsonObject["Position"]["posX"]["value"];
    timeSignature->position.y = jsonObject["Position"]["posY"]["value"];
}

void Editor::SetSelection(std::vector<BaseElement*> newSelected)
{
    // deselect elements
    for (int i = selectedElements.size() - 1; i >= 0; i--)
    {
        BaseElement* element = selectedElements[i];
        if (element->elementType == BaseElement::ElementType::CSLyric)
        {
            CSLyric* lyric = dynamic_cast<CSLyric*>(element);
            lyric->lyricText.selectedColor = 0x000000FF;

            if (lyric->lyricText.text == "")
            {
                lyric->Delete();
            }
        }
        else if (element->elementType == BaseElement::ElementType::CSChord)
        {
            CSChord* chord = dynamic_cast<CSChord*>(element);
            chord->chordSymbol.selectedColor = 0x000000FF;
        }
        element->selectedColor = 0x000000FF;

        selectedElements.erase(selectedElements.begin() + i);
    }

    bool noSelection = false;
    if (!newSelected.empty())
    {
        if (!newSelected[0])
            noSelection = true;
    }
    else
        noSelection = true;

    // select elements
    if (!noSelection)
    {
        selectedElements = newSelected;
        
        BaseElement* selectedElement = selectedElements[0]; // TODO: revise when implementing multi-select

        selectedElement->selectedColor = 0x3333EEFF;

        if (selectedElement->elementType == BaseElement::ElementType::CSLyric)
        {
            CSLyric* lyric = dynamic_cast<CSLyric*>(selectedElement);
            lyric->lyricText.selectedColor = 0x3333EEFF;

            UpdateLyricProperties(lyric);
        }
        else if (selectedElement->elementType == BaseElement::ElementType::CSChord)
        {
            CSChord* chord = dynamic_cast<CSChord*>(selectedElement);
            chord->chordSymbol.selectedColor = 0x3333EEFF;
            
            UpdateChordProperties(chord);
        }
        else if (selectedElement->elementType == BaseElement::ElementType::CSMeasure)
        {
            CSMeasure* measure = dynamic_cast<CSMeasure*>(selectedElement);

            UpdateMeasureProperties(measure);
        }
        else if (selectedElement->elementType == BaseElement::ElementType::Credit)
        {
            Credit* credit = dynamic_cast<Credit*>(selectedElement);

            UpdateCreditProperties(credit);
        }
        else if (selectedElement->elementType == BaseElement::ElementType::TextDirection)
        {
            TextDirection* direction = dynamic_cast<TextDirection*>(selectedElement);

            UpdateTextDirectionProperties(direction);
        }
        else if (selectedElement->elementType == BaseElement::ElementType::TimeSignature)
        {
            TimeSignature* timeSignature = dynamic_cast<TimeSignature*>(selectedElement);

            UpdateTimeSignatureProperties(timeSignature);
        }
        else
        {
            LOGE("Did not recognize element type");
        }
    }
    else
    {
        UpdateDisplayConstantsProperties();
        //Callbacks::GetInstance().UpdateProperties("{}");
    }
}

void Editor::Update()
{
    song->OnUpdate();
    song->CalculateSystems();
    song->CalculateSystemPositionsAndPageBreaks();

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);
}