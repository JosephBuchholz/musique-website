#pragma once

#include <memory>

#include "../MusicData/Song.h"
#include "../Events/InputEvent.h"
#include "../Rendering/MusicRenderer.h"

class Editor
{
public:

    bool OnPointerEvent(const PointerEvent& event);
    bool OnTextFieldEvent(int id, const std::string& input);
    void OnPropertiesUpdated(const std::string& propertiesString);

public:
    std::shared_ptr<Song> song = nullptr;
    std::shared_ptr<MusicRenderer> musicRenderer = nullptr;

    std::vector<std::shared_ptr<BaseElement>> selectedElements;
};