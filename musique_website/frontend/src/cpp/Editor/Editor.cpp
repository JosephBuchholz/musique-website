#include "Editor.h"

#include <sstream>

#include "Properties.h"
#include "../Callbacks.h"

#include "JsonHelper.h"

std::shared_ptr<BaseElement> Editor::FindSelectedElement(Vec2<float> point)
{
    for (const auto& credit : song->credits)
    {
        BoundingBox bb = credit->GetBoundingBox();
        bb.position += musicRenderer->pagePositions[credit->pageNumber - 1];

        if (bb.DoesOverlapWithPoint(point))
        {
            return credit;
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
                        std::shared_ptr<CSMeasure> measure = staff->csStaff->measures[m];

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
                                //return measure->timeSignature;
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
                                return lyric;
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
                                    return lyric;
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
                                return chord;
                            }
                        }

                        for (const auto& direction : measure->textDirections)
                        {
                            BoundingBox bb = direction->GetBoundingBoxRelativeToParent();
                            bb.position.x += measurePositionX + measure->pickupWidth;
                            bb.position += musicRenderer->systemPositions[systemIndex];

                            if (bb.DoesOverlapWithPoint(point))
                            {
                                return direction;
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

            std::shared_ptr<BaseElement> selectedElement = FindSelectedElement(point);

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
        else if (event.keyCode == KeyboardEvent::KeyCode::Control)
        {
            controlPressed = true;
        }
        else if (event.keyCode == KeyboardEvent::KeyCode::z)
        {
            if (controlPressed)
            {
                UndoCommand();
                Update();
            }
        }
        else if (event.keyCode == KeyboardEvent::KeyCode::y)
        {
            if (controlPressed)
            {
                RedoCommand();
                Update();
            }
        }
    }
    else if (event.eventType == KeyboardEvent::KeyboardEventType::Up)
    {
        if (event.keyCode == KeyboardEvent::KeyCode::Control)
        {
            controlPressed = false;
        }
    }

    return true;
}

bool Editor::OnTextFieldEvent(int id, const std::string& input)
{
    for (auto e : selectedElements)
    {
        CSLyric* lyric = dynamic_cast<CSLyric*>(e.get());
        lyric->lyricText.text = input;
    }

    musicRenderer->updateRenderData = true;
    musicRenderer->layoutCalculated = false;
    musicRenderer->Render(song, song->settings);

    return true;
}

void Editor::OnPropertiesUpdated(const std::string& propertiesString)
{
    for (auto e : selectedElements)
    {
        if (e->elementType == BaseElement::ElementType::CSLyric)
        {
            CSLyric* lyric = dynamic_cast<CSLyric*>(e.get());
            if (lyric->lyricText.text == "")
            {
                UndoCommand(false); // undo/get rid of temp lyric
                lyric->lyricText.text = "delete"; // so that this lyric does accidentally tigger undo again (TODO: this is kinda dumb, should get rid of this)

                // add new lyric with updated properties
                std::unique_ptr<AddLyricCommand> command = std::make_unique<AddLyricCommand>();

                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                SetLyricProperties(newLyric.get(), propertiesString);

                command->lyric = newLyric;
                command->measure = dynamic_cast<CSMeasure*>(lyric->parent);

                ExecuteCommand(std::move(command));

                SetSelection({ newLyric });

                break;
            }
        }

        std::unique_ptr<ChangePropertiesCommand> command = std::make_unique<ChangePropertiesCommand>();
        command->editor = this;
        command->element = e;
        command->properties = propertiesString;
        ExecuteCommand(std::move(command));
        
        if (e->elementType == BaseElement::ElementType::CSLyric)
        {
            CSLyric* lyric = dynamic_cast<CSLyric*>(e.get());

            json jsonObject = json::parse(propertiesString);
            std::string text = jsonObject["Main"]["text"]["value"];

            if (text.back() == ' ')
            {
                text.pop_back();
                if (lyric->lyricText.text == text)
                {
                    UndoCommand(false);
                }

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

                std::unique_ptr<AddLyricCommand> command = std::make_unique<AddLyricCommand>();

                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                newLyric->lyricText.text = "";
                newLyric->beatPosition = lyric->beatPosition;

                command->lyric = newLyric;
                command->measure = measure;

                ExecuteCommand(std::move(command));

                SetSelection({ newLyric });
            }
        }
    }

    if (selectedElements.empty())
    {
        json jsonObject = json::parse(propertiesString);

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
        BaseElement* e = selectedElements[0].get();
        if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            CSMeasure* measure = dynamic_cast<CSMeasure*>(e);

            if (id == 0)
            {
                std::unique_ptr<AddLyricCommand> command = std::make_unique<AddLyricCommand>();

                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                newLyric->lyricText.text = "none";

                command->lyric = newLyric;
                command->measure = measure;

                ExecuteCommand(std::move(command));
            }
            else if (id == 1)
            {
                std::unique_ptr<AddChordCommand> command = std::make_unique<AddChordCommand>();

                std::shared_ptr<CSChord> newChord = std::make_shared<CSChord>();
                newChord->chordSymbol = Chord::CreateChordFromString("C");
                newChord->chordSymbol.CalculateChordName(song->settings);
                newChord->noteHead = std::make_unique<NoteHead>();
                newChord->noteHead->noteDuration = NoteValue::Quarter;
                newChord->duration = 1.0f;
                newChord->noteStem = std::make_unique<NoteStem>();
                newChord->noteStem->stemType = NoteStem::StemType::Up;
                command->chord = newChord;
                command->measure = measure;

                ExecuteCommand(std::move(command));
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
                CSMeasure* measure = nullptr;
                for (auto& instrument : song->instruments)
                {
                    for (auto& staff : instrument->staves)
                    {
                        if (staff->csStaff)
                        {
                            for (auto& m : staff->csStaff->measures)
                            {
                                for (auto& c : m->chords)
                                {
                                    if (c.get() == chord)
                                    {
                                        measure = m.get();
                                    }
                                }
                            }
                        }
                    }
                }

                ASSERT(measure);

                std::unique_ptr<AddLyricCommand> command = std::make_unique<AddLyricCommand>();

                std::shared_ptr<CSLyric> newLyric = std::make_shared<CSLyric>();
                newLyric->lyricText.text = "none";
                newLyric->beatPosition = chord->beatPosition;

                command->lyric = newLyric;
                command->measure = measure;

                ExecuteCommand(std::move(command));

            }
            else if (id >= 10 && id <= 15) // change duration
            {
                if (id == 10)
                {
                    chord->noteHead->noteDuration = NoteValue::Whole;
                    chord->noteStem = nullptr;
                    chord->noteFlag = nullptr;
                    chord->augDot = nullptr;
                    chord->duration = 4.0f;
                }
                else if (id == 11)
                {
                    chord->noteHead->noteDuration = NoteValue::Half;
                    if (!chord->noteStem)
                    {
                        chord->noteStem = std::make_unique<NoteStem>();
                        chord->noteStem->stemType = NoteStem::StemType::Up;
                    }
                    chord->noteFlag = nullptr;
                    chord->augDot = nullptr;
                    chord->duration = 2.0f;
                }
                else if (id == 12)
                {
                    chord->noteHead->noteDuration = NoteValue::Quarter;
                    if (!chord->noteStem)
                    {
                        chord->noteStem = std::make_unique<NoteStem>();
                        chord->noteStem->stemType = NoteStem::StemType::Up;
                    }
                    chord->noteFlag = nullptr;
                    chord->augDot = nullptr;
                    chord->duration = 1.0f;
                }
                else if (id == 13)
                {
                    chord->noteHead->noteDuration = NoteValue::Eighth;
                    if (!chord->noteStem)
                    {
                        chord->noteStem = std::make_unique<NoteStem>();
                        chord->noteStem->stemType = NoteStem::StemType::Up;
                    }

                    chord->noteFlag = std::make_unique<NoteFlag>();
                    chord->noteFlag->type = NoteFlag::Type::Up;
                    chord->noteFlag->noteDuration = NoteValue::Eighth;

                    chord->augDot = nullptr;
                    chord->duration = 0.5f;
                }
                else if (id == 14)
                {
                    chord->noteHead->noteDuration = NoteValue::Sixteenth;
                    if (!chord->noteStem)
                    {
                        chord->noteStem = std::make_unique<NoteStem>();
                        chord->noteStem->stemType = NoteStem::StemType::Up;
                    }

                    chord->noteFlag = std::make_unique<NoteFlag>();
                    chord->noteFlag->type = NoteFlag::Type::Up;
                    chord->noteFlag->noteDuration = NoteValue::Sixteenth;

                    chord->augDot = nullptr;
                    chord->duration = 0.25f;
                }
                else if (id == 15)
                {
                    if (chord->augDot)
                    {
                        chord->augDot = nullptr;
                        chord->duration *= 2.0f/3.0f;
                    }
                    else
                    {
                        chord->augDot = std::make_unique<AugmentationDot>();
                        chord->duration *= 3.0f/2.0f;
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

    if (id == 17)
        UndoCommand();
    else if (id == 18)
        RedoCommand();

    Update();
}

void Editor::OnDeleteSelected()
{
    for (auto e : selectedElements)
    {
        if (e->elementType == BaseElement::ElementType::CSMeasure)
        {
            CSMeasure* measure = dynamic_cast<CSMeasure*>(e.get());
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
            TimeSignature* timeSignature = dynamic_cast<TimeSignature*>(e.get());
            
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
        else if (e->elementType == BaseElement::ElementType::CSChord)
        {
            std::shared_ptr<CSChord> chord = std::dynamic_pointer_cast<CSChord>(e);

            std::unique_ptr<DeleteChordCommand> command = std::make_unique<DeleteChordCommand>();

            command->chord = chord;
            
            if (chord->parent->elementType == BaseElement::ElementType::CSMeasure)
            {
                CSMeasure* parentMeasure = dynamic_cast<CSMeasure*>(chord->parent);
                command->measure = parentMeasure;
            }

            ExecuteCommand(std::move(command));
        }
        else if (e->elementType == BaseElement::ElementType::CSLyric)
        {
            std::shared_ptr<CSLyric> lyric = std::dynamic_pointer_cast<CSLyric>(e);

            std::unique_ptr<DeleteLyricCommand> command = std::make_unique<DeleteLyricCommand>();

            command->lyric = lyric;
            
            if (lyric->parent->elementType == BaseElement::ElementType::CSMeasure)
            {
                CSMeasure* parentMeasure = dynamic_cast<CSMeasure*>(lyric->parent);
                command->measure = parentMeasure;
            }

            ExecuteCommand(std::move(command));
        }
        else
        {
            e->Delete();
        }
    }

    selectedElements.clear();

    Update();
}

std::string Editor::GetProperties(const BaseElement* element) const
{
    switch (element->elementType)
    {
        case BaseElement::ElementType::CSLyric:
        {
            return GetLyricProperties(dynamic_cast<const CSLyric*>(element));
        }
        case BaseElement::ElementType::CSChord:
        {
            return GetChordProperties(dynamic_cast<const CSChord*>(element));
        }
        case BaseElement::ElementType::CSMeasure:
        {
            return GetMeasureProperties(dynamic_cast<const CSMeasure*>(element));
        }
        case BaseElement::ElementType::Credit:
        {
            return GetCreditProperties(dynamic_cast<const Credit*>(element));
        }
        case BaseElement::ElementType::TextDirection:
        {
            return GetTextDirectionProperties(dynamic_cast<const TextDirection*>(element));
        }
        case BaseElement::ElementType::TimeSignature:
        {
            return GetTimeSignatureProperties(dynamic_cast<const TimeSignature*>(element));
        }
        default: break;
    }

    ASSERT(false);
    return "";
}

void Editor::SetProperties(BaseElement* element, const std::string& propertiesString)
{
    switch (element->elementType)
    {
        case BaseElement::ElementType::CSLyric:
        {
            SetLyricProperties(dynamic_cast<CSLyric*>(element), propertiesString); break;
        }
        case BaseElement::ElementType::CSChord:
        {
            SetChordProperties(dynamic_cast<CSChord*>(element), propertiesString); break;
        }
        case BaseElement::ElementType::CSMeasure:
        {
            SetMeasureProperties(dynamic_cast<CSMeasure*>(element), propertiesString); break;
        }
        case BaseElement::ElementType::Credit:
        {
            SetCreditProperties(dynamic_cast<Credit*>(element), propertiesString); break;
        }
        case BaseElement::ElementType::TextDirection:
        {
            SetTextDirectionProperties(dynamic_cast<TextDirection*>(element), propertiesString); break;
        }
        case BaseElement::ElementType::TimeSignature:
        {
            SetTimeSignatureProperties(dynamic_cast<TimeSignature*>(element), propertiesString); break;
        }
        default:
        {
            ASSERT(false);
            break;
        }
    }
}

void Editor::UpdateDisplayConstantsProperties()
{
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddFloatToJson(jsonObject, "Lyrics", "fontSize", "Font Size", song->settings.displayConstants.lyricFontSize.size);
    JsonHelper::AddFloatToJson(jsonObject, "Lyrics", "lyricPositionY", "Lyric Y Position", song->settings.displayConstants.lyricPositionY);
    JsonHelper::AddFloatToJson(jsonObject, "Lyrics", "lyricSpaceWidth", "Lyric Spacing", song->settings.displayConstants.lyricSpaceWidth);

    JsonHelper::AddFloatToJson(jsonObject, "Other", "chordMarginFromBarline", "Chord Margin From Barline", song->settings.displayConstants.chordMarginFromBarline);
    JsonHelper::AddFloatToJson(jsonObject, "Other", "beatWidth", "Beat Width", song->settings.displayConstants.beatWidth);
    JsonHelper::AddFloatToJson(jsonObject, "Other", "chordPositionY", "Chord Y Position", song->settings.displayConstants.chordPositionY);
    JsonHelper::AddFloatToJson(jsonObject, "Other", "minimumMeasureWidth", "Minimum Measure Width", song->settings.displayConstants.minimumMeasureWidth);
    JsonHelper::AddBoolToJson(jsonObject, "Other", "displayReminderPickupLyrics", "Display Reminder Pickup Lyrics", song->settings.displayConstants.displayReminderPickupLyrics);
    JsonHelper::AddFloatToJson(jsonObject, "Other", "measureBarlineHeight", "Measure Height", song->settings.displayConstants.measureBarlineHeight);
    JsonHelper::AddFloatToJson(jsonObject, "Other", "chordFontSize", "Chord Font Size", song->settings.displayConstants.chordFontSize.size);

    JsonHelper::AddFloatToJson(jsonObject, "Systems", "systemDistance", "System Distance", song->settings.displayConstants.systemLayout.systemDistance);
    JsonHelper::AddFloatToJson(jsonObject, "Systems", "firstPageTopSystemDistance", "First Page: System Distance From Top", song->settings.displayConstants.systemLayout.firstPageTopSystemDistance);
    JsonHelper::AddFloatToJson(jsonObject, "Systems", "topSystemDistance", "System Distance From Top", song->settings.displayConstants.systemLayout.topSystemDistance);
    JsonHelper::AddFloatToJson(jsonObject, "Pages", "topMargin", "Top Margin", song->settings.displayConstants.topMargin);
    JsonHelper::AddFloatToJson(jsonObject, "Pages", "bottomMargin", "Bottom Margin", song->settings.displayConstants.bottomMargin);
    JsonHelper::AddFloatToJson(jsonObject, "Pages", "leftMargin", "Left Margin", song->settings.displayConstants.leftMargin);
    JsonHelper::AddFloatToJson(jsonObject, "Pages", "rightMargin", "Right Margin", song->settings.displayConstants.rightMargin);

    Callbacks::GetInstance().UpdateProperties(jsonObject.dump());
}

void Editor::SetSelection(std::vector<std::shared_ptr<BaseElement>> newSelected)
{
    // deselect elements
    for (int i = selectedElements.size() - 1; i >= 0; i--)
    {
        BaseElement* element = selectedElements[i].get();
        if (element->elementType == BaseElement::ElementType::CSLyric)
        {
            CSLyric* lyric = dynamic_cast<CSLyric*>(element);
            lyric->lyricText.selectedColor = 0x000000FF;

            if (lyric->lyricText.text == "")
            {
                UndoCommand(false);
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
        
        std::shared_ptr<BaseElement> selectedElement = selectedElements[0]; // TODO: revise when implementing multi-select

        selectedElement->selectedColor = 0x3333EEFF;

        std::string propertiesString = GetProperties(selectedElement.get());
        Callbacks::GetInstance().UpdateProperties(propertiesString);

        if (selectedElement->elementType == BaseElement::ElementType::CSLyric)
        {
            CSLyric* lyric = dynamic_cast<CSLyric*>(selectedElement.get());
            lyric->lyricText.selectedColor = 0x3333EEFF;
        }
        else if (selectedElement->elementType == BaseElement::ElementType::CSChord)
        {
            CSChord* chord = dynamic_cast<CSChord*>(selectedElement.get());
            chord->chordSymbol.selectedColor = 0x3333EEFF;
        }
    }
    else
    {
        UpdateDisplayConstantsProperties();
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

void Editor::ExecuteCommand(std::unique_ptr<EditorCommand> command)
{
    command->Execute();
    executedCommandStack.push_back(std::move(command));
    redoCommandStack.clear();
}

void Editor::UndoCommand(bool addToRedoStack)
{
    if (!executedCommandStack.empty())
    {
        std::unique_ptr<EditorCommand> command = std::move(executedCommandStack.back());
        executedCommandStack.pop_back();

        command->Undo();

        if (addToRedoStack)
            redoCommandStack.push_back(std::move(command));
    }
}

void Editor::RedoCommand()
{
    if (!redoCommandStack.empty())
    {
        std::unique_ptr<EditorCommand> command = std::move(redoCommandStack.back());
        redoCommandStack.pop_back();

        command->Execute();

        executedCommandStack.push_back(std::move(command));
    }
}

void ChangePropertiesCommand::Execute()
{
    originalProperties = editor->GetProperties(element.get());
    editor->SetProperties(element.get(), properties);
}

void ChangePropertiesCommand::Undo()
{
    properties = editor->GetProperties(element.get());
    editor->SetProperties(element.get(), originalProperties);
}