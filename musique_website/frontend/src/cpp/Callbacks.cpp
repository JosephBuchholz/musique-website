#include "Callbacks.h"
#include "Debugging/Debug.h"
#include "Rendering/Renderer.h"

void UpdateRender(const RenderData& renderData)
{
    Renderer& renderer = Renderer::GetInstance();

    Vec2<float> scale = { 1.0f, 1.0f };

    for (const Line& line : renderData.m_lines)
    {
        Paint paint = line.paint;
        paint.color = 0x000000FF;

        renderer.DrawLine(line.start * scale, line.end * scale, paint);
    }

    for (const Text& text : renderData.m_texts)
    {
        Paint paint = text.paint;
        paint.color = 0x000000FF;

        renderer.DrawText(text.text, text.position * scale, paint);

        //BoundingBox boundingBox = renderer.MeasureText(text.text, text.paint);

        //renderer.DrawRect((text.position * scale) + boundingBox.position, boundingBox.size);
    }

    for (const SpannableText& text : renderData.m_spannableTexts)
    {
        LOGD("Drawing spannable text! size: %d", text.textSize);
        renderer.DrawSpannableText(text);
    }

    for (const SMuFLGlyph& glyph : renderData.m_SMuFLGlyphs)
    {
        Paint textPaint = glyph.paint;
        textPaint.color = 0x000000FF;

        renderer.DrawGlyph(glyph.codePoint, glyph.position * scale, textPaint);

        //BoundingBox boundingBox = renderer.MeasureGlyph(glyph.codePoint, textPaint);

        //renderer.DrawRect((glyph.position * scale) + boundingBox.position, boundingBox.size);
    }

    for (const CubicCurve& curve : renderData.m_cubicCurves)
    {
        renderer.DrawCubicCurve(curve.point1, curve.point2, curve.point3, curve.point4, curve.paint);
    }

    LOGD("Callbacks: Update Render function called");
}

void UpdateFrameData(const FrameData& frameData)
{
    LOGD("Callbacks: Update Frame Data function called");
}

void UpdateSongData(const SongData& songData)
{
    LOGD("Callbacks: Update Song Data function called");
}

void UpdatePrintRenderData(const PrintRenderData& printRenderData)
{
    LOGD("Callbacks: Update Print Render Data function called");
}

void UpdateViewModelData(const ViewModelData& viewModelData)
{
    LOGD("Callbacks: Update View Model Data function called");
}

void WriteMidi(char* bytes, size_t size)
{
    LOGD("Callbacks: Update Write Midi function called");
}

void SetMidiVolume(int volume)
{
    LOGD("Callbacks: Update Set Midi Volume function called");
}

void SetMidiReverb(int reverb)
{
    LOGD("Callbacks: Update Set Midi Reverb function called");
}

void OnTempoChangedCallback(float tempo)
{
    LOGD("Callbacks: Update On Tempo Changed function called");
}