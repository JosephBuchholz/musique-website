#include "Editor.h"

#include "JsonHelper.h"

std::string Editor::GetLyricProperties(const CSLyric* lyric) const
{
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddTextToJson(jsonObject, "Main", "text", "Text", lyric->lyricText.text);
    JsonHelper::AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", lyric->beatPosition);
    JsonHelper::AddFloatToJson(jsonObject, "Main", "duration", "Duration", lyric->duration);
    JsonHelper::AddBoolToJson(jsonObject, "Main", "startsPickup", "Pickup To Next Measure", lyric->startsPickup);

    JsonHelper::AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)lyric->lyricText.fontWeight - 1));
    JsonHelper::AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)lyric->lyricText.fontStyle - 1));
    JsonHelper::AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", lyric->lyricText.fontSize.size);
    JsonHelper::AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", lyric->lyricText.color.color);

    JsonHelper::AddFloatToJson(jsonObject, "Position", "posX", "Position X", lyric->position.x);
    JsonHelper::AddFloatToJson(jsonObject, "Position", "posY", "Position Y", lyric->position.y);

    return jsonObject.dump();
}

void Editor::SetLyricProperties(CSLyric* lyric, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    lyric->beatPosition = jsonObject["Main"]["beatPosition"]["value"];
    lyric->duration = jsonObject["Main"]["duration"]["value"];
    lyric->startsPickup = jsonObject["Main"]["startsPickup"]["value"];

    std::string text = jsonObject["Main"]["text"]["value"];

    if (text.back() == ' ')
        text.pop_back();

    lyric->lyricText.text = text;

    bool fontWeight = jsonObject["Text Properties"]["isBold"]["value"];
    bool fontStyle = jsonObject["Text Properties"]["isItalic"]["value"];

    lyric->lyricText.fontWeight = (FontWeight)(1 + fontWeight);
    lyric->lyricText.fontStyle = (FontStyle)(1 + fontStyle);
    lyric->lyricText.fontSize.size = jsonObject["Text Properties"]["textSize"]["value"];
    lyric->lyricText.color.color = jsonObject["Text Properties"]["color"]["value"];

    lyric->position.x = jsonObject["Position"]["posX"]["value"];
    lyric->position.y = jsonObject["Position"]["posY"]["value"];
}

std::string Editor::GetChordProperties(const CSChord* chord) const
{
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddTextToJson(jsonObject, "Main", "chordName", "Chord Name", chord->chordSymbol.GetChordNameAsStandardString());
    JsonHelper::AddFloatToJson(jsonObject, "Main", "beatPosition", "Beat Position", chord->beatPosition);
    JsonHelper::AddFloatToJson(jsonObject, "Main", "duration", "Duration", chord->duration);

    JsonHelper::AddFloatToJson(jsonObject, "Position", "posX", "Position X", chord->position.x);
    JsonHelper::AddFloatToJson(jsonObject, "Position", "posY", "Position Y", chord->position.y);

    return jsonObject.dump();
}

void Editor::SetChordProperties(CSChord* chord, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    chord->chordSymbol = Chord::CreateChordFromString(jsonObject["Main"]["chordName"]["value"]);
    chord->chordSymbol.CalculateChordName(song->settings);
    chord->beatPosition = jsonObject["Main"]["beatPosition"]["value"];
    chord->duration = jsonObject["Main"]["duration"]["value"];

    chord->position.x = jsonObject["Position"]["posX"]["value"];
    chord->position.y = jsonObject["Position"]["posY"]["value"];
}

std::string Editor::GetMeasureProperties(const CSMeasure* measure) const
{
    int measureIndex = measure->GetMeasureIndex();
            
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddFloatToJson(jsonObject, "Main", "width", "Width", measure->width);
    JsonHelper::AddBoolToJson(jsonObject, "Main", "pageBreak", "Page Break", song->systemMeasures[measureIndex]->pageBreak);
    JsonHelper::AddBoolToJson(jsonObject, "Main", "systemBreak", "System Break", song->systemMeasures[measureIndex]->systemBreak);
    JsonHelper::AddBoolToJson(jsonObject, "Main", "isPickupMeasure", "Is Pickup Measure", song->systemMeasures[measureIndex]->isPickupMeasure);

    return jsonObject.dump();
}

void Editor::SetMeasureProperties(CSMeasure* measure, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    int measureIndex = measure->GetMeasureIndex();

    measure->width = jsonObject["Main"]["width"]["value"];
    song->systemMeasures[measureIndex]->pageBreak = jsonObject["Main"]["pageBreak"]["value"];
    song->systemMeasures[measureIndex]->systemBreak = jsonObject["Main"]["systemBreak"]["value"];
    song->systemMeasures[measureIndex]->isPickupMeasure = jsonObject["Main"]["isPickupMeasure"]["value"];

}

std::string Editor::GetCreditProperties(const Credit* credit) const
{
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddTextToJson(jsonObject, "Main", "text", "Text", credit->words.text);
    JsonHelper::AddIntToJson(jsonObject, "Main", "pageNumber", "Page", credit->pageNumber);

    JsonHelper::AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)credit->words.fontWeight - 1));
    JsonHelper::AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)credit->words.fontStyle - 1));
    JsonHelper::AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", credit->words.fontSize.size);
    JsonHelper::AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", credit->words.color.color);

    JsonHelper::AddFloatToJson(jsonObject, "Position", "posX", "Position X", credit->words.positionX);
    JsonHelper::AddFloatToJson(jsonObject, "Position", "posY", "Position Y", credit->words.positionY);

    return jsonObject.dump();
}

void Editor::SetCreditProperties(Credit* credit, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    credit->words.text = jsonObject["Main"]["text"]["value"];
    credit->pageNumber = jsonObject["Main"]["pageNumber"]["value"];

    bool fontWeight = jsonObject["Text Properties"]["isBold"]["value"];
    bool fontStyle = jsonObject["Text Properties"]["isItalic"]["value"];

    credit->words.fontWeight = (FontWeight)(1 + fontWeight);
    credit->words.fontStyle = (FontStyle)(1 + fontStyle);
    credit->words.fontSize.size = jsonObject["Text Properties"]["textSize"]["value"];
    credit->words.color.color = jsonObject["Text Properties"]["color"]["value"];

    credit->words.positionX = jsonObject["Position"]["posX"]["value"];
    credit->words.positionY = jsonObject["Position"]["posY"]["value"];
}

std::string Editor::GetTextDirectionProperties(const TextDirection* direction) const
{
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddTextToJson(jsonObject, "Main", "text", "Text", direction->text.text);

    JsonHelper::AddBoolToJson(jsonObject, "Text Properties", "isBold", "Bold", (bool)((int)direction->text.fontWeight - 1));
    JsonHelper::AddBoolToJson(jsonObject, "Text Properties", "isItalic", "Italic", (bool)((int)direction->text.fontStyle - 1));
    JsonHelper::AddFloatToJson(jsonObject, "Text Properties", "textSize", "Text Size", direction->text.fontSize.size);
    JsonHelper::AddColorToJson(jsonObject, "Text Properties", "color", "Text Color", direction->text.color.color);

    JsonHelper::AddFloatToJson(jsonObject, "Position", "posX", "Position X", direction->position.x);
    JsonHelper::AddFloatToJson(jsonObject, "Position", "posY", "Position Y", direction->position.y);

    return jsonObject.dump();
}

void Editor::SetTextDirectionProperties(TextDirection* direction, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    direction->text.text = jsonObject["Main"]["text"]["value"];

    bool fontWeight = jsonObject["Text Properties"]["isBold"]["value"];
    bool fontStyle = jsonObject["Text Properties"]["isItalic"]["value"];

    direction->text.fontWeight = (FontWeight)(1 + fontWeight);
    direction->text.fontStyle = (FontStyle)(1 + fontStyle);
    direction->text.fontSize.size = jsonObject["Text Properties"]["textSize"]["value"];
    direction->text.color.color = jsonObject["Text Properties"]["color"]["value"];

    direction->position.x = jsonObject["Position"]["posX"]["value"];
    direction->position.y = jsonObject["Position"]["posY"]["value"];
}

std::string Editor::GetTimeSignatureProperties(const TimeSignature* timeSignature) const
{
    nlohmann::ordered_json jsonObject;

    JsonHelper::AddIntToJson(jsonObject, "Main", "notes", "Top", timeSignature->notes);
    JsonHelper::AddIntToJson(jsonObject, "Main", "noteType", "Bottom", timeSignature->noteType);

    JsonHelper::AddFloatToJson(jsonObject, "Position", "posX", "Position X", timeSignature->position.x);
    JsonHelper::AddFloatToJson(jsonObject, "Position", "posY", "Position Y", timeSignature->position.y);

    jsonObject.dump();
}

void Editor::SetTimeSignatureProperties(TimeSignature* timeSignature, const std::string& propertiesString)
{
    json jsonObject = json::parse(propertiesString);

    timeSignature->notes = jsonObject["Main"]["notes"]["value"];
    timeSignature->noteType = jsonObject["Main"]["noteType"]["value"];

    timeSignature->position.x = jsonObject["Position"]["posX"]["value"];
    timeSignature->position.y = jsonObject["Position"]["posY"]["value"];
}