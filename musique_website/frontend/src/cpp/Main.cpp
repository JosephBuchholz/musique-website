#include <iostream>
#include <chrono>

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include "Debugging/Debug.h"
#include "Rendering/SMuFLGlyph.h"
#include "Collisions/Vec2.h"
#include "Rendering/Renderer.h"
#include "Callbacks.h"
#include "MusicPlayers/MidiCallbacks.h"
#include "MusicData/Types.h"

#include "App.h"

using namespace std::chrono;



milliseconds previousTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
void MainLoop()
{
    milliseconds timeNow = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    double deltaTime = (timeNow - previousTime).count();

    App& app = App::GetInstance();

    app.OnUpdate(deltaTime);

    //LOGV("Main Loop!!!!: %lf", deltaTime);

    previousTime = timeNow;
}

EM_BOOL MouseWheelCallback(int eventType, const EmscriptenWheelEvent* wheelEvent, void* userData)
{
    App& app = App::GetInstance();

    MouseScrollEvent event;
    event.deltaX = wheelEvent->deltaX;
    event.deltaY = wheelEvent->deltaY;
    event.deltaZ = wheelEvent->deltaZ;

    return app.OnMouseScrollEvent(event);
}

int main()
{
    LOGI("Initilizing C++");

    const char* target = "#canvas";
    EMSCRIPTEN_RESULT result = emscripten_set_wheel_callback(target, nullptr, EM_TRUE, MouseWheelCallback);

    LOGD("got emscipten result: %d", result);

    emscripten_set_main_loop(MainLoop, 60, false);

    return 0;
}

bool LoadSong(int songStringPtr, int fileTypePtr)
{
    char* songString = reinterpret_cast<char*>(songStringPtr);
    char* fileType = reinterpret_cast<char*>(fileTypePtr);

    App& app = App::GetInstance();

    app.LoadSongFromString(fileType, songString);

    app.StartRendering();

    free(songString);
    free(fileType);

    return true;
}

bool OnButtonEvent(int buttonType, int eventType)
{
    App& app = App::GetInstance();

    ButtonEvent buttonEvent;
    buttonEvent.type = InputEvent::InputEventType::Button;
    buttonEvent.buttonType = (ButtonEvent::ButtonType)buttonType;
    buttonEvent.eventType = (ButtonEvent::EventType)eventType;

    return app.OnButtonEvent(buttonEvent);
}

bool OnPointerEvent(int pointerEventType, float positionX, float positionY)
{
    App& app = App::GetInstance();

    PointerEvent event;
    event.type = InputEvent::InputEventType::Pointer;
    event.eventType = (PointerEvent::PointerEventType)pointerEventType;
    event.position = { positionX, positionY };

    return app.OnPointerEvent(event);
}

bool OnTextFieldEvent(int id, int inputStringPtr)
{
    App& app = App::GetInstance();

    char* inputStringChar = reinterpret_cast<char*>(inputStringPtr); 
    std::string inputString = inputStringChar; 

    free(inputStringChar);

    return app.editor->OnTextFieldEvent(id, inputString);
}

void AddAudioCallbacksToCpp(int writeMidiFP)
{
    MidiCallbacks& midiCallbacks = MidiCallbacks::GetInstance();

    midiCallbacks.WriteMidiCallback = reinterpret_cast<void (*)(const uint8_t*, int)>(writeMidiFP);
}

void AddFunctionsToCpp(int clearFP, int drawLineFP, int drawTextFP, int drawUTF16TextFP, int drawGlyphFP, int drawCubicCurveFP, int measureTextFP, int measureUTF16TextFP, int measureGlyphFP, int startPDFRenderFP, int endPDFRenderFP, int startNewPDFPageFP)
{
    LOGI("Adding functions (from c++)");

    Renderer& renderer = Renderer::GetInstance();

    renderer.ClearCallback = reinterpret_cast<void (*)()>(clearFP);

    renderer.DrawLineCallback = reinterpret_cast<void (*)(float, float, float, float, const char*)>(drawLineFP);
    renderer.DrawTextCallback = reinterpret_cast<void (*)(const char*, float, float, const char*)>(drawTextFP);
    renderer.DrawUTF16TextCallback = reinterpret_cast<void (*)(const uint16_t*, float, float, const char*)>(drawUTF16TextFP);
    renderer.DrawGlyphCallback = reinterpret_cast<void (*)(uint16_t, float, float, const char*)>(drawGlyphFP);
    renderer.DrawCubicCurveCallback = reinterpret_cast<void (*)(float, float, float, float, float, float, float, float, const char*)>(drawCubicCurveFP);

    renderer.MeasureTextCallback = reinterpret_cast<float* (*)(const char*, const char*)>(measureTextFP);
    renderer.MeasureUTF16TextCallback = reinterpret_cast<float* (*)(const uint16_t*, const char*)>(measureUTF16TextFP);
    renderer.MeasureGlyphCallback = reinterpret_cast<float* (*)(uint16_t, const char*)>(measureGlyphFP);

    renderer.StartPDFRenderCallback = reinterpret_cast<void (*)()>(startPDFRenderFP);
    renderer.EndPDFRenderCallback = reinterpret_cast<void (*)()>(endPDFRenderFP);
    renderer.StartNewPDFPageCallback = reinterpret_cast<void (*)()>(startNewPDFPageFP);
}

void AddCallbackFunctionsToCpp(int downloadTextFP)
{
    Callbacks& callbacks = Callbacks::GetInstance();

    callbacks.DownloadTextCallback = reinterpret_cast<void (*)(const char*, const char*)>(downloadTextFP);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("addFunctionsToCpp", &AddFunctionsToCpp);
    emscripten::function("addCallbackFunctionsToCpp", &AddCallbackFunctionsToCpp);
    emscripten::function("addAudioCallbacksToCpp", &AddAudioCallbacksToCpp);
    emscripten::function("onButtonEvent", &OnButtonEvent);
    emscripten::function("onPointerEvent", &OnPointerEvent);
    emscripten::function("onTextFieldEvent", &OnTextFieldEvent);
    emscripten::function("loadSong", &LoadSong);
}