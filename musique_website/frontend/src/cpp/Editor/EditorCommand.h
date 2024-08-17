#pragma once

#include <memory>

#include "../MusicData/ChordSheetData/CSChord.h"
#include "../MusicData/ChordSheetData/CSMeasure.h"
#include "../MusicData/Song.h"

class EditorCommand
{
public:
    virtual ~EditorCommand() {};

    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

class AddChordCommand : public EditorCommand
{
public:
    
    virtual void Execute() override;
    virtual void Undo() override;

public:
    std::shared_ptr<CSChord> chord;
    CSMeasure* measure;
};

class DeleteChordCommand : public EditorCommand
{
public:
    
    virtual void Execute() override;
    virtual void Undo() override;

public:
    std::shared_ptr<CSChord> chord;
    CSMeasure* measure;
};

class AddLyricCommand : public EditorCommand
{
public:
    
    virtual void Execute() override;
    virtual void Undo() override;

public:
    std::shared_ptr<CSLyric> lyric;
    CSMeasure* measure;
};

class DeleteLyricCommand : public EditorCommand
{
public:
    
    virtual void Execute() override;
    virtual void Undo() override;

public:
    std::shared_ptr<CSLyric> lyric;
    CSMeasure* measure;
};


class AddMeasureCommand : public EditorCommand
{
public:
    AddMeasureCommand(std::shared_ptr<CSMeasure> measure, std::shared_ptr<SystemMeasure> systemMeasure, int measureIndex, Song* song)
        : measure(measure), systemMeasure(systemMeasure), measureIndex(measureIndex), song(song) {}
    
    virtual void Execute() override;
    virtual void Undo() override;

private:
    std::shared_ptr<CSMeasure> measure;
    std::shared_ptr<SystemMeasure> systemMeasure;
    int measureIndex;
    Song* song;
};

class DeleteMeasureCommand : public EditorCommand
{
public:
    DeleteMeasureCommand(std::shared_ptr<CSMeasure> measure, std::shared_ptr<SystemMeasure> systemMeasure, int measureIndex, Song* song)
        : measure(measure), systemMeasure(systemMeasure), measureIndex(measureIndex), song(song) {}
    
    virtual void Execute() override;
    virtual void Undo() override;

private:
    std::shared_ptr<CSMeasure> measure;
    std::shared_ptr<SystemMeasure> systemMeasure;
    int measureIndex;
    Song* song;
};


class AddTextDirectionCommand : public EditorCommand
{
public:
    AddTextDirectionCommand(std::shared_ptr<TextDirection> direction, CSMeasure* measure)
        : direction(direction), measure(measure) {}
    
    virtual void Execute() override;
    virtual void Undo() override;

private:
    std::shared_ptr<TextDirection> direction;
    CSMeasure* measure;
};

class DeleteTextDirectionCommand : public EditorCommand
{
public:
    DeleteTextDirectionCommand(std::shared_ptr<TextDirection> direction, CSMeasure* measure)
        : direction(direction), measure(measure) {}
    
    virtual void Execute() override;
    virtual void Undo() override;

private:
    std::shared_ptr<TextDirection> direction;
    CSMeasure* measure;
};


class ChangeChordDurationCommand : public EditorCommand
{
public:
    ChangeChordDurationCommand(CSChord* chord, NoteValue noteValue, bool isDotted)
        : chord(chord), noteValue(noteValue), isDotted(isDotted) {}
    
    virtual void Execute() override;
    virtual void Undo() override;

private:
    CSChord* chord;
    NoteValue noteValue;
    bool isDotted;
};