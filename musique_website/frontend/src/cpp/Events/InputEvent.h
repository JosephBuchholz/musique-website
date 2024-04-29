#ifndef MUSIQUE_INPUTEVENT_H
#define MUSIQUE_INPUTEVENT_H

#include "../Collisions/Vec2.h"

class InputEvent
{
public:
    enum class InputEventType {
        None, Tap, MouseScroll, Button
    } type = InputEventType::None;
};

class TapEvent : public InputEvent
{
public:
    Vec2<float> position = { 0.0f, 0.0f };
};

class MouseScrollEvent : public InputEvent
{
public:
    double deltaX = 0.0;
    double deltaY = 0.0;
    double deltaZ = 0.0;
};

class ButtonEvent : public InputEvent
{
public:
    enum class ButtonType
    {
        None, Play, Reset, Metronome, DownloadPDF, Export
    } buttonType = ButtonType::None;

    enum class EventType
    {
        None, Pressed, ToggledOn, ToggledOff
    } eventType = EventType::Pressed;
};

#endif //MUSIQUE_INPUTEVENT_H
