#pragma once

#include <memory>

#include "../MusicData/ChordSheetData/CSChord.h"
#include "../MusicData/ChordSheetData/CSMeasure.h"

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