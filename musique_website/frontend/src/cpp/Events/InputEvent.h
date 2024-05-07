#ifndef MUSIQUE_INPUTEVENT_H
#define MUSIQUE_INPUTEVENT_H

#include "../Collisions/Vec2.h"

class InputEvent
{
public:
    enum class InputEventType {
        None, Tap, MouseScroll, Button, Pointer, Keyboard
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

class PointerEvent : public InputEvent
{
public:
    enum class PointerEventType
    {
        None, Down, Up, Move
    } eventType = PointerEventType::Down;

    Vec2<float> position;
};

class KeyboardEvent : public InputEvent
{
public:
    enum class KeyboardEventType
    {
        None, Down, Up 
    } eventType = KeyboardEventType::Down;

    enum class KeyCode {
        None = 0,
        a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
        A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
        Shift, Delete, Backspace, Enter, Control, Alt, Tab, Escape,
        Num1,Num2,Num3,Num4,Num5,Num6,Num7,Num8,Num9,Num0,
        BracketLeft,BracketRight,
        NumLock,CapsLock,
        Minus,Plus,
    } keyCode = KeyCode::None;
};

#endif //MUSIQUE_INPUTEVENT_H
