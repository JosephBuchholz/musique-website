#pragma once

#include <memory>

#include "../MusicData/Song.h"
#include "../Events/InputEvent.h"
#include "../Rendering/MusicRenderer.h"

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
    void UpdateLyricProperties(std::shared_ptr<CSLyric> lyric);
    void UpdateChordProperties(std::shared_ptr<CSChord> chord);
    void UpdateMeasureProperties(std::shared_ptr<CSMeasure> measure);
    void UpdateCreditProperties(std::shared_ptr<Credit> credit);
    void UpdateDisplayConstantsProperties();

    void SetSelection(std::vector<std::shared_ptr<BaseElement>> newSelected);

    void Update();

public:
    std::shared_ptr<Song> song = nullptr;
    std::shared_ptr<MusicRenderer> musicRenderer = nullptr;

    std::vector<std::shared_ptr<BaseElement>> selectedElements;

    bool pointerIsDown = false;
    Vec2<float> pointerDownPosition = { 0.0f, 0.0f };
};