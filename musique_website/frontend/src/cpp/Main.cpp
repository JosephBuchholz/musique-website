#include <iostream>

#include <emscripten.h>
#include <emscripten/bind.h>

#include "Debugging/Debug.h"
#include "Rendering/SMuFLGlyph.h"
#include "Collisions/Vec2.h"
#include "Rendering/Renderer.h"
#include "MusicData/Types.h"

void AddFunctionsToCpp(int drawLineFP, int drawTextFP, int drawGlyphFP)
{
    LOGI("Adding functions (from c++)");

    Renderer& renderer = Renderer::GetInstance();
    renderer.DrawLineCallback = reinterpret_cast<void (*)(int, int, int, int, const char*)>(drawLineFP);
    renderer.DrawTextCallback = reinterpret_cast<void (*)(const char*, int, int, const char*)>(drawTextFP);
    renderer.DrawGlyphCallback = reinterpret_cast<void (*)(uint16_t, int, int, const char*)>(drawGlyphFP);
}

void callJsFunction()
{
    LOGI("calling js function from c++");
    Renderer& renderer = Renderer::GetInstance();
    renderer.DrawLine({ 10.0f, 10.0f }, { 200.0f, 10.0f });

    Paint linePaint;

    linePaint.color = 0xFFFF00FF;
    linePaint.strokeWidth = 10.0f;

    renderer.DrawLine({ 50.0f, 500.0f }, { 500.0f, 550.0f }, linePaint);

    Paint textPaint;

    textPaint.color = 0x0000FFFF;
    textPaint.textSize = 40.0f;

    renderer.DrawText("Hello World from C++!!", { 20.0f, 50.0f }, textPaint);
    renderer.DrawGlyph(SMuFLID::fClef, { 300.0f, 300.0f }, textPaint);
    renderer.DrawGlyph(SMuFLID::gClef, { 350.0f, 300.0f });
    renderer.DrawGlyph(SMuFLID::cClef, { 400.0f, 300.0f }, linePaint);
    renderer.DrawGlyph(SMuFLID::flag256thDown, { 400.0f, 400.0f });
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("addFunctionsToCpp", &AddFunctionsToCpp);
    emscripten::function("callJsFunction", &callJsFunction);
}