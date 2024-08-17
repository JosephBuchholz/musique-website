#include "EditorCommand.h"

#include "../MusicData/ChordSheetData/CSStaff.h"

void AddChordCommand::Execute()
{
    chord->selectedColor.color = 0x000000FF;
    measure->chords.push_back(chord);
}

void AddChordCommand::Undo()
{
    DeleteChordCommand command;
    command.chord = chord;
    command.measure = measure;
    command.Execute();
}

void DeleteChordCommand::Execute()
{
    chord->Delete();
}

void DeleteChordCommand::Undo()
{
    AddChordCommand command;
    command.chord = chord;
    command.measure = measure;
    command.Execute();
}


void AddLyricCommand::Execute()
{
    lyric->selectedColor.color = 0x000000FF;
    measure->lyrics.push_back(lyric);
}

void AddLyricCommand::Undo()
{
    DeleteLyricCommand command;
    command.lyric = lyric;
    command.measure = measure;
    command.Execute();
}

void DeleteLyricCommand::Execute()
{
    lyric->Delete();
}

void DeleteLyricCommand::Undo()
{
    AddLyricCommand command;
    command.lyric = lyric;
    command.measure = measure;
    command.Execute();
}


void AddMeasureCommand::Execute()
{
    for (auto& instrument : song->instruments)
    {
        for (auto& staff : instrument->staves)
        {
            if (staff->csStaff)
            {
                measure->parent = staff->csStaff;
                staff->csStaff->measures.insert(staff->csStaff->measures.begin() + measureIndex, measure);
                song->systemMeasures.insert(song->systemMeasures.begin() + measureIndex, systemMeasure);
            }
        }
    }
}

void AddMeasureCommand::Undo()
{
    DeleteMeasureCommand command(measure, systemMeasure, measureIndex, song);
    command.Execute();
}

void DeleteMeasureCommand::Execute()
{
    if (measure->parent)
    {
        if (measure->parent->elementType == BaseElement::ElementType::CSStaff)
        {
            std::shared_ptr<CSStaff> staff = std::dynamic_pointer_cast<CSStaff>(measure->parent);

            staff->measures.erase(staff->measures.begin() + measureIndex);
            song->systemMeasures.erase(song->systemMeasures.begin() + measureIndex);
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

void DeleteMeasureCommand::Undo()
{
    AddMeasureCommand command(measure, systemMeasure, measureIndex, song);
    command.Execute();
}


void AddTextDirectionCommand::Execute()
{
    direction->selectedColor.color = 0x000000FF;
    measure->textDirections.push_back(direction);
}

void AddTextDirectionCommand::Undo()
{
    DeleteTextDirectionCommand command(direction, measure);
    command.Execute();
}

void DeleteTextDirectionCommand::Execute()
{
    direction->Delete();
}

void DeleteTextDirectionCommand::Undo()
{
    AddTextDirectionCommand command(direction, measure);
    command.Execute();
}


void ChangeChordDurationCommand::Execute()
{
    NoteValue oldNoteValue = chord->noteHead->noteDuration;
    bool oldIsDotted = (chord->augDot != nullptr);

    chord->noteHead->noteDuration = noteValue;

    if (noteValue == NoteValue::Whole)
    {
        chord->noteStem = nullptr;
        chord->noteFlag = nullptr;
        chord->duration = 4.0f;
    }
    else if (noteValue == NoteValue::Half)
    {
        if (!chord->noteStem)
        {
            chord->noteStem = std::make_unique<NoteStem>();
            chord->noteStem->stemType = NoteStem::StemType::Up;
        }

        chord->noteFlag = nullptr;
        chord->duration = 2.0f;
    }
    else if (noteValue == NoteValue::Quarter)
    {
        if (!chord->noteStem)
        {
            chord->noteStem = std::make_unique<NoteStem>();
            chord->noteStem->stemType = NoteStem::StemType::Up;
        }

        chord->noteFlag = nullptr;
        chord->duration = 1.0f;
    }
    else if (noteValue == NoteValue::Eighth)
    {
        if (!chord->noteStem)
        {
            chord->noteStem = std::make_unique<NoteStem>();
            chord->noteStem->stemType = NoteStem::StemType::Up;
        }

        chord->noteFlag = std::make_unique<NoteFlag>();
        chord->noteFlag->type = NoteFlag::Type::Up;
        chord->noteFlag->noteDuration = NoteValue::Eighth;

        chord->duration = 0.5f;
    }
    else if (noteValue == NoteValue::Sixteenth)
    {
        if (!chord->noteStem)
        {
            chord->noteStem = std::make_unique<NoteStem>();
            chord->noteStem->stemType = NoteStem::StemType::Up;
        }

        chord->noteFlag = std::make_unique<NoteFlag>();
        chord->noteFlag->type = NoteFlag::Type::Up;
        chord->noteFlag->noteDuration = NoteValue::Sixteenth;

        chord->duration = 0.25f;
    }
    

    if (!isDotted)
    {
        chord->augDot = nullptr;
    }
    else
    {
        chord->augDot = std::make_unique<AugmentationDot>();
        chord->duration *= 3.0f/2.0f;
    }

    noteValue = oldNoteValue;
    isDotted = oldIsDotted;
}

void ChangeChordDurationCommand::Undo()
{
    Execute();
}