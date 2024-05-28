#pragma once

#include <memory>

#include "../MusicData/Song.h"
#include "../Events/InputEvent.h"
#include "../Rendering/MusicRenderer.h"
#include "../MusicData/Measures/TimeSignature.h"
#include "EditorCommand.h"

class Editor;

class ChangePropertiesCommand : public EditorCommand
{
public:
    
    virtual void Execute() override;
    virtual void Undo() override;

public:
    std::shared_ptr<BaseElement> element;
    std::string properties;
    Editor* editor;

private:
    std::string originalProperties;
};

class Editor
{
public:

    bool OnPointerEvent(const PointerEvent& event);
    bool OnKeyboardEvent(const KeyboardEvent& event);
    bool OnTextFieldEvent(int id, const std::string& input);
    void OnPropertiesUpdated(const std::string& propertiesString);
    std::string GetProperties(const BaseElement* element) const;
    void SetProperties(BaseElement* element, const std::string& propertiesString);
    std::shared_ptr<BaseElement> FindSelectedElement(Vec2<float> point);
    void OnNewElement(int id);
    void OnDeleteSelected();

private:
    std::string GetLyricProperties(const CSLyric* lyric) const;
    void SetLyricProperties(CSLyric* lyric, const std::string& propertiesString);

    std::string GetChordProperties(const CSChord* chord) const;
    void SetChordProperties(CSChord* chord, const std::string& propertiesString);

    std::string GetMeasureProperties(const CSMeasure* measure) const;
    void SetMeasureProperties(CSMeasure* measure, const std::string& propertiesString);

    std::string GetCreditProperties(const Credit* credit) const;
    void SetCreditProperties(Credit* credit, const std::string& propertiesString);

    std::string GetTextDirectionProperties(const TextDirection* direction) const;
    void SetTextDirectionProperties(TextDirection* direction, const std::string& propertiesString);

    std::string GetTimeSignatureProperties(const TimeSignature* timeSignature) const;
    void SetTimeSignatureProperties(TimeSignature* timeSignature, const std::string& propertiesString);

    void UpdateDisplayConstantsProperties();

    void SetSelection(std::vector<std::shared_ptr<BaseElement>> newSelected);

    void Update();

    void ExecuteCommand(std::unique_ptr<EditorCommand> command);

    void UndoCommand(bool addToRedoStack = true);
    void RedoCommand();

    void InsertMeasure(int index); /// inserts measure at the given index
    void AppendMeasure();

public:
    std::shared_ptr<Song> song = nullptr;
    std::shared_ptr<MusicRenderer> musicRenderer = nullptr;

    std::vector<std::shared_ptr<BaseElement>> selectedElements;

    std::vector<std::unique_ptr<EditorCommand>> executedCommandStack;
    std::vector<std::unique_ptr<EditorCommand>> redoCommandStack;

    bool pointerIsDown = false;
    Vec2<float> pointerDownPosition = { 0.0f, 0.0f };

    bool controlPressed = false;
};