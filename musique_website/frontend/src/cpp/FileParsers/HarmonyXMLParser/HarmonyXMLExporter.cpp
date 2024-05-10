#include "HarmonyXMLExporter.h"

#include "../../Utils/Converters.h"
#include <memory>
#include <algorithm>

#include "../../Collisions/Vec2.h"
#include "../MusicXMLParser/BaseElementParser.h"
#include "../MusicXMLParser/NoteElementParser.h"
#include "../../Exceptions/Exceptions.h"

#include "../../MusicData/Directions/Words.h"

#include "../../libs/tinyxml2/tinyxml2.h"

#include "HarmonyXMLExportHelper.h"

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

    // relative to measure
    lyricElement->SetAttribute("default-x", lyric->position.x);
    lyricElement->SetAttribute("default-y", lyric->position.y);

    lyricElement->SetAttribute("is-pickup", HarmonyXMLExportHelper::FromBoolToYesNo(lyric->isPickupToNextMeasure));
    lyricElement->SetAttribute("starts-pickup", HarmonyXMLExportHelper::FromBoolToYesNo(lyric->startsPickup));

    // syllabic element
    if (lyric->parentSyllableGroup)
    {
        if (lyric->parentSyllableGroup->lyrics.size() != 0)
        {
            std::string syllabicValue = "";
            if (lyric->parentSyllableGroup->lyrics.size() == 1) // only this lyric
            {
                syllabicValue = "single";
            }
            else if (lyric->parentSyllableGroup->lyrics[0] == lyric) // lyric is the start
            {
                syllabicValue = "start";
            }
            else if (lyric->parentSyllableGroup->lyrics.back() == lyric) // lyric is at the end
            {
                syllabicValue = "end";
            }

            if (syllabicValue != "")
                lyricElement->InsertEndChild(NewTextElement(doc, "syllabic", syllabicValue));
        }
    }

    // text element
    XMLElement* lyricTextElement = doc.NewElement("text");
    lyricTextElement->SetText(lyric->lyricText.text.c_str());
    lyricElement->InsertEndChild(lyricTextElement);

    // phrase element
    if (lyric->parentLyricalPhrase)
    {
        if (lyric->parentLyricalPhrase->lyrics.size() != 0)
        {
            std::string phraseValue = "";
            if (lyric->parentLyricalPhrase->lyrics.size() == 1) // only one lyric in phrase
            {
                phraseValue = "single";
            }
            else if (lyric->parentLyricalPhrase->lyrics[0] == lyric) // is the first lyric in the lyrical phrase
            {
                phraseValue = "start";
            }
            else if (lyric->parentLyricalPhrase->lyrics.back() == lyric) // is the last lyric
            {
                phraseValue = "end";
            }


            if (phraseValue != "")
                lyricElement->InsertEndChild(NewTextElement(doc, "phrase", phraseValue));
        }
    }

    return lyricElement;
}

XMLElement* ExportChord(XMLDocument& doc, const std::shared_ptr<CSChord>& chord)
{
    XMLElement* chordElement = doc.NewElement("chord-symbol");

    // relative to the measure
    chordElement->SetAttribute("default-x", chord->position.x);
    chordElement->SetAttribute("default-y", chord->position.y);
    
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

XMLElement* ExportTextDirection(XMLDocument& doc, const std::shared_ptr<TextDirection>& direction)
{
    XMLElement* directionElement = doc.NewElement("direction");

    XMLElement* directionType = doc.NewElement("direction-type");
    
    if (direction->directionType == TextDirection::DirectionType::Rehearsal)
    {
        std::shared_ptr<Rehearsal> rehearsal = std::dynamic_pointer_cast<Rehearsal>(direction);

        XMLElement* rehearsalElement = doc.NewElement("rehearsal");

        HarmonyXMLExportHelper::SetTextualAttributes(rehearsalElement, rehearsal->text);
        //rehearsalElement->SetAttribute("font-size", rehearsal->text.fontSize.size);
        //rehearsalElement->SetAttribute("font-weight", rehearsal->text.fontSize.size);

        rehearsalElement->SetText(rehearsal->text.text.c_str());

        directionType->InsertEndChild(rehearsalElement);
    }

    directionElement->InsertEndChild(directionType);

    return directionElement;
}

XMLElement* ExportMeasure(XMLDocument& doc, const std::shared_ptr<CSMeasure>& measure, int index, const std::shared_ptr<System>& currentSystem, bool startsNewSystem)
{
    XMLElement* measureElement = doc.NewElement("measure");

    //measureElement->SetAttribute("number", measure->number);
    measureElement->SetAttribute("width", measure->width);

    if (measure->isFirstMeasureOfSystem || startsNewSystem)
    {
        XMLElement* printElement = doc.NewElement("print");

        printElement->SetAttribute("new-system", "yes");
        
        XMLElement* systemLayoutElement = doc.NewElement("system-layout");
        XMLElement* systemMarginsElement = doc.NewElement("system-margins");
        systemMarginsElement->InsertEndChild(NewTextElement(doc, "left-margin", ToString(currentSystem->layout.systemLeftMargin)));
        systemMarginsElement->InsertEndChild(NewTextElement(doc, "right-margin", ToString(currentSystem->layout.systemRightMargin)));
        systemLayoutElement->InsertEndChild(systemMarginsElement);

        systemLayoutElement->InsertEndChild(NewTextElement(doc, "system-distance", ToString(currentSystem->layout.systemDistance)));

        printElement->InsertEndChild(systemLayoutElement);

        measureElement->InsertEndChild(printElement);
    }

    if (index == 0)
    {
        XMLElement* attributesElement = doc.NewElement("attributes");
        attributesElement->InsertEndChild(NewTextElement(doc, "divisions", ToString(16)));

        measureElement->InsertEndChild(attributesElement);
    }

    int divisions = 16;

    struct TimedElement
    {
        XMLElement* element = nullptr;
        int timeInMeasure = 0;
    };
    
    std::vector<TimedElement> elements;

    for (auto lyric : measure->lyrics)
    {
        TimedElement newElement;
        newElement.element = ExportLyric(doc, lyric);
        newElement.timeInMeasure = lyric->beatPosition * divisions;
        elements.push_back(newElement);
    }

    for (auto chord : measure->chords)
    {
        TimedElement newElement;
        newElement.element = ExportChord(doc, chord);
        newElement.timeInMeasure = chord->beatPosition * divisions;
        elements.push_back(newElement);
    }

    for (auto direction : measure->textDirections)
    {
        TimedElement newElement;
        newElement.element = ExportTextDirection(doc, direction);
        newElement.timeInMeasure = direction->beatPosition * divisions;
        elements.push_back(newElement);
    }
    
    std::sort(elements.begin(), elements.end(), [](TimedElement a, TimedElement b)
        {
            return a.timeInMeasure < b.timeInMeasure;
        });
    
    int previousTime = 0;
    for (auto element : elements)
    {
        if (element.timeInMeasure > previousTime)
        {
            XMLElement* forwardElement = doc.NewElement("forward");
            forwardElement->InsertEndChild(NewTextElement(doc, "duration", ToString(element.timeInMeasure - previousTime)));
            measureElement->InsertEndChild(forwardElement);
            previousTime = element.timeInMeasure;
        }

        measureElement->InsertEndChild(element.element);
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

    // work
    XMLElement* work = doc.NewElement("work");
    XMLElement* workTitle = doc.NewElement("work-title");
    workTitle->SetText(song->songData.songTitle.c_str());
    work->InsertFirstChild(workTitle);

    root->InsertFirstChild(work);

    // credits
    for (auto credit : song->credits)
    {
        XMLElement* creditElement = doc.NewElement("credit");

        creditElement->SetAttribute("page", credit->pageNumber);

        XMLElement* creditTypeElement = doc.NewElement("credit-type");

        std::string creditTypeString = "";
        switch (credit->creditType)
        {
            case Credit::CreditType::PageNumber: creditTypeString = "page number"; break;
            case Credit::CreditType::Title: creditTypeString = "title"; break;
            case Credit::CreditType::Subtitle: creditTypeString = "subtitle"; break;
            case Credit::CreditType::Composer: creditTypeString = "composer"; break;
            case Credit::CreditType::Arranger: creditTypeString = "arranger"; break;
            case Credit::CreditType::Lyricist: creditTypeString = "lyricist"; break;
            case Credit::CreditType::Rights: creditTypeString = "rights"; break;
            case Credit::CreditType::PartName: creditTypeString = "part name"; break;
            case Credit::CreditType::Transcriber: creditTypeString = "transcriber"; break;

            case Credit::CreditType::None:
            default: creditTypeString = "composer"; break;
        }
        creditTypeElement->SetText(creditTypeString.c_str());

        creditElement->InsertEndChild(creditTypeElement);

        XMLElement* creditWordsElement = doc.NewElement("credit-words");

        creditWordsElement->SetText(credit->words.text.c_str());
        creditWordsElement->SetAttribute("default-x", credit->words.positionX);
        creditWordsElement->SetAttribute("default-y", DEFAULT_PAGE_HEIGHT - credit->words.positionY);
        HarmonyXMLExportHelper::SetTextualAttributes(creditWordsElement, credit->words);

        creditElement->InsertEndChild(creditWordsElement);

        root->InsertEndChild(creditElement);
    }

    // part list
    XMLElement* partList = doc.NewElement("part-list");
    
    for (const auto& instrument : song->instruments)
    {
        XMLElement* scorePart = doc.NewElement("score-part");

        scorePart->SetAttribute("id", instrument->id.c_str());

        scorePart->InsertEndChild(NewTextElement(doc, "part-name", instrument->name.string));
        if (instrument->nameAbbreviation.string != "")
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
                int systemIndex = -1;
                int i = 0;
                for (const auto& measure : staff->csStaff->measures)
                {
                    if (measure->isFirstMeasureOfSystem)
                    {
                        systemIndex++;
                    }

                    bool startsNewSystem = false;
                    if (i > 0)
                        startsNewSystem = song->systemMeasures[i - 1]->systemBreak;

                    if (startsNewSystem)
                        systemIndex++;

                    XMLElement* m = ExportMeasure(doc, measure, i, song->systems[systemIndex], startsNewSystem);
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