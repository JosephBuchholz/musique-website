#pragma once

#include <memory>

#include "../MusicData/Song.h"
#include "../Events/InputEvent.h"
#include "../Rendering/MusicRenderer.h"
#include "../MusicData/Measures/TimeSignature.h"

class Editor
{
public:

    bool OnPointerEvent(const PointerEvent& event);
    bool OnKeyboardEvent(const KeyboardEvent& event);
    bool OnTextFieldEvent(int id, const std::string& input);
    void OnPropertiesUpdated(const std::string& propertiesString);
    BaseElement* FindSelectedElement(Vec2<float> point);
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

    void SetSelection(std::vector<BaseElement*> newSelected);

    void Update();

public:
    std::shared_ptr<Song> song = nullptr;
    std::shared_ptr<MusicRenderer> musicRenderer = nullptr;

    std::vector<BaseElement*> selectedElements;

    bool pointerIsDown = false;
    Vec2<float> pointerDownPosition = { 0.0f, 0.0f };
};