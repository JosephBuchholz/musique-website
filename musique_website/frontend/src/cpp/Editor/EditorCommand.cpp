#include "EditorCommand.h"

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

