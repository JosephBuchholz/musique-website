#include "Chord.h"
#include "../../Utils/Converters.h"

#include "../../RenderMeasurement.h"

void Chord::Render(RenderData& renderData, const Settings& settings, Vec2<float> measurePosition) const
{
    // paint
    Paint paint = Paint();
    TextualElement::ModifyPaint(paint);
    paint.textSize = settings.displayConstants.chordFontSize.size;

    // render
    if (chordDiagram && settings.showChordDiagram == Settings::ShowChordDiagram::Auto)
        chordDiagram->Render(renderData, position + measurePosition);

    std::vector<TextSpan> textSpans;
    Paint glyphPaint = Paint(paint.color);
    glyphPaint.useMusicFont = true;
    glyphPaint.textSize = paint.textSize;
    glyphPaint.align = Paint::Align::Left;

    Paint normalPaint = Paint(paint.color);
    normalPaint.textSize = paint.textSize;
    normalPaint.align = Paint::Align::Left;

    // convert Spans to TextSpans
    for (auto span : spans)
    {
        textSpans.emplace_back(span.startIndex, span.endIndex, (span.type == (uint16_t)SpanType::MusicText) ? glyphPaint : normalPaint);
    }

    renderData.AddSpannableText(SpannableText(chordNameString, position + measurePosition, textSpans, paint));
}

void Chord::RenderDebug(RenderData& renderData, const Settings& settings, Vec2<float> measurePosition) const
{
    BoundingBox bb = GetBoundingBoxRelativeToParent();

    bb.position += measurePosition;

    bb.Render(renderData, 0xFFFF0000);
}

BoundingBox Chord::GetBoundingBoxRelativeToParent() const
{
    BoundingBox bb;

    Paint paint = Paint();
    if (fontStyle == FontStyle::Italic)
        paint.isItalic = true;
    if (fontWeight == FontWeight::Bold)
        paint.isBold = true;
    paint.textSize = fontSize.size;

    std::string name;
    for (const auto& c : chordNameString)
    {
        name += "B";
    }
    BoundingBox textBoundingBox = RenderMeasurement::GetTextBoundingBox(Text(name, position.x, position.y, paint));

    bb.position.x = textBoundingBox.position.x + position.x;
    bb.position.y = textBoundingBox.position.y + position.y;
    
    if (justify == Justify::Left)
        bb.position.x += textBoundingBox.size.x / 2.0f;

    //bb.position.y += textBoundingBox.size.y / 2.0f;
    bb.size.x = textBoundingBox.size.x;
    bb.size.y = textBoundingBox.size.y;

    if (chordDiagram)
    {
        bb = BoundingBox::CombineBoundingBoxes(bb, chordDiagram->GetBoundingBoxRelativeToParent());
    }

    return bb;
}

void Chord::UpdateBoundingBox(const Vec2<float> &parentPosition)
{
    boundingBox = GetBoundingBoxRelativeToParent();
    boundingBox.position += parentPosition;

    boundingBox.AddPadding(3.0f);

    boundingBox.constraints.emplace_back(Constraint::ConstraintType::NoHorizontal);

#if DEBUG_BOUNDING_BOXES
    debugBoundingBox = boundingBox;
#endif
}

void Chord::AddCharsToString(std::vector<uint16_t>& dest, const std::vector<uint16_t>& charsToAdd)
{
    for (const auto& c : charsToAdd)
    {
        dest.push_back(c);
    }
}

void Chord::AddCharsToString(std::vector<uint16_t>& dest, const std::string& charsToAdd)
{
    for (const auto& c : charsToAdd)
    {
        dest.push_back(c);
    }
}

std::vector<uint16_t> Chord::GetTextFromPitch(const Pitch& pitch)
{
    std::vector<uint16_t> chars;

    chars.push_back((uint16_t)DiatonicNoteToString(pitch.step)[0]);
    if (pitch.alter != 0)
        chars.push_back((uint16_t) GetChordSymbolAccidentalSMuFLID((int)pitch.alter));

    return chars;
}

std::vector<uint16_t> Chord::GetHarmonyStringWithSymbols() const
{
    SMuFLID symbol;

    switch (harmonyType)
    {
        case HarmonyType::Major: symbol = SMuFLID::None; break;
        case HarmonyType::MajorSeventh: symbol = SMuFLID::csymMajorSeventh; break;
        case HarmonyType::Minor: symbol = SMuFLID::csymMinor; break;
        case HarmonyType::MinorSeventh: symbol = SMuFLID::csymMinor; break;
        case HarmonyType::Augmented: symbol = SMuFLID::csymAugmented; break;
        case HarmonyType::Diminished: symbol = SMuFLID::csymDiminished; break;
        case HarmonyType::HalfDiminished: symbol = SMuFLID::csymHalfDiminished; break;
        default: symbol = SMuFLID::None; break;
    }

    if (symbol == SMuFLID::None)
    {
        return GetHarmonyStringWithoutSymbols();
    }

    std::vector<uint16_t> chars;
    chars.push_back((uint16_t)symbol);
    return chars;
}

std::vector<uint16_t> Chord::GetHarmonyStringWithoutSymbols() const
{
    std::string harmonyText;

    switch (harmonyType)
    {
        case HarmonyType::Major: break;
        case HarmonyType::MajorSeventh: harmonyText = "maj7"; break;
        case HarmonyType::Minor: harmonyText = "m"; break;
        case HarmonyType::MinorSeventh: harmonyText = "m7"; break;
        case HarmonyType::Dominant: harmonyText = "7"; break;
        case HarmonyType::Augmented: harmonyText = "aug"; break;
        case HarmonyType::Diminished: harmonyText = "dim"; break;
        case HarmonyType::HalfDiminished:
        {
            std::vector<uint16_t> chars; // m7b5
            chars.push_back('m');
            chars.push_back('7');
            chars.push_back((uint16_t)SMuFLID::csymAccidentalFlat);
            chars.push_back('5');
            return chars;
        }
        case HarmonyType::Power: harmonyText = "5"; break;
        case HarmonyType::SuspendedFourth: harmonyText = "sus"; break;
        case HarmonyType::SuspendedSecond: harmonyText = "sus2"; break;

        case HarmonyType::MajorNinth: harmonyText = "maj9"; break;
        case HarmonyType::Major11th: harmonyText = "maj11"; break;
        case HarmonyType::Major13th: harmonyText = "maj13"; break;

        case HarmonyType::MinorNinth: harmonyText = "m9"; break;
        case HarmonyType::Minor11th: harmonyText = "m11"; break;
        case HarmonyType::Minor13th: harmonyText = "m13"; break;

        case HarmonyType::DominantNinth: harmonyText = "9"; break;
        case HarmonyType::Dominant11th: harmonyText = "11"; break;
        case HarmonyType::Dominant13th: harmonyText = "13"; break;

        default: break;
    }

    std::vector<uint16_t> chars;
    for (const auto& c : harmonyText)
    {
        chars.push_back(c);
    }

    return chars;
}

void Chord::CalculateChordName(const Settings& settings)
{
    chordNameString.clear();

    AddCharsToString(chordNameString, GetTextFromPitch(rootPitch));

    if (settings.chordSymbolStyle == Settings::ChordSymbolStyle::Auto)
    {
        if (!harmonyTypeText.empty())
        {
            AddCharsToString(chordNameString, harmonyTypeText);
        }
        else
        {
            if (useSymbols)
                AddCharsToString(chordNameString, GetHarmonyStringWithSymbols());
            else
                AddCharsToString(chordNameString, GetHarmonyStringWithoutSymbols());
        }
    }
    else if (settings.chordSymbolStyle == Settings::ChordSymbolStyle::Normal)
    {
        AddCharsToString(chordNameString, GetHarmonyStringWithoutSymbols());
    }
    else if (settings.chordSymbolStyle == Settings::ChordSymbolStyle::Jazzy)
    {
        AddCharsToString(chordNameString, GetHarmonyStringWithSymbols());
    }

    std::vector<uint16_t> degreeChars;
    for (const ChordDegree& degree : degrees)
    {
        std::string sDegree = ToString((int) degree.degree.degree);
        std::vector<uint16_t> sDegreeChars;
        if (degree.degreeAlter.alter != 0.0f)
            sDegreeChars.push_back((uint16_t) GetChordSymbolAccidentalSMuFLID((int)degree.degreeAlter.alter));
        AddCharsToString(sDegreeChars, sDegree);

        if (degree.degreeType.type == DegreeType::Type::Add)
        {
            if (parentheses)
            {
                degreeChars.push_back((uint16_t)SMuFLID::csymParensLeftTall);
                AddCharsToString(degreeChars, sDegreeChars);
                degreeChars.push_back((uint16_t)SMuFLID::csymParensRightTall);
            }
            else if (brackets)
            {
                degreeChars.push_back((uint16_t)SMuFLID::csymBracketLeftTall);
                AddCharsToString(degreeChars, sDegreeChars);
                degreeChars.push_back((uint16_t)SMuFLID::csymBracketRightTall);
            }
            else
            {
                AddCharsToString(degreeChars, "add");
                AddCharsToString(degreeChars, sDegreeChars);
            }
        }
        else if (degree.degreeType.type == DegreeType::Type::Subtract)
        {
            AddCharsToString(degreeChars, "no");
            AddCharsToString(degreeChars, sDegreeChars);
        }
        else if (degree.degreeType.type == DegreeType::Type::Alter)
        {
            AddCharsToString(degreeChars, sDegreeChars);
        }
    }

    AddCharsToString(chordNameString, degreeChars);

    if (hasBassNote)
    {
        chordNameString.push_back((uint16_t)SMuFLID::csymAlteredBassSlash);
        AddCharsToString(chordNameString, GetTextFromPitch(bassPitch));
    }

    AddSpansForChordString();
}


void Chord::AddSpansForChordString()
{
    spans.clear();

    // assuming first character is not a musical one (will cause problem if first character was a musical character)
    bool isMusicTextSpan = false;
    Span currentSpan;
    currentSpan.type = (uint16_t)SpanType::NormalText;
    currentSpan.startIndex = 0;

    int index = 0;
    for (uint16_t c : chordNameString)
    {
        switch (c)
        {
            // if it is a musical symbol
            case (uint16_t)SMuFLID::csymAccidentalFlat:
            case (uint16_t)SMuFLID::csymAccidentalDoubleFlat:
            case (uint16_t)SMuFLID::accidentalSharp:
            case (uint16_t)SMuFLID::csymAccidentalDoubleSharp:
            case (uint16_t)SMuFLID::csymAccidentalNatural:
            case (uint16_t)SMuFLID::csymAlteredBassSlash:
            case (uint16_t)SMuFLID::csymBracketLeftTall:
            case (uint16_t)SMuFLID::csymBracketRightTall:
            case (uint16_t)SMuFLID::csymParensLeftTall:
            case (uint16_t)SMuFLID::csymParensRightTall:
            case (uint16_t)SMuFLID::csymMajorSeventh:
            case (uint16_t)SMuFLID::csymMinor:
            case (uint16_t)SMuFLID::csymAugmented:
            case (uint16_t)SMuFLID::csymDiminished:
            case (uint16_t)SMuFLID::csymHalfDiminished:
            {
                if (!isMusicTextSpan) // if span is not music text
                {
                    // push back current span
                    currentSpan.endIndex = index;
                    spans.push_back(currentSpan);

                    // create new span
                    currentSpan = Span();
                    currentSpan.startIndex = index;
                    currentSpan.type = (uint16_t)SpanType::MusicText;

                    isMusicTextSpan = true;
                }

                break;
            }

            // it is not a musical symbol
            default:
            {
                if (isMusicTextSpan) // if current span is music text
                {
                    // push back current span
                    currentSpan.endIndex = index;
                    spans.push_back(currentSpan);

                    // create new span
                    currentSpan = Span();
                    currentSpan.startIndex = index;
                    currentSpan.type = (uint16_t)SpanType::NormalText;


                    isMusicTextSpan = false;
                }
                
                break;
            }
        }

        index++;
    }

    // push back last span
    currentSpan.endIndex = index;
    spans.push_back(currentSpan);
}

void Chord::CalculatePositionAsPaged(const MusicDisplayConstants& displayConstants, float defaultX, float defaultY)
{
    position.x = defaultX;
    position.y = defaultY;

    fontSize.size = 8.0f;

    if (chordDiagram)
        chordDiagram->CalculatePositionAsPaged(displayConstants, { 0.0f, 18.0f });
}

void Chord::OnTranspose(const TranspositionRequest& transposeRequest, const MusicalKey& currentKey)
{
    rootPitch.OnTranspose(transposeRequest, currentKey);
    bassPitch.OnTranspose(transposeRequest, currentKey);
}

SMuFLID Chord::GetChordSymbolAccidentalSMuFLID(int alter)
{
    switch (alter)
    {
        case -3: return SMuFLID::csymAccidentalTripleFlat;
        case -2: return SMuFLID::csymAccidentalDoubleFlat;
        case -1: return SMuFLID::csymAccidentalFlat;
        case 0: return SMuFLID::csymAccidentalNatural;
        case 1: return SMuFLID::accidentalSharp;
        case 2: return SMuFLID::csymAccidentalDoubleSharp;
        case 3: return SMuFLID::csymAccidentalTripleSharp;
        default: return SMuFLID::ErrorGlyph;
    }
}

DiatonicNote Chord::ParseBasicPitch(char c)
{
    if (c == 'A')
        return DiatonicNote::A;
    else if (c == 'B')
        return  DiatonicNote::B;
    else if (c == 'C')
        return DiatonicNote::C;
    else if (c == 'D')
        return DiatonicNote::D;
    else if (c == 'E')
        return DiatonicNote::E;
    else if (c == 'F')
        return DiatonicNote::F;
    else if (c == 'G')
        return DiatonicNote::G;

    return DiatonicNote::C;
}

float Chord::ParseAlter(const std::string& chordSymbol, int index)
{
    if (index < chordSymbol.size())
    {
        char c = chordSymbol[index];

        if (c == '#')
            return 1.0f;
        else if (c == 'b')
            return -1.0f;
    }

    return 0.0f;
}

Chord Chord::CreateChordFromString(const std::string& chordString)
{
    Chord newChord;

    if (chordString.empty())
        return newChord;

    newChord.rootPitch.step = ParseBasicPitch(chordString[0]);
    newChord.rootPitch.alter = ParseAlter(chordString, 1);

    size_t foundSlash = chordString.find_first_of('/');
    if (foundSlash != std::string::npos)
    {
        if (chordString.size() > foundSlash + 1)
        {
            newChord.bassPitch.step = ParseBasicPitch(chordString[foundSlash + 1]);
            newChord.bassPitch.alter = ParseAlter(chordString, foundSlash + 2);
            newChord.hasBassNote = true;
        }
    }

    size_t foundMinor = chordString.find('m');
    if (foundMinor != std::string::npos)
        newChord.harmonyType = Chord::HarmonyType::Minor;

    size_t foundSus4 = chordString.find("sus4");
    if (foundSus4 != std::string::npos)
        newChord.harmonyType = Chord::HarmonyType::SuspendedFourth;

    return newChord;
}

std::string Chord::GetChordNameAsStandardString() const
{
    std::string chordString;

    for (uint16_t c : chordNameString)
    {
        // TODO: handle special characters
        chordString += c;
    }

    return chordString;
}