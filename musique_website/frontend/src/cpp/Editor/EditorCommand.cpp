#include "EditorCommand.h"

void AddChordCommand::Execute()
{
    LOGD("Add chord command: %f", chord->beatPosition);
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
    LOGD("Delete chord command: %f", chord->beatPosition);
    /*std::shared_ptr<CSChord> chordCopy = std::make_shared<CSChord>();
    chordCopy->chordSymbol = chord->chordSymbol;
    chordCopy->duration = chord->duration;
    chordCopy->beatPosition = chord->beatPosition;
    chordCopy->beatPositionInSong = chord->beatPositionInSong;
    //chordCopy->noteHead = std::move(chord->noteHead);
    //chordCopy->noteStem = std::move(chord->noteStem);
    //chordCopy->noteFlag = std::move(chord->noteFlag);
    //chordCopy->augDot = std::move(chord->augDot);
    chordCopy->parent = chord->parent;*/

    chord->Delete();

    //chord = chordCopy;
}

void DeleteChordCommand::Undo()
{
    AddChordCommand command;
    command.chord = chord;
    command.measure = measure;
    command.Execute();
}