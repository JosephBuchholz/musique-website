#pragma once

#include <memory>

#include "../MusicData/Song.h"
#include "../Events/InputEvent.h"
#include "../Rendering/MusicRenderer.h"
#include "../MusicData/Measures/TimeSignature.h"
#include "EditorCommand.h"

class Editor
{
public:

    bool OnPointerEvent(const PointerEvent& event);
    bool OnKeyboardEvent(const KeyboardEvent& event);
    bool OnTextFieldEvent(int id, const std::string& input);
    void OnPropertiesUpdated(const std::string& propertiesString);
    std::shared_ptr<BaseElement> FindSelectedElement(Vec2<float> point);
    void OnNewElement(int id);
    void OnDeleteSelected();

private:
    void UpdateLyricProperties(CSLyric* lyric);
    void UpdateChordProperties(CSChord* chord);
    void UpdateMeasureProperties(CSMeasure* measure);
    void UpdateCreditProperties(Credit* credit);
    void UpdateTextDirectionProperties(TextDirection* direction);
    void UpdateTimeSignatureProperties(TimeSignature* timeSignature);
    void SetTimeSignatureProperties(TimeSignature* timeSignature, const std::string& propertiesString);
    void UpdateDisplayConstantsProperties();

    void SetSelection(std::vector<std::shared_ptr<BaseElement>> newSelected);

    void Update();

    void ExecuteCommand(std::unique_ptr<EditorCommand> command);

public:
    std::shared_ptr<Song> song = nullptr;
    std::shared_ptr<MusicRenderer> musicRenderer = nullptr;

    std::vector<std::shared_ptr<BaseElement>> selectedElements;

    std::vector<std::unique_ptr<EditorCommand>> executedCommandStack;
    std::vector<std::unique_ptr<EditorCommand>> redoCommandStack;

    bool pointerIsDown = false;
    Vec2<float> pointerDownPosition = { 0.0f, 0.0f };
};