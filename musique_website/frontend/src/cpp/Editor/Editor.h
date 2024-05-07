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

public:
    std::shared_ptr<Song> song = nullptr;
    std::shared_ptr<MusicRenderer> musicRenderer = nullptr;

    std::vector<std::shared_ptr<BaseElement>> selectedElements;

    bool pointerIsDown = false;
    Vec2<float> pointerDownPosition = { 0.0f, 0.0f };
};