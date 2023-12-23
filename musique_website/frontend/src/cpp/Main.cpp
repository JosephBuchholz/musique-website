#include <iostream>

#include <emscripten.h>
#include <emscripten/bind.h>

#include "Debugging/Debug.h"
#include "Rendering/SMuFLGlyph.h"
#include "Collisions/Vec2.h"
#include "Rendering/Renderer.h"

void AddFunctionsToCpp(int drawLineFP, int drawTextFP, int drawGlyphFP)
{
    LOGI("Adding functions (from c++)");

    Renderer& renderer = Renderer::GetInstance();
    renderer.DrawLineCallback = reinterpret_cast<void (*)(int, int, int, int)>(drawLineFP);
    renderer.DrawTextCallback = reinterpret_cast<void (*)(const char*, int, int)>(drawTextFP);
    renderer.DrawGlyphCallback = reinterpret_cast<void (*)(uint16_t, int, int)>(drawGlyphFP);
}

void callJsFunction()
{
    LOGI("calling js function from c++");
    Renderer& renderer = Renderer::GetInstance();
    renderer.DrawLine({ 10.0f, 10.0f }, { 200.0f, 10.0f });
    renderer.DrawText("Hello World from C++!!", { 20.0f, 50.0f });
    renderer.DrawGlyph(SMuFLID::fClef, { 300.0f, 300.0f });
    renderer.DrawGlyph(SMuFLID::gClef, { 350.0f, 300.0f });
    renderer.DrawGlyph(SMuFLID::cClef, { 400.0f, 300.0f });
    renderer.DrawGlyph(SMuFLID::flag256thDown, { 400.0f, 400.0f });
}

int stringLength(std::string str) {
    return str.length();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("stringLength", &stringLength);
    emscripten::function("addFunctionsToCpp", &AddFunctionsToCpp);
    emscripten::function("callJsFunction", &callJsFunction);
}