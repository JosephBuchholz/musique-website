#include "HarmonyXMLExporter.h"

#include "../../Utils/Converters.h"
#include <memory>

#include "../../Collisions/Vec2.h"
#include "../MusicXMLParser/BaseElementParser.h"
#include "../MusicXMLParser/NoteElementParser.h"
#include "../../Exceptions/Exceptions.h"

#include "../../MusicData/Directions/Words.h"

#include "../../libs/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

XMLElement* NewTextElement(XMLDocument& doc, const std::string& name, const std::string& value)
{
    XMLElement* element = doc.NewElement(name.c_str());
    element->SetText(value.c_str());
    return element;
}

XMLElement* ExportLyric(XMLDocument& doc, const std::shared_ptr<CSLyric>& lyric)
{
    XMLElement* lyricElement = doc.NewElement("lyric");

    // syllabic element
    std::string syllabicValue = "single";
    lyricElement->InsertEndChild(NewTextElement(doc, "syllabic", syllabicValue));

    // text element
    XMLElement* lyricTextElement = doc.NewElement("text");
    lyricTextElement->SetText(lyric->lyricText.text.c_str());
    lyricElement->InsertEndChild(lyricTextElement);

    return lyricElement;
}

XMLElement* ExportChord(XMLDocument& doc, const std::shared_ptr<CSChord>& chord)
{
    XMLElement* chordElement = doc.NewElement("chord-symbol");
    
    int divisions = 16;

    // duration
    int durationValue = divisions * chord->duration; // in divisions

    chordElement->InsertEndChild(NewTextElement(doc, "duration", ToString(durationValue)));

    // root
    XMLElement* rootElement = doc.NewElement("root");
    XMLElement* rootStepElement = doc.NewElement("root-step");
    rootStepElement->SetText(DiatonicNoteToString(chord->chordSymbol.rootPitch.step).c_str());
    rootElement->InsertEndChild(rootStepElement);

    XMLElement* rootAlterElement = doc.NewElement("root-alter");
    rootAlterElement->SetText(ToString(chord->chordSymbol.rootPitch.alter).c_str());
    rootElement->InsertEndChild(rootAlterElement);
    
    chordElement->InsertEndChild(rootElement);

    // bass
    if (chord->chordSymbol.hasBassNote)
    {
        XMLElement* bassElement = doc.NewElement("bass");
        XMLElement* bassStepElement = doc.NewElement("bass-step");
        bassStepElement->SetText(DiatonicNoteToString(chord->chordSymbol.bassPitch.step).c_str());
        bassElement->InsertEndChild(bassStepElement);

        XMLElement* bassAlterElement = doc.NewElement("bass-alter");
        bassAlterElement->SetText(ToString(chord->chordSymbol.bassPitch.alter).c_str());
        bassElement->InsertEndChild(bassAlterElement);
    }

    // kind
    XMLElement* kindElement = doc.NewElement("kind");

    std::string harmonyTypeString = "";
    switch (chord->chordSymbol.harmonyType)
    {
        case Chord::HarmonyType::Other: harmonyTypeString = "other"; break;
        case Chord::HarmonyType::Augmented: harmonyTypeString = "augmmented"; break;
        case Chord::HarmonyType::AugmentedSeventh: harmonyTypeString = "augmmented-seventh"; break;
        case Chord::HarmonyType::Diminished: harmonyTypeString = "diminished"; break;
        case Chord::HarmonyType::DiminishedSeventh: harmonyTypeString = "diminish-seventh"; break;
        case Chord::HarmonyType::Dominant: harmonyTypeString = "dominant"; break;
        case Chord::HarmonyType::Dominant11th: harmonyTypeString = "dominant-11th"; break;
        case Chord::HarmonyType::Dominant13th: harmonyTypeString = "dominant-13th"; break;
        case Chord::HarmonyType::DominantNinth: harmonyTypeString = "dominant-ninth"; break;
        case Chord::HarmonyType::French: harmonyTypeString = "French"; break;
        case Chord::HarmonyType::German: harmonyTypeString = "German"; break;
        case Chord::HarmonyType::HalfDiminished: harmonyTypeString = "half-diminished"; break;
        case Chord::HarmonyType::Italian: harmonyTypeString = "Italian"; break;
        case Chord::HarmonyType::Major: harmonyTypeString = "major"; break;
        case Chord::HarmonyType::Major11th: harmonyTypeString = "major-11th"; break;
        case Chord::HarmonyType::Major13th: harmonyTypeString = "major-13th"; break;
        case Chord::HarmonyType::MajorMinor: harmonyTypeString = "major-minor"; break;
        case Chord::HarmonyType::MajorNinth: harmonyTypeString = "major-ninth"; break;
        case Chord::HarmonyType::MajorSeventh: harmonyTypeString = "major-seventh"; break;
        case Chord::HarmonyType::MajorSixth: harmonyTypeString = "major-sixth"; break;
        case Chord::HarmonyType::Minor: harmonyTypeString = "minor"; break;
        case Chord::HarmonyType::Minor11th: harmonyTypeString = "minor-11th"; break;
        case Chord::HarmonyType::Minor13th: harmonyTypeString = "minor-13th"; break;
        case Chord::HarmonyType::MinorNinth: harmonyTypeString = "minor-ninth"; break;
        case Chord::HarmonyType::MinorSeventh: harmonyTypeString = "minor-seventh"; break;
        case Chord::HarmonyType::MinorSixth: harmonyTypeString = "minor-sixth"; break;
        case Chord::HarmonyType::Neapolitan: harmonyTypeString = "Neapolitan"; break;
        case Chord::HarmonyType::Pedal: harmonyTypeString = "pedal"; break;
        case Chord::HarmonyType::Power: harmonyTypeString = "power"; break;
        case Chord::HarmonyType::SuspendedFourth: harmonyTypeString = "suspended-fourth"; break;
        case Chord::HarmonyType::SuspendedSecond: harmonyTypeString = "suspended-second"; break;
        case Chord::HarmonyType::Tristan: harmonyTypeString = "Tristan"; break;
        case Chord::HarmonyType::NoHarmony:
        default: harmonyTypeString = "none"; break;
    }

    kindElement->SetText(harmonyTypeString.c_str());
    chordElement->InsertEndChild(kindElement);
    
    return chordElement;
}

XMLElement* ExportMeasure(XMLDocument& doc, const std::shared_ptr<CSMeasure>& measure, int index)
{
    XMLElement* measureElement = doc.NewElement("measure");

    //measureElement->SetAttribute("number", measure->number);
    measureElement->SetAttribute("width", measure->width);

    if (index == 0)
    {
        XMLElement* attributesElement = doc.NewElement("attributes");
        attributesElement->InsertEndChild(NewTextElement(doc, "divisions", ToString(4)));

        measureElement->InsertEndChild(attributesElement);
    }
    
    std::vector<XMLElement*> elements;

    for (auto lyric : measure->lyrics)
    {
        elements.push_back(ExportLyric(doc, lyric));
    }

    for (auto chord : measure->chords)
    {
        elements.push_back(ExportChord(doc, chord));
    }

    for (auto element : elements)
    {
        measureElement->InsertEndChild(element);
    }

    return measureElement;
}

std::string HarmonyXMLExporter::ExportHarmonyXML(const std::shared_ptr<Song>& song)
{
    XMLDocument doc;

    XMLNode* declaration = doc.NewDeclaration();
    doc.InsertFirstChild(declaration);

    XMLElement* root = doc.NewElement("score-partwise");
    root->SetAttribute("version", 1.0);

    XMLElement* work = doc.NewElement("work");
    XMLElement* workTitle = doc.NewElement("work-title");
    workTitle->SetText(song->songData.songTitle.c_str());
    work->InsertFirstChild(workTitle);

    root->InsertFirstChild(work);

    XMLElement* partList = doc.NewElement("part-list");
    
    for (const auto& instrument : song->instruments)
    {
        XMLElement* scorePart = doc.NewElement("score-part");

        scorePart->SetAttribute("id", instrument->id.c_str());

        scorePart->InsertEndChild(NewTextElement(doc, "part-name", instrument->name.string));
        scorePart->InsertEndChild(NewTextElement(doc, "part-abbreviation", instrument->nameAbbreviation.string));

        partList->InsertEndChild(scorePart);
    }
    
    root->InsertEndChild(partList);

    for (const auto& instrument : song->instruments)
    {
        XMLElement* part = doc.NewElement("part");

        part->SetAttribute("id", instrument->id.c_str());

        for (const auto& staff : instrument->staves)
        {
            if (staff->type == Staff::StaffType::ChordSheet && staff->csStaff)
            {
                int i = 0;
                for (const auto& measure : staff->csStaff->measures)
                {
                    XMLElement* m = ExportMeasure(doc, measure, i);
                    part->InsertEndChild(m);
                    i++;
                }
            }
        }

        root->InsertEndChild(part);
    }


    doc.InsertEndChild(root);

    XMLPrinter printer;
    doc.Print(&printer);
    std::string data = printer.CStr();

    return data;
}