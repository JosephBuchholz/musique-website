#include <iostream>

#include <emscripten.h>
#include <emscripten/bind.h>

#include "Debugging/Debug.h"
#include "Rendering/SMuFLGlyph.h"
#include "Collisions/Vec2.h"
#include "Rendering/Renderer.h"
#include "MusicData/Types.h"

#include "App.h"

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
    std::shared_ptr<App> app = std::make_shared<App>();


    /*app->LoadSongFromString("txt", R"V0G0N(
key C

#v

C.. Csus4.. C

How I _long to _breathe the air of _Heaven

Am.. F.. G

Where pain is _gone, and _mercy fills the _streets

C/E.. F.. C. G. Am..

To look _upon the _one who bled to _save _me _

F.. G.. C... Csus4. C

And walk with _Him for _all eterni_ty

#c

F.. G.. C... C/E.

There will be a _day when _all will bow be_fore Him. There will _be a

F.. Am.. G... C/E.

_day when _death will be no_  more. Standing _face to

F.. G.. C. Em. F..

_face with _He who died and _rose _a_gain

F.. G.. C. C/E. F.. Am.. G..

(1) _Holy, _Holy is the _Lord

(to Verse 2)

F.. G.. C... Csus4. C

(2) _Holy, _Holy is the _Lord

(to Verse 3)

#v

C.. Csus4.. C

And every _prayer we _prayed in desper_ation,

Am.. F.. G

The songs of _faith we _sang through doubt and _fear

C/E.. F.. C. G. Am..

In the _end we will _see that it was _worth _it _

F.. G.. C... Csus4. C

When He re_turns to _wipe away _our tears

#v

C.. Csus4.. C

And on that _day we _join the resur_rection

Am.. F.. G

And stand be_side the _heroes of the _faith

C/E.. F.. C. G. Am..

And with one _voice a _thousand gener_a_tions,

F.. G.. C

(1) Sing _worthy is the _Lamb who was _slain

(repeat)

F.. G.. Am.. C..

(2) Sing _worthy is the _Lamb who was _slain

F.. G.. C... Csus4. C

For_ever _He shall re_ign

#c

F.. G.. C... C/E.

So let it be to_day we _shout the hymn of _Heaven. With angles _and the

F.. Am.. G... C/E.

_Saints we _raise a mighty _roar. Glory _to our 

F.. G.. C. Em. F..

_God who _gave us life bey_ond _the _grave

F.. G.. C

(1) _Holy, _Holy is the _Lord

F.. G.. Am.. C/E..

(2) _Holy, _Holy is the _Lord

F.. G.. Am.. C/E..

_Holy, _Holy is the _Lord

F.. G.. C

_Holy, _Holy is the _Lord

)V0G0N");*/

    app->StartRendering();

    app->OnUpdate(0.1f);


    LOGI_TAG(LOG_TAG, "calling js function from c++");
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