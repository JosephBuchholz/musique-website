#include "Renderer.h"

#include "../Utils/Converters.h"

Renderer& Renderer::GetInstance()
{
    static Renderer instance;
    return instance;
}

void AddValueToJson(std::string& json, const std::string& key, const std::string& value)
{
    json += "\"";
    json += key;
    json += "\":";
    json += value;
}

void AddIntValueToJson(std::string& json, const std::string& key, uint32_t value)
{
    AddValueToJson(json, key, ToString(value));
}

void AddFloatValueToJson(std::string& json, const std::string& key, float value)
{
    AddValueToJson(json, key, ToString(value));
}

void AddBooleanValueToJson(std::string& json, const std::string& key, bool value)
{
    AddValueToJson(json, key, (value ? "true" : "false"));
}

void AddStringValueToJson(std::string& json, const std::string& key, const std::string& value)
{
    AddValueToJson(json, key, "\"" + value + "\"");
}

void Renderer::Clear()
{
    ClearCallback();
}

void Renderer::DrawLine(Vec2<float> start, Vec2<float> end, const Paint& paint)
{
    DrawLineCallback(start.x * scale, start.y * scale, end.x * scale, end.y * scale, EncodePaintObject(paint, scale).c_str());
}

void Renderer::DrawRect(Vec2<float> position, Vec2<float> size, const Paint& paint)
{
    DrawLine({ position.x, position.y }, { position.x + size.x, position.y }, paint);
    DrawLine({ position.x + size.x, position.y }, { position.x + size.x, position.y + size.y }, paint);
    DrawLine({ position.x + size.x, position.y + size.y }, { position.x, position.y + size.y }, paint);
    DrawLine({ position.x, position.y + size.y }, { position.x, position.y }, paint);
}

void Renderer::DrawText(const std::string& text, Vec2<float> position, const Paint& paint)
{
    DrawTextCallback(text.c_str(), position.x * scale, position.y * scale, EncodePaintObject(paint, scale).c_str());
}

uint16_t* NewSubString(uint16_t* string, int size, int startIndex, int endIndex)
{
    int length = (endIndex - startIndex) + 1;
    uint16_t* subString = (uint16_t*)malloc(length * sizeof(uint16_t));

    int j = 0;
    for (int i = startIndex; i < endIndex; i++)
    {
        subString[j] = string[i];
        j++;
    }

    // null terminator
    subString[length - 1] = 0;

    return subString;
}

void Renderer::DrawSpannableText(const SpannableText& spannableText)
{
    Vec2<float> currentPosition = spannableText.position * scale;

    for (const TextSpan& span : spannableText.spans)
    {
        uint16_t* text = nullptr;

        if (span.endIndex <= spannableText.textSize)
            text = NewSubString(spannableText.text, spannableText.textSize, span.startIndex, span.endIndex);
        else if (span.startIndex < spannableText.textSize)
            text = NewSubString(spannableText.text, spannableText.textSize, span.startIndex, spannableText.textSize);

        ASSERT(text != nullptr);

        DrawUTF16TextCallback(text, currentPosition.x, currentPosition.y, EncodePaintObject(span.paint, scale).c_str());

        currentPosition.x += MeasureUTF16Text(text, span.paint).size.x;

        free(text);
    }
}

void Renderer::DrawGlyph(SMuFLID glyph, Vec2<float> position, const Paint& paint)
{
    DrawGlyphCallback((uint16_t)glyph, position.x * scale, position.y * scale, EncodePaintObject(paint, scale).c_str());
}

void Renderer::DrawCubicCurve(Vec2<float> point1, Vec2<float> point2, Vec2<float> point3, Vec2<float> point4, const Paint& paint)
{
    DrawCubicCurveCallback(point1.x * scale, point1.y * scale, point2.x * scale, point2.y * scale, point3.x * scale, point3.y * scale, point4.x * scale, point4.y * scale, EncodePaintObject(paint, scale).c_str());
}

BoundingBox Renderer::MeasureText(const std::string &text, const Paint &paint)
{
    BoundingBox boundingBox;
    float* boundingBoxArray = MeasureTextCallback(text.c_str(), EncodePaintObject(paint).c_str());

    if (boundingBoxArray == nullptr)
    {
        LOGE("BoundingBoxArray from JS is nullptr");
        return boundingBox;
    }

    boundingBox.position.x = boundingBoxArray[0];
    boundingBox.position.y = boundingBoxArray[1];
    boundingBox.size.x = boundingBoxArray[2];
    boundingBox.size.y = boundingBoxArray[3];

    free(boundingBoxArray);

    return boundingBox;
}

BoundingBox Renderer::MeasureUTF16Text(const uint16_t *text, const Paint &paint)
{
    BoundingBox boundingBox;
    float* boundingBoxArray = MeasureUTF16TextCallback(text, EncodePaintObject(paint).c_str());

    if (boundingBoxArray == nullptr)
    {
        LOGE("BoundingBoxArray from JS is nullptr");
        return boundingBox;
    }

    boundingBox.position.x = boundingBoxArray[0];
    boundingBox.position.y = boundingBoxArray[1];
    boundingBox.size.x = boundingBoxArray[2];
    boundingBox.size.y = boundingBoxArray[3];

    free(boundingBoxArray);

    return boundingBox;
}

BoundingBox Renderer::MeasureGlyph(SMuFLID glyph, const Paint &paint)
{
    BoundingBox boundingBox;
    float* boundingBoxArray = MeasureGlyphCallback((uint16_t)glyph, EncodePaintObject(paint).c_str());

    if (boundingBoxArray == nullptr)
    {
        LOGE("BoundingBoxArray from JS is nullptr");
        return boundingBox;
    }

    //LOGD("array: %f, %f, %f, %f, %d", boundingBoxArray[0], boundingBoxArray[1], boundingBoxArray[2], boundingBoxArray[3], boundingBoxArray);

    boundingBox.position.x = boundingBoxArray[0];
    boundingBox.position.y = boundingBoxArray[1];
    boundingBox.size.x = boundingBoxArray[2];
    boundingBox.size.y = boundingBoxArray[3];

    free(boundingBoxArray);

    return boundingBox;
}

void Renderer::StartPDFRender()
{
    StartPDFRenderCallback();
    isRenderingPDF = true;
}

void Renderer::EndPDFRender()
{
    EndPDFRenderCallback();
    isRenderingPDF = false;
}

void Renderer::StartNewPDFPage()
{
    LOGD("Starting new PDF page!!!!!!!!");
    StartNewPDFPageCallback();
}

std::string Renderer::EncodePaintObject(const Paint& paint, float scale)
{
	std::string paintString;

	paintString += "{";

	AddIntValueToJson(paintString, "color", paint.color);
	paintString += ",";
	AddFloatValueToJson(paintString, "strokeWidth", paint.strokeWidth * scale);
	paintString += ",";
	AddFloatValueToJson(paintString, "textSize", paint.textSize * scale);
	paintString += ",";
	AddFloatValueToJson(paintString, "glyphSizeFactor", paint.glyphSizeFactor * scale);
	paintString += ",";
	AddBooleanValueToJson(paintString, "isBold", paint.isBold);
	paintString += ",";
	AddBooleanValueToJson(paintString, "isItalic", paint.isItalic);
	paintString += ",";
	AddBooleanValueToJson(paintString, "useMusicFont", paint.useMusicFont);
	paintString += ",";
	AddBooleanValueToJson(paintString, "varyThickness", paint.varyThickness);
	paintString += ",";
	AddFloatValueToJson(paintString, "slurSpacing", paint.slurSpacing * scale);
	paintString += ",";
	AddBooleanValueToJson(paintString, "verticalEnds", paint.verticalEnds);
	paintString += ",";

	AddBooleanValueToJson(paintString, "isTablature", paint.isTablature);
	paintString += ",";

	AddBooleanValueToJson(paintString, "centerHorizontally", paint.centerHorizontally);
	paintString += ",";
	AddBooleanValueToJson(paintString, "centerVetically", paint.centerVertically);
	paintString += ",";



    std::string strokeCapString;
    switch (paint.strokeCap)
    {
        case Paint::Cap::Butt: strokeCapString = "butt"; break;
        case Paint::Cap::Round: strokeCapString = "round"; break;
        case Paint::Cap::Square: strokeCapString = "square"; break;
    }

	AddStringValueToJson(paintString, "strokeCap", strokeCapString);
    paintString += ",";

    std::string alignString;
    switch (paint.align)
    {
        case Paint::Align::Left: alignString = "left"; break;
        case Paint::Align::Center: alignString = "center"; break;
        case Paint::Align::Right: alignString = "right"; break;
    }

	AddStringValueToJson(paintString, "align", alignString);

	paintString += "}";

    return paintString;
}
