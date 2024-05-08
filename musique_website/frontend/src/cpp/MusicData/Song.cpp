#include "Song.h"

#include "SongData.h"
#include "../Utils/Converters.h"
#include "../Utils/Math.h"
#include "../Rendering/RenderData.h"

#include <algorithm>

#include <chrono>
using namespace std::chrono;

Song::~Song()
{
    LOGD("deconstructing song data");
    m_MeasureWidths.clear();
    m_MeasureBeginWidths.clear();
    m_MinNoteData.clear();
    LOGD("done deconstructing song data");
}

std::shared_ptr<Instrument> Song::GetInstrument(const std::string& id) const {
    for (auto& inst : instruments) {
        if (inst->id == id) {
            return inst;
        }
    }

    return nullptr;
}

void Song::OnLayoutChanged(Settings::MusicLayout layout)
{
    settings.musicLayout = layout;
    OnUpdate();
}

void Song::OnUpdate()
{
    // TODO: this is some pretty terrible looking code, needs fixed
    LOGI("Updating song data");

    for (const auto& instrument : instruments)
    {
        if (instrument->staves[0]->csStaff)
        {
            for (const auto& measure: instrument->staves[0]->csStaff->measures)
            {
                measure->Init(settings);
            }

            return;
        }
    }

    songData.instrumentInfos.clear();
    for (const auto& instrument: instruments)
    {
        InstrumentInfo instInfo = InstrumentInfo();
        instInfo.name = instrument->name.string;
        instInfo.volume = 100;
        instInfo.visible = true;
        songData.instrumentInfos.push_back(instInfo);
    }

    // beam calculations
    for (const auto& instrument: instruments)
    {
        for (const auto& staff: instrument->staves)
        {
            int measureIndex = 0;
            for (const auto& measure: staff->measures)
            {
                int noteIndex = 0;
                const int maxBeamLevel = 4; // TODO: probably should be fixed
                std::array<Beam, maxBeamLevel> beams; // temp to store/keep track of beams
                BeamGroup beamGroup; // temp to store/keep track of a beam group

                bool beamGroupIsDone = false;

                for (const auto& note: measure->notes)
                {
                    for (const NoteBeamData &noteBeamData: note->beamData)
                    {
                        if (noteBeamData.beamType == NoteBeamData::BeamType::Begin)
                        {
                            if (noteBeamData.beamLevel == 1) // start a new beam group
                            {
                                beamGroup = BeamGroup();

                                if (note->noteStem.stemType == NoteStem::StemType::Up)
                                    beamGroup.isAboveNote = true;
                            }

                            Beam beam = Beam();
                            beam.beamLevel = noteBeamData.beamLevel;
                            beam.notes.push_back(note);
                            if (noteBeamData.beamLevel == 1)
                                beamGroup.notes.push_back(note);
                            beam.beamType = Beam::BeamType::Normal;

                            beams[noteBeamData.beamLevel - 1] = beam;
                        } else if (noteBeamData.beamType == NoteBeamData::BeamType::End)
                        {
                            beams[noteBeamData.beamLevel - 1].notes.push_back(note);
                            if (noteBeamData.beamLevel == 1)
                                beamGroup.notes.push_back(note);

                            beamGroup.beams.push_back(beams[noteBeamData.beamLevel - 1]);

                            if (noteBeamData.beamLevel == 1) // the end of this beam group
                                beamGroupIsDone = true;
                        } else if (noteBeamData.beamType == NoteBeamData::BeamType::Continue)
                        {
                            beams[noteBeamData.beamLevel - 1].notes.push_back(note);
                            if (noteBeamData.beamLevel == 1)
                                beamGroup.notes.push_back(note);
                        } else if (noteBeamData.beamType == NoteBeamData::BeamType::ForwardHook)
                        {
                            Beam beam = Beam();
                            beam.beamLevel = noteBeamData.beamLevel;
                            beam.notes.push_back(note);
                            beam.beamType = Beam::BeamType::ForwardHook;

                            beamGroup.beams.push_back(beam);
                        } else if (noteBeamData.beamType == NoteBeamData::BeamType::BackwardHook)
                        {
                            Beam beam = Beam();
                            beam.beamLevel = noteBeamData.beamLevel;
                            beam.notes.push_back(note);
                            beam.beamType = Beam::BeamType::BackwardHook;

                            beamGroup.beams.push_back(beam);
                        } else
                        {
                            LOGW("Beam type is not supported yet.");
                        }
                    }

                    if (beamGroupIsDone)
                    {
                        measure->beams.push_back(beamGroup);
                        beamGroupIsDone = false;
                    }

                    noteIndex++;
                }

                measureIndex++;
            }
        }
    }

    // beat positions
    for (const auto& instrument: instruments)
    {
        for (const auto& staff: instrument->staves)
        {
            totalBeatWidth = staff->GetTotalBeatWidth();
            int measureIndex = 0;
            for (const auto& measure: staff->measures)
            {
                measure->InitBeatPosition(staff->GetMeasureBeatPosition(measureIndex));
                measureIndex++;
            }
        }
    }

    for (const auto& instrument : instruments)
    {
        for (const auto& staff : instrument->staves)
        {
            for (const auto& direction : staff->durationDirections)
            {
                direction->beatPositionInSongStart = direction->beatPositionStart + staff->GetMeasureBeatPosition(direction->startMeasureIndex);
                direction->beatPositionInSongEnd = direction->beatPositionEnd + staff->GetMeasureBeatPosition(direction->endMeasureIndex);
            }
        }
    }

    {
        int instrumentIndex = 0;
        for (const auto& instrument : instruments)
        {
            instrument->midiInstrument.channel = instrumentIndex;
            for (const auto& staff : instrument->staves)
            {
                for (const auto& measure : staff->measures)
                {
                    std::shared_ptr<Note> previousNote = nullptr;
                    for (const auto& note : measure->notes)
                    {
                        note->InitSound(previousNote);
                        previousNote = note;
                    }

                    previousNote = nullptr;
                    for (const auto& noteChord : measure->noteChords)
                    {
                        noteChord->InitSound(previousNote);
                        previousNote = noteChord->m_notes[0];
                    }
                }
            }

            instrumentIndex++;
        }
    }

    if (settings.musicLayout == Settings::MusicLayout::Paged)
    {
        //m_MinNoteData.clear();
        m_MeasureWidths.clear();
        m_MeasureBeginWidths.clear();
        LOGI("Paged");

        for (auto& credit : credits)
        {
            credit->CalculatePositionAsPaged();
        }

        for (const auto& endingGroup : endingGroups)
        {
            for (const auto& ending : endingGroup->endings)
            {
                if (GetSystemIndex(ending->startMeasureIndex) != GetSystemIndex(ending->endMeasureIndex))
                {
                    ending->isBroken = true;
                }

                if (!ending->isBroken) // only one segment
                {
                    std::shared_ptr<EndingSegment> segment = std::make_shared<EndingSegment>();
                    segment->startMeasureIndex = ending->startMeasureIndex;
                    segment->endMeasureIndex = ending->endMeasureIndex;

                    segment->isStart = true;
                    segment->isEnd = true;

                    ending->segments.push_back(segment);
                }
                else // multiple segments
                {
                    bool first = true;
                    int startSegmentMeasureIndex = ending->startMeasureIndex;
                    for (int i = ending->startMeasureIndex; i <= ending->endMeasureIndex; i++)
                    {
                        if (DoesMeasureStartNewSystem(i) && !first)
                        {
                            std::shared_ptr<EndingSegment> segment = std::make_shared<EndingSegment>();
                            segment->startMeasureIndex = startSegmentMeasureIndex;
                            segment->endMeasureIndex = i - 1;

                            ending->segments.push_back(segment);

                            startSegmentMeasureIndex = i;
                        }

                        first = false;
                    }

                    // add last segment
                    std::shared_ptr<EndingSegment> segment = std::make_shared<EndingSegment>();
                    segment->startMeasureIndex = startSegmentMeasureIndex;
                    segment->endMeasureIndex = ending->endMeasureIndex;

                    ending->segments.push_back(segment);

                    ending->segments[0]->isStart = true;
                    ending->segments[ending->segments.size() - 1]->isEnd = true;
                }

                if (ending)
                    ending->CalculatePositionAsPaged(displayConstants);
            }
        }

        // calculate positions for notes and measures
        for (const auto& instrument : instruments)
        {
            if (instrument->instrumentBracket)
                instrument->instrumentBracket->CalculateAsPaged(displayConstants, -18.0f);

            for (const auto& staff : instrument->staves)
            {
                staff->CalculateAsPaged(displayConstants);

                int measureIndex = 0;
                int systemIndex = -1;
                for (const auto& measure : staff->measures)
                {
                    if (measure->startNewSystem)
                        systemIndex++;

                    measure->CalculateAsPaged(displayConstants, settings, systems[systemIndex], staff->lines);
                    if (measureIndex >= m_MeasureWidths.size())
                    {
                        m_MeasureBeginWidths.push_back(0.0f);
                        m_MeasureWidths.push_back(measure->measureWidth);
                    }

                    if (staff->type == Staff::StaffType::Tab)
                        measure->showKeySignature = false;

                    //m_MeasureBeginWidths[measureIndex] = std::max(measure->GetBeginningWidth(), m_MeasureBeginWidths[measureIndex]);

                    /*systems[systemIndex].clefPositionX = std::max(measure->GetClefPositionInMeasure(systems[systemIndex]), systems[systemIndex].clefPositionX);
                    systems[systemIndex].keySignaturePositionX = std::max(measure->GetKeySignaturePositionInMeasure(systems[systemIndex], systems[systemIndex].clefPositionX), systems[systemIndex].keySignaturePositionX);
                    systems[systemIndex].timeSignaturePositionX = std::max(measure->GetTimeSignaturePositionInMeasure(systems[systemIndex], systems[systemIndex].keySignaturePositionX), systems[systemIndex].timeSignaturePositionX);*/

                    int noteIndex = 0;
                    for (const auto& note : measure->notes)
                    {
                        /*if (note->isRest && note->type == NoteType::Tab && softwareName == "MuseScore" && softwareMajorVersion == 4) // musescore only
                        {
                            note->defaultPosition.y = INVALID_FLOAT_VALUE;
                        }*/

                        if (note->isRest)
                        {
                            note->defaultPosition.y = INVALID_FLOAT_VALUE;
                        }

                        note->CalculatePositionAsPaged(displayConstants, staff->lines);

                        if (note->type == NoteType::Standard && !note->isRest)
                            note->position.y = (displayConstants.lineSpacing * measure->CalculateNoteYPositionRelativeToMeasure(noteIndex));

                        for (auto& lyric : note->lyrics)
                        {
                            lyric.CalculatePositionAsPaged(displayConstants);
                        }

                        noteIndex++;
                    }

                    for (const auto& noteChord : measure->noteChords)
                    {
                        noteChord->CalculatePositionAsPaged(displayConstants, staff->lines, measure, (softwareName == "MuseScore" && softwareMajorVersion == 4));
                    }

                    measureIndex++;
                }
            }
        }

        // calculate rest y positions
        for (const auto& instrument : instruments)
        {
            for (const auto& staff : instrument->staves)
            {
                for (const auto& measure : staff->measures)
                {
                    for (const auto& restNote : measure->notes)
                    {
                        if (restNote->isRest)
                        {
                            for (const auto& note : measure->notes)
                            {
                                if (restNote->beatPosition == note->beatPosition && restNote != note)
                                {
                                    restNote->position.y = note->position.y;

                                    float offset = 35.0f;

                                    if (note->noteStem.stemType == NoteStem::StemType::Up)
                                    {
                                        restNote->position.y += offset;
                                        break;
                                    }
                                    else if (note->noteStem.stemType == NoteStem::StemType::Down)
                                    {
                                        restNote->position.y -= offset;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // calculate width for multi measure rests
        for (const auto& instrument : instruments)
        {
            for (const auto& staff : instrument->staves)
            {
                std::vector<std::shared_ptr<Measure>> multiMeasureRests;
                int multiMeasureRestCount = 0;
                float totalMeasureWidth = 0.0f;

                std::shared_ptr<Measure> previousMultiMeasureRest = nullptr;
                std::shared_ptr<Measure> previousMeasure = nullptr;

                int measureIndex = 0;
                int systemIndex = 0;
                for (const auto& measure : staff->measures)
                {
                    if ((measure->startNewSystem && measureIndex != 0) || measureIndex == staff->measures.size() - 1)
                    {
                        if (measureIndex == staff->measures.size() - 1)
                        {
                            if (measure->isPartOfMultiMeasureRest)
                            {
                                measure->measureWidth = 0.0f;
                                m_MeasureWidths[measure->index] = measure->measureWidth;
                            }

                            totalMeasureWidth += measure->measureWidth;
                        }
                        //if (measure->startsMultiMeasureRest && measureIndex == staff->measures.size() - 1)
                        //{
                        //    multiMeasureRests.push_back(measure);
                        //    multiMeasureRestCount++;
                        //}

                        float systemWidth = displayConstants.pageWidth - systems[systemIndex]->layout.systemLeftMargin - systems[systemIndex]->layout.systemRightMargin - displayConstants.leftMargin - displayConstants.rightMargin;

                        if (previousMeasure)
                        {
                            if (previousMeasure->isPartOfMultiMeasureRest)
                            {
                                if (systemIndex + 1 < systems.size())
                                    systems[systemIndex + 1]->layout.systemDistance /= 4.0f;
                            }
                        }

                        float multiMeasureRestWidth = 0.0f;
                        if (multiMeasureRestCount != 0)
                        {
                            float remainingWidth = (systemWidth - totalMeasureWidth);
                            multiMeasureRestWidth = remainingWidth / (float) multiMeasureRestCount;
                        }

                        LOGV("systemLeftMargin: %f, systemRightMargin: %f, systemWidth: %f, remainingWidth: %f, multiMeasureRestWidth: %f", systems[systemIndex]->layout.systemLeftMargin, systems[systemIndex]->layout.systemRightMargin,
                             systemWidth, (systemWidth - totalMeasureWidth), multiMeasureRestWidth);

                        for (const auto& multiMeasureRest : multiMeasureRests)
                        {
                            multiMeasureRest->measureWidth = multiMeasureRestWidth;

                            m_MeasureWidths[multiMeasureRest->index] = multiMeasureRest->measureWidth;

                            if (multiMeasureRest->startsMultiMeasureRest && multiMeasureRest->multiMeasureRestSymbol != nullptr)
                            {
                                float lineSpacing;
                                if (multiMeasureRest->type == Measure::MeasureType::Tab)
                                    lineSpacing = displayConstants.tabLineSpacing;
                                else
                                    lineSpacing = displayConstants.lineSpacing;

                                auto measureDataItem = systems[systemIndex]->systemMeasureData.find(multiMeasureRest->index);

                                multiMeasureRest->multiMeasureRestSymbol->CalculateAsPaged(displayConstants, lineSpacing, staff->lines, multiMeasureRest->measureWidth, measureDataItem->second.measureBeginningWidth);
                            }
                        }

                        multiMeasureRests.clear();
                        multiMeasureRestCount = 0;
                        totalMeasureWidth = 0.0f;
                        systemIndex++;
                    }

                    if (measure->startsMultiMeasureRest)
                    {
                        multiMeasureRests.push_back(measure);
                        previousMultiMeasureRest = measure;
                        multiMeasureRestCount++;
                    }
                    else
                    {
                        /*if (previousMultiMeasureRest)
                        {
                            if (measure->index - previousMultiMeasureRest->index < previousMultiMeasureRest->numberOfMeasuresInMultiMeasureRest) // is part of multi measure rest
                            {

                            }
                        }*/

                        if (measure->isPartOfMultiMeasureRest)
                        {
                            measure->measureWidth = 0.0f;
                            m_MeasureWidths[measure->index] = measure->measureWidth;
                        }

                        totalMeasureWidth += measure->measureWidth;
                    }

                    previousMeasure = measure;
                    measureIndex++;
                }
            }
        }

        // sort all the notes by their beat positions and put them in an array of time space points
        m_TimeSpacePoints.clear();
        for (const auto& instrument : instruments) {
            for (const auto& staff : instrument->staves) {
                int measureIndex = 0;
                for (const auto& measure : staff->measures) {
                    TimeSpacePoint beginMeasurePoint = TimeSpacePoint();
                    beginMeasurePoint.beatPositionInSong = measure->beatPosition;
                    beginMeasurePoint.position = 0.0f;
                    beginMeasurePoint.measureIndex = measureIndex;
                    TimeSpacePoint endMeasurePoint = TimeSpacePoint();
                    endMeasurePoint.beatPositionInSong = measure->beatPosition + measure->duration.duration;
                    endMeasurePoint.position = measure->measureWidth;
                    endMeasurePoint.measureIndex = measureIndex;

                    //AddTimeSpacePoint(beginMeasurePoint);
                    AddTimeSpacePoint(endMeasurePoint);

                    for (const auto& note : measure->notes) {

                        //if (note->isRest)
                        //    continue;

                        int insertIndex = 0;
                        bool added = false;
                        for (TimeSpacePoint point : m_TimeSpacePoints) {
                            if (note->beatPositionInSong < point.beatPositionInSong) // note should come before this point
                            {
                                TimeSpacePoint newPoint = TimeSpacePoint();
                                newPoint.beatPositionInSong = note->beatPositionInSong;
                                newPoint.position = note->position.x;
                                newPoint.measureIndex = measureIndex;
                                m_TimeSpacePoints.insert(m_TimeSpacePoints.begin() + insertIndex, newPoint); // insert note at insertIndex
                                added = true;
                                break;
                            }
                            else if (note->beatPositionInSong == point.beatPositionInSong && note->measureIndex == point.measureIndex) // note is not needed to make a new point
                            {
                                added = true;
                                break;
                            } // else note should come after orderedNote

                            insertIndex++;
                        }

                        if (!added)
                        {
                            TimeSpacePoint newPoint = TimeSpacePoint();
                            newPoint.beatPositionInSong = note->beatPositionInSong;
                            newPoint.position = note->position.x;
                            newPoint.measureIndex = measureIndex;
                            m_TimeSpacePoints.push_back(newPoint);
                        }
                    }

                    measureIndex++;
                }
            }
        }

        // calculate positions for everything else
        for (const auto& instrument : instruments)
        {
            for (const auto& staff : instrument->staves)
            {
                float defY = -30.0f;
                for (const auto& direction : staff->durationDirections)
                {
                    if (GetSystemIndex(direction->startMeasureIndex) != GetSystemIndex(direction->endMeasureIndex))
                    {
                        direction->isBroken = true;
                    }

                    if (!direction->isBroken) // only one segment
                    {
                        DurationDirectionSegment segment;
                        segment.startMeasureIndex = direction->startMeasureIndex;
                        segment.endMeasureIndex = direction->endMeasureIndex;

                        segment.positionStart = { GetPositionXInMeasure(direction->beatPositionInSongStart, segment.startMeasureIndex), defY };
                        segment.positionEnd = { GetPositionXInMeasure(direction->beatPositionInSongEnd, segment.endMeasureIndex), defY };

                        direction->AddNewSegment(segment);
                    }
                    else // multiple segments
                    {
                        bool first = true;
                        int startSegmentMeasureIndex = direction->startMeasureIndex;
                        float startSegmentBeatPositionInSong = direction->beatPositionInSongStart;
                        for (int i = direction->startMeasureIndex; i <= direction->endMeasureIndex; i++)
                        {
                            if (DoesMeasureStartNewSystem(i) && !first)
                            {
                                DurationDirectionSegment segment;
                                segment.startMeasureIndex = startSegmentMeasureIndex;
                                segment.endMeasureIndex = i - 1;

                                segment.positionStart = { GetPositionXInMeasure(startSegmentBeatPositionInSong, segment.startMeasureIndex), defY };
                                segment.positionEnd = { GetPositionXInMeasure(staff->GetMeasureBeatPosition(segment.endMeasureIndex + 1), segment.endMeasureIndex), defY };

                                direction->AddNewSegment(segment);

                                startSegmentMeasureIndex = i;
                                startSegmentBeatPositionInSong = staff->GetMeasureBeatPosition(i);
                            }

                            first = false;
                        }

                        // add last segment
                        DurationDirectionSegment segment;
                        segment.startMeasureIndex = startSegmentMeasureIndex;
                        segment.endMeasureIndex = direction->endMeasureIndex;

                        segment.positionStart = { GetPositionXInMeasure(startSegmentBeatPositionInSong, segment.startMeasureIndex), defY };
                        segment.positionEnd = { GetPositionXInMeasure(direction->beatPositionInSongEnd, segment.endMeasureIndex), defY };

                        direction->AddNewSegment(segment);
                    }

                    direction->CalculateAsPaged(displayConstants);
                }

                int measureIndex = 0;
                for (const auto& measure : staff->measures)
                {
                    for (auto& direction : measure->directions)
                    {
                        float defaultX = GetPositionXInMeasure(direction.beatPositionInSong, measureIndex);
                        float defaultY = -30.0f;

                        for (auto& word : direction.words)
                        {
                            word.CalculatePositionAsPaged(displayConstants, defaultX, defaultY);
                        }

                        for (auto& rehearsals : direction.rehearsals)
                        {
                            rehearsals.CalculatePositionAsPaged(displayConstants, defaultX, defaultY);
                        }

                        if (direction.metronomeMark != nullptr)
                        {
                            direction.metronomeMark->CalculatePositionAsPaged(displayConstants, defaultX, defaultY);
                        }

                        for (auto& dynamic : direction.dynamics)
                        {
                            dynamic.CalculatePositionAsPaged(displayConstants, { defaultX, defaultY });
                        }

                        if (direction.marker != nullptr)
                        {
                            direction.marker->CalculatePositionAsPaged(displayConstants, { 0.0f, -20.0f });
                        }

                        std::shared_ptr<SoundEvent> soundEvent = direction.GetSoundEvent();

                        if (soundEvent)
                        {
                            measure->soundEvents.push_back(soundEvent);
                        }
                    }

                    for (auto& chord : measure->chords)
                    {
                        float defaultX = GetPositionXInMeasure(chord.beatPositionInSong, measureIndex);
                        float defaultY = -40.0f;

                        chord.CalculatePositionAsPaged(displayConstants, defaultX, defaultY);
                    }

                    float measureHeight = measure->GetMiddleHeight(staff->lines, staff->GetLineSpacing(displayConstants));
                    for (const auto& tuplet : measure->tuplets)
                    {
                        tuplet->CalculatePositionAsPaged(displayConstants, measureHeight, { 0.0f, 0.0f }, { 0.0f, 0.0f });
                    }

                    for (const auto& note : measure->notes)
                    {
                        if (note->isRest && (softwareName == "MuseScore" && softwareMajorVersion == 3))
                        {
                            note->position.x = GetPositionXInMeasure(note->beatPositionInSong, measureIndex);
                            if (note->isFullMeasureRest)
                            {
                                note->position.x = measure->measureWidth / 2.0f;
                            }
                        }

                        note->UpdateTieAndGlissSlide(measure->startNewSystem);
                    }

                    for (const auto& noteChord : measure->noteChords)
                    {
                        for (const auto& note : noteChord->m_notes)
                        {
                            note->UpdateTieAndGlissSlide(measure->startNewSystem);
                        }
                    }

                    // calculate beam positions
                    for (BeamGroup& beamGroup : measure->beams)
                    {
                        if (!beamGroup.notes.empty())
                        {
                            std::shared_ptr<Note> firstNote = beamGroup.notes[0];
                            std::shared_ptr<Note> lastNote = beamGroup.notes[beamGroup.notes.size() - 1];

                            beamGroup.beamStartPosition.x = firstNote->position.x + firstNote->noteStem.stemPositionX;
                            beamGroup.beamStartPosition.y = firstNote->position.y + firstNote->noteStem.stemEndY;

                            beamGroup.beamEndPosition.x = lastNote->position.x + lastNote->noteStem.stemPositionX;
                            beamGroup.beamEndPosition.y = lastNote->position.y + lastNote->noteStem.stemEndY;

                            float slope = std::abs(beamGroup.GetSlope());

                            if (slope > displayConstants.maxBeamSlope) // slope needs to be smaller
                            {
                                float run = beamGroup.beamEndPosition.x - beamGroup.beamStartPosition.x;
                                float maxRise = run * displayConstants.maxBeamSlope; // since: (run * maxslope) / run = maxslope

                                if (beamGroup.beamStartPosition.y > beamGroup.beamEndPosition.y)
                                {
                                    beamGroup.beamStartPosition.y = beamGroup.beamEndPosition.y + maxRise;
                                }
                                else if (beamGroup.beamEndPosition.y > beamGroup.beamStartPosition.y)
                                {
                                    beamGroup.beamEndPosition.y = beamGroup.beamStartPosition.y + maxRise;
                                }
                            }
                        }

                        for (Beam& beam : beamGroup.beams)
                        {
                            if (!beam.notes.empty())
                            {
                                std::shared_ptr<Note> firstNote = beam.notes[0];
                                std::shared_ptr<Note> lastNote = beam.notes[beam.notes.size() - 1];

                                beam.beamStartPosition.x = firstNote->position.x + firstNote->noteStem.stemPositionX;
                                beam.beamStartPosition.y = beamGroup.GetPositionYOnBeam(beam.beamStartPosition.x);

                                beam.beamEndPosition.x = lastNote->position.x + lastNote->noteStem.stemPositionX;
                                beam.beamEndPosition.y = beamGroup.GetPositionYOnBeam(beam.beamEndPosition.x);
                            }
                            else
                                LOGE("Beam has no notes associated with it");
                        }

                        // calculate stem lengths
                        float smallestStemHeight = INFINITY;
                        float direction = 1.0f;
                        if (beamGroup.notes[0]->noteStem.stemEndY < beamGroup.notes[0]->noteStem.stemStartY)
                            direction = -1.0f;

                        for (const std::shared_ptr<Note>& beamNote : beamGroup.notes)
                        {
                            float beamPositionYAtNote = beamGroup.GetPositionYOnBeam(beamNote->position.x + beamNote->noteStem.stemPositionX);
                            beamNote->noteStem.stemEndY = beamPositionYAtNote - beamNote->position.y;

                            float stemHeight = beamNote->noteStem.stemEndY - beamNote->noteStem.stemStartY;

                            if (std::abs(stemHeight) < smallestStemHeight)
                            {
                                smallestStemHeight = std::abs(stemHeight);
                            }
                        }

                        if (smallestStemHeight < displayConstants.minNoteStemHeight) // stems need to be longer
                        {
                            float heightOffset = displayConstants.minNoteStemHeight - smallestStemHeight; // how much to offset the beam
                            heightOffset *= direction;
                            heightOffset += 20.0f * direction; // TODO: temp

                            beamGroup.beamStartPosition.y += heightOffset;
                            beamGroup.beamEndPosition.y += heightOffset;

                            for (Beam& beam : beamGroup.beams)
                            {
                                beam.beamStartPosition.y += heightOffset;
                                beam.beamEndPosition.y += heightOffset;
                            }

                            // recalculate stem lengths
                            for (const std::shared_ptr<Note>& beamNote : beamGroup.notes)
                            {
                                float beamPositionYAtNote = beamGroup.GetPositionYOnBeam(beamNote->position.x + beamNote->noteStem.stemPositionX);
                                beamNote->noteStem.stemEndY = beamPositionYAtNote - beamNote->position.y;
                            }
                        }
                    }

                    measureIndex++;
                }
            }
        }

        /*for (const auto& measure : instruments[0]->staves[0]->measures)
        {
            if (measure->startNewPage)
            {
                Page page;
                page.start

                pages.push_back(page);
            }
        }*/
    }

    LOGI("Done updating song data!");
}

void Song::CalculateSystems()
{
    systems.clear();
    const float maxSystemWidth = displayConstants.pageWidth - (displayConstants.systemLayout.systemLeftMargin + displayConstants.systemLayout.systemRightMargin) - (displayConstants.leftMargin + displayConstants.rightMargin);

    float currentSystemWidth = 0.0f;
    std::shared_ptr<System> currentSystem = std::make_shared<System>();
    currentSystem->beginningMeasureIndex = 0;
    currentSystem->endingMeasureIndex = 0;
    System::SystemLayout defaultLayout;
    defaultLayout = settings.displayCosntants.systemLayout;

    currentSystem->layout = defaultLayout;
    for (const auto& instrument : instruments)
    {
        for (const auto& staff : instrument->staves)
        {
            if (staff->csStaff)
            {
                int measureIndex = 0;
                for (const auto& measure : staff->csStaff->measures)
                {
                    bool systemBreak = false;
                    if (measureIndex > 1)
                    {
                        systemBreak = systemMeasures[measureIndex - 1]->systemBreak;
                    }

                    if (currentSystemWidth + measure->GetTotalWidth() >= maxSystemWidth || systemBreak)
                    {
                        // push back system
                        currentSystem->endingMeasureIndex = measureIndex - 1;

                        for (int i = currentSystem->beginningMeasureIndex; i <= currentSystem->endingMeasureIndex; i++)
                        {
                            SystemMeasure systemMeasure;
                            systemMeasure.measureIndex = i;
                            currentSystem->systemMeasures.push_back(systemMeasure);
                        }

                        systems.push_back(currentSystem);
                        
                        // new system
                        currentSystem = std::make_shared<System>();
                        currentSystem->layout = defaultLayout;
                        currentSystem->beginningMeasureIndex = measureIndex;
                        currentSystemWidth = 0.0f;
                    }

                    currentSystemWidth += measure->GetTotalWidth();
                    measureIndex++;
                }


                // push back last system
                currentSystem->endingMeasureIndex = measureIndex - 1;

                for (int i = currentSystem->beginningMeasureIndex; i <= currentSystem->endingMeasureIndex; i++)
                {
                    SystemMeasure systemMeasure;
                    systemMeasure.measureIndex = i;
                    currentSystem->systemMeasures.push_back(systemMeasure);
                }

                systems.push_back(currentSystem);
            }
        }
    }
}

void Song::CalculateSystemPositionsAndPageBreaks()
{
    if (systems.empty() || instruments.empty())
        return;

    auto start = high_resolution_clock::now();
    for (const auto& instrument : instruments)
    {
        instrument->CalculateTotalBoundingBoxes(displayConstants, systems);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    LOGD("Time taken by position calculation in CalculateSystemPositionsAndPageBreaks func.: %lld milliseconds | %f seconds", duration.count(), duration.count() / 1000.0f);

    systemBoundingBoxes.clear();
    int systemIndex = 0;
    for (const auto& system : systems)
    {
        BoundingBox bb;

        bool boundingBoxNotSet = true;

        Vec2<float> previousInstPosition = { 0.0f, 0.0f };
        float previousInstBelowAndMiddleHeight = 0.0f;
        bool firstInst = true;
        for (const auto& instrument : instruments)
        {
            instrument->CalculateSystemPositionData(displayConstants, systemIndex, system->beginningMeasureIndex, system->endingMeasureIndex, firstInst, previousInstPosition, previousInstBelowAndMiddleHeight);
            BoundingBox instrumentBoundingBox = instrument->GetTotalBoundingBox(displayConstants, systemIndex);
            instrumentBoundingBox.position += instrument->systemPositionData[systemIndex];

            if (boundingBoxNotSet)
            {
                bb = instrumentBoundingBox;
                boundingBoxNotSet = false;
            }
            else
                bb = BoundingBox::CombineBoundingBoxes(bb, instrumentBoundingBox);
            firstInst = false;
        }

        for (const auto& endingGroup : endingGroups)
        {
            for (const auto& ending : endingGroup->endings)
            {
                if (DoBoundsCollide(ending->startMeasureIndex, ending->endMeasureIndex, system->beginningMeasureIndex, system->endingMeasureIndex))
                {
                    BoundingBox endingBoundingBox = ending->GetBoundingBoxRelativeToParent();
                    bb = BoundingBox::CombineBoundingBoxes(bb, endingBoundingBox);
                }
            }
        }

        systemBoundingBoxes.push_back(bb);
        systemIndex++;
    }

    // remove all page breaks
    for (const auto& instrument : instruments)
    {
        for (const auto& staff : instrument->staves)
        {
            for (const auto& measure : staff->measures)
            {
                measure->startNewPage = false;
            }
        }
    }

    pages.clear();

    // calculate system positions and page breaks
    CreatePageBreak(0);
    Page newPage;

    Vec2<float> previousSystemPosition = { 0.0f, displayConstants.topMargin + systems[0]->layout.topSystemDistance };
    float bottomLimit = displayConstants.pageHeight * (17.0f / 18.0f);
    int previousSystemIndex = -1;
    systemIndex = 0;
    bool startNewPage = true;
    float systemPadding = 5.0f;
    for (auto& system : systems)
    {
        if (previousSystemIndex >= 0 && !startNewPage)
        {
            float nextSystemPositionY = previousSystemPosition.y +
                                        GetSystemBoundingBox(previousSystemIndex).AddPadding(systemPadding).GetBottom();
            if (nextSystemPositionY + GetSystemBoundingBox(systemIndex).AddPadding(systemPadding).size.y >= bottomLimit)
            {
                float bottomOfLastSystem = systems[previousSystemIndex]->position.y + GetSystemBoundingBox(previousSystemIndex).GetBottom();
                float dist = bottomLimit - bottomOfLastSystem;
                if (newPage.systems.size() - 1 != 0)
                {
                    float extra = dist / (float) newPage.systems.size() - 1;
                    LOGD_TAG("Song", "extra: %f", extra);
                    for (int i = 0; i < newPage.systems.size(); i++)
                    {
                        newPage.systems[i]->position.y += extra * (float)i;
                    }
                }

                // start a new page
                LOGD_TAG("Song", "Starting new page");
                startNewPage = true;
                previousSystemPosition = { 0.0f, displayConstants.topMargin + systems[systemIndex]->layout.topSystemDistance };
                CreatePageBreak(system->beginningMeasureIndex);
                pages.push_back(newPage);
                newPage = Page();
            }
        }

        system->position.x = displayConstants.leftMargin + system->layout.systemLeftMargin;
        system->position.y = previousSystemPosition.y;

        if (previousSystemIndex >= 0 && !startNewPage)
        {
            system->position.y += GetSystemBoundingBox(previousSystemIndex).AddPadding(systemPadding).GetBottom();
            system->position.y += -GetSystemBoundingBox(systemIndex).AddPadding(systemPadding).GetTop();
        }

        LOGD_TAG("Song", "Pushing back system!!!!!!!!!!!!!!!!");
        newPage.systems.push_back(system);
        previousSystemPosition = system->position;
        previousSystemIndex = systemIndex;
        systemIndex++;
        startNewPage = false;
    }

    pages.push_back(newPage);

    // page numbers
    Vec2<float> pageNumberDefaultPosition = { displayConstants.pageWidth / 2.0f, displayConstants.pageHeight * (17.0f / 18.0f) };
    int i = 0;
    for (auto& page : pages)
    {
        PageNumber newPageNumber(i + 1);

        newPageNumber.CalculatePosition(displayConstants, pageNumberDefaultPosition);

        page.pageNumber = newPageNumber;

        i++;
    }
}

void Song::AddTimeSpacePoint(TimeSpacePoint point)
{
    bool added = false;
    int insertIndex = 0;
    for (TimeSpacePoint compare : m_TimeSpacePoints) {
        if (point.beatPositionInSong < compare.beatPositionInSong) // point should come before compare
        {
            m_TimeSpacePoints.insert(m_TimeSpacePoints.begin() + insertIndex, point); // insert point at insertIndex
            added = true;
            break;
        }
        else if (point.beatPositionInSong == compare.beatPositionInSong) // point is not needed
        {
            added = true;
            break;
        } // else point should come after compare

        insertIndex++;
    }

    if (!added)
        m_TimeSpacePoints.push_back(point);
}

float Song::GetNoteMinWidthInFront(const std::shared_ptr<Note>& note) const
{
    return note->GetMinWidth();
}

float Song::GetNoteMinWidthBehind(const std::shared_ptr<Note>& note) const
{
    return 0.0f;
}

float Song::GetMeasureWidth(int measureIndex) const
{
    if (m_MeasureWidths.size() > measureIndex) {
        return m_MeasureWidths[measureIndex];
    }
    return 0.0f;
}

float Song::GetMeasurePositionX(int measureIndex) const
{
    float position = 0.0f;
    if (DoesMeasureStartNewSystem(measureIndex))
        return position;
    else
    {
        int systemIndex = GetSystemIndex(measureIndex);
        int firstMeasureInSystemIndex = GetFirstMeasureInSystem(systemIndex);

        for (int i = firstMeasureInSystemIndex; i < measureIndex; i++)
        {
            position += GetMeasureWidth(i);
        }

        return position;
    }
}

float Song::GetMeasureBeatPosition(int measureIndex) const
{
    float beatPosition = 0.0f;
    if (!instruments.empty())
    {
        if (!instruments[0]->staves.empty())
        {
            beatPosition = instruments[0]->staves[0]->GetMeasureBeatPosition(measureIndex);
        }
    }
    return beatPosition;
}

float Song::GetMeasurePositionY(int measureIndex) const
{
    return 0.0f;
}

float Song::GetSongWidth() const
{
    float songWidth = 0.0f;
    for (float w : m_MeasureWidths)
    {
        songWidth += w;
    }
    return songWidth;
}

float Song::GetPositionXInMeasure(float beatPositionInSong, int currentMeasureIndex) const
{
    float position = -1.0f;

    if (settings.musicLayout == Settings::MusicLayout::Paged)
    {
        //LOGD("GetPositionXInMeasure! %f", beatPositionInSong);
        TimeSpacePoint previousPoint = TimeSpacePoint();
        for (const TimeSpacePoint& point : m_TimeSpacePoints)
        {
            if (currentMeasureIndex == point.measureIndex)
            {
                if (FloatsAreEqual(beatPositionInSong, point.beatPositionInSong))
                {
                    position = point.position;
                    //LOGE("beatPositionInSong == point.beatPosition: %f", position);
                    //LOGE("beatPositionInSong == point.beatPosition: bpos: %f, pos: %f, pointBP: %f, prePointBP: %f, pMI: %d, prePMI: %d, pointP: %f, prePointP: %f", beatPositionInSong, position, point.beatPosition, previousPoint.beatPosition, point.measureIndex, previousPoint.measureIndex, point.position, previousPoint.position);

                    break;
                }
                else if (beatPositionInSong < point.beatPositionInSong)
                {
                    float previousBeatPosition = previousPoint.beatPositionInSong;
                    float previousPosition = previousPoint.position;
                    if (currentMeasureIndex != previousPoint.measureIndex)
                    {
                        previousBeatPosition = GetMeasureBeatPosition(currentMeasureIndex);
                        previousPosition = 0.0f;
                    }

                    float posRange = (point.beatPositionInSong - previousBeatPosition);
                    if (posRange == 0)
                        LOGE("divide by 0!!!!!!!!!!");
                    float percentage = (beatPositionInSong - previousBeatPosition) / posRange;
                    position = (point.position - previousPosition) * percentage;
                    position += previousPosition;

                    //LOGE("beatPositionInSong < point.beatPosition: bpos: %f, pos: %f, posRange: %f, percentage: %f, pointBP: %f, prePointBP: %f, pMI: %d, prePMI: %d, pointP: %f, prePointP: %f", beatPositionInSong, position, posRange, percentage, point.beatPosition, previousPoint.beatPosition, point.measureIndex, previousPoint.measureIndex, point.position, previousPoint.position);

                    break;
                }

                /*if (beatPositionInSong < point.beatPosition)
                {
                    float posRange = (point.beatPosition - previousPoint.beatPosition);
                    if (posRange == 0)
                        LOGE("divide by 0!!!!!!!!!!");
                    float percentage = (beatPositionInSong - previousPoint.beatPosition) / posRange;
                    position = (point.position - previousPoint.position) * percentage;

                    LOGE("beatPositionInSong < point.beatPosition: bpos: %f, pos: %f, posRange: %f, percentage: %f, pointBP: %f, prePointBP: %f, pointP: %f, prePointP: %f", beatPositionInSong, position, posRange, percentage, point.beatPosition, previousPoint.beatPosition, point.position, previousPoint.position);

                    break;
                }
                else if (beatPositionInSong == point.beatPosition) // TODO: fix dangerous comparison
                {
                    position = point.position;
                    LOGE("beatPositionInSong == point.beatPosition: %f", position);
                    break;
                }*/
            }

            previousPoint = point;
        }
    }
    else
    {
        position += m_MeasureBeginWidths[currentMeasureIndex];
        // add the measure's beginning width <--

        /*float measureBeatPosition = 0.0f;
        if (!instruments.empty())
        {
            if (!instruments[0]->staves.empty())
            {
                if (!instruments[0]->staves[0]->measures.empty())
                {
                    measureBeatPosition = instruments[0]->staves[0]->measures[0]->beatPosition;
                }
            }
        }*/

        float pos = 0.0f; // debug var
        float time = 0.0f;

        float previousNoteWidth = 0.0f;
        float previousNoteBeatPosition = 0.0f;
        float previousNoteDuration = 0.0f;
        for (auto noteData: m_MinNoteData)
        {
            if (time >= GetMeasureBeatPosition(currentMeasureIndex)) // should be equal first time that it is true
            {
                if (previousNoteBeatPosition == 0.0f) // first time, set to the start of the measure
                {
                    previousNoteBeatPosition = GetMeasureBeatPosition(currentMeasureIndex);
                }

                if (noteData.beatPositionInSong >= beatPositionInSong) // noteData.beatPositionInSong is past(or equal to) beatPositionInSong(what we want the x position of)
                {
                    if (previousNoteDuration == 0.0f) // noteData is the first in the list so the position is just 0.0f
                    {
                        break; // done
                    }

                    // calculates how much of the previous note's width should be taken away from the position
                    float diff = (beatPositionInSong - previousNoteBeatPosition); // num of beats between beatPositionInSong and previousNoteBeatPosition
                    float howMuch = (diff / previousNoteDuration); // the percentage of previousNoteWidth that needs to be added to previousNoteX to get the x of beatPositionInSong
                    float howMuchToSub = 1.0f - howMuch; // the percentage of previousNoteWidth that needs to be subtracted from currentNoteX to get the x of beatPositionInSong
                    position -= (previousNoteWidth * howMuchToSub); // get the percentage of previousNoteWidth and subtract
                    break; // done
                }

                position += noteData.width;
                previousNoteBeatPosition = noteData.beatPositionInSong;
                previousNoteDuration = noteData.duration;
            }
            pos += noteData.width;
            time += noteData.duration;
            previousNoteWidth = noteData.width;
        }
    }

    return position;
}

float Song::GetPositionXInSong(float beatPositionInSong, int currentMeasureIndex) const
{
    float position = GetPositionXInMeasure(beatPositionInSong, currentMeasureIndex);

    float measurePositionX = GetMeasurePositionX(currentMeasureIndex);
    position += measurePositionX;

    return position;
}

std::shared_ptr<Measure> Song::GetMeasure(int measureIndex) const
{
    if (!instruments.empty())
        if (!instruments[0]->staves.empty())
            if (instruments[0]->staves[0]->measures.size() > measureIndex)
                return instruments[0]->staves[0]->measures[measureIndex];

    ASSERT_MSG(false, "Cannot get measure");
}

bool Song::DoesMeasureStartNewSystem(int measureIndex) const
{
    for (const auto& system : systems)
    {
        if (measureIndex >= system->beginningMeasureIndex && measureIndex <= system->endingMeasureIndex)
        {
            if (measureIndex == system->beginningMeasureIndex)
                return true;

            return false;
        }
    }

    return false;

    /*ASSERT(measureIndex >= 0 && measureIndex < systemMeasures.size());

    if (measureIndex == 0)
        return true;

    return systemMeasures[measureIndex - 1]->systemBreak;*/
}

bool Song::DoesMeasureStartNewPage(int measureIndex) const
{
    for (const auto& page : pages)
    {
        if (!page.systems.empty())
        {
            if (page.systems[0]->beginningMeasureIndex == measureIndex)
            {
                return true;
            }
        }
    }

    return false;
}

float Song::GetSystemPositionY(int measureIndex) const // TODO: needs finished
{
    float instYPosition = 0.0f;
    int instrumentIndex = 0;
    std::shared_ptr<Instrument> prevInstrument = nullptr;
    for (const auto& instrument: instruments) {

        if (songData.instrumentInfos[instrumentIndex].visible) {
            if (prevInstrument != nullptr)
                instYPosition += prevInstrument->GetMiddleHeight(10.0f, 13.33f, 0,
                                                                 instrument->GetMeasureCount()) +
                                 prevInstrument->GetBelowHeight(10.0f, 13.33f, 0,
                                                                instrument->GetMeasureCount());
            instYPosition += instrument->GetAboveHeight(10.0f, 13.33f, 0,
                                                        instrument->GetMeasureCount());

            prevInstrument = instrument;
        } // instrument is visible
        instrumentIndex++;
    } // instruments loop

    instYPosition += 100.0f;
    return instYPosition * (measureIndex/4);
}

float Song::GetSystemHeight(int systemIndex) const
{
    ASSERT(systemIndex < systems.size());

    std::shared_ptr<System> system = systems[systemIndex];

    std::shared_ptr<Instrument> bottomInstrument = instruments[instruments.size() - 1];
    float height = bottomInstrument->systemPositionData[systemIndex].y;

    float instHeight = bottomInstrument->staves[bottomInstrument->staves.size() - 1]->systemPositionData[systemIndex].y;
    float lineSpacing = displayConstants.lineSpacing;
    if (bottomInstrument->staves[bottomInstrument->staves.size() - 1]->type == Staff::StaffType::Tab)
        lineSpacing = displayConstants.tabLineSpacing;
    instHeight += bottomInstrument->staves[bottomInstrument->staves.size() - 1]->GetMiddleHeight(lineSpacing);

    height += instHeight;

    return height;
}

BoundingBox Song::GetSystemBoundingBox(int systemIndex) const
{
    ASSERT(systemIndex < systemBoundingBoxes.size());
    return systemBoundingBoxes[systemIndex];
}

void Song::CreatePageBreak(int measureIndex) const
{
    ASSERT(measureIndex >= 0);

    for (const auto& instrument : instruments)
    {
        for (const auto& staff : instrument->staves)
        {
            if (staff->type != Staff::StaffType::ChordSheet)
            {
                ASSERT(measureIndex < staff->measures.size());

                staff->measures[measureIndex]->startNewPage = true;
                staff->measures[measureIndex]->startNewSystem = true; // a new page always starts a new system
            }
        }
    }
}

void Song::RemovePageBreak(int measureIndex) const
{

}

int Song::GetSystemIndex(int measureIndex) const
{
    int systemIndex = 0;
    for (const auto& system : systems)
    {
        if (measureIndex >= system->beginningMeasureIndex && measureIndex <= system->endingMeasureIndex)
        {
            return systemIndex;
        }

        systemIndex++;
    }

    return 0;
}

int Song::GetPageIndex(int measureIndex) const
{
    int i = 0;
    for (const Page& page : pages)
    {
        ASSERT_MSG(!page.systems.empty(), "No systems on page");

        if (page.systems[0]->beginningMeasureIndex <= measureIndex && measureIndex <= page.systems.back()->endingMeasureIndex)
            return i;

        i++;
    }

    ASSERT(false);
}

int Song::GetFirstMeasureOnPage(uint16_t pageIndex) const
{
    int measureIndex = 0;

    ASSERT(pageIndex <= pages.size());

    Page page = pages[pageIndex];

    ASSERT(!page.systems.empty());

    return page.systems[0]->beginningMeasureIndex;
}

int Song::GetFirstMeasureInSystem(int systemIndex) const
{
    int measureIndex = 0;

    if (instruments.empty())
        return 0;
    if (instruments[0]->staves.empty())
        return 0;

    int i = -1;
    for (const auto& measure : instruments[0]->staves[0]->measures)
    {
        if (measure->startNewSystem)
            i++;

        if (i == systemIndex)
            break;

        measureIndex++;
    }

    return measureIndex;
}

int Song::GetNumPages() const
{
    /*int numPages = 1;

    if (instruments.empty())
        return numPages;
    if (!instruments[0])
        return numPages;
    if (instruments[0]->staves.empty())
        return numPages;
    if (!instruments[0]->staves[0])
        return numPages;

    for (const auto& measure : instruments[0]->staves[0]->measures)
    {
        if (measure)
        {
            if (measure->startNewPage)
                numPages++;
        }
    }

    return numPages - 1;*/

    return pages.size();
}

/*float Song::GetMeasurePositionX(int measureIndex, Vec2<float> pagePosition, Vec2<float> systemPosition)
{
    if (DoesMeasureStartNewSystem(measureIndex)) // if it is the first measure in the system
        return pagePosition.x + systemPosition.x;

    float measurePositionX = 0.0f;

    int systemIndex = GetSystemIndex(measureIndex);
    int firstMeasureInSystemIndex = GetFirstMeasureInSystem(systemIndex);

    int nextMeasureIndex = firstMeasureInSystemIndex;
    float currentPositionX = systemPosition.x;

    while (true)
    {
        if (nextMeasureIndex == measureIndex)
        {
            break;
        }

        float width = GetMeasureWidth(nextMeasureIndex);

        currentPositionX += width;

        nextMeasureIndex++;

        if (nextMeasureIndex - firstMeasureInSystemIndex >= 100) // probably a infinite loop
        {
            LOGE("INFINITE LOOP");
            break;
        }
    }

    measurePositionX = currentPositionX;

    return measurePositionX;
}*/

float Song::GetInstrumentPositionY(int measureIndex, int instrumentIndex) const
{
    float instrumentPositionY = 0.0f;

    std::shared_ptr<Instrument> prevInstrument = nullptr;
    int currentInstrumentIndex = 0;
    for (const auto& instrument : instruments)
    {
        if (prevInstrument != nullptr)
        {
            int firstMeasureIndex = GetFirstMeasureInSystem(GetSystemIndex(measureIndex));
            instrumentPositionY += prevInstrument->GetMiddleHeight(10.0f, 13.33f, 0, instrument->GetMeasureCount());
            instrumentPositionY += instrument->staves.front()->measures[firstMeasureIndex]->staffDistance;
        }

        if (currentInstrumentIndex == instrumentIndex)
        {
            break;
        }

        prevInstrument = instrument;
        currentInstrumentIndex++;
    }

    return instrumentPositionY;
}

void Song::UpdateBoundingBoxes(const std::vector<Vec2<float>>& pagePositions, const std::vector<Vec2<float>>& systemPositions)
{
    ASSERT(!pagePositions.empty());
    ASSERT(!systemPositions.empty());

    int instrumentIndex = 0;
    for (const auto& instrument : instruments) {

        int staffIndex = 0;
        for (const auto& staff : instrument->staves) {
            float ls = displayConstants.lineSpacing;
            if (staff->type == Staff::StaffType::Tab) {
                ls = displayConstants.tabLineSpacing;
            }

            staff->UpdateBoundingBoxes(displayConstants);

            for (const auto& direction : staff->durationDirections)
            {
                for (int i = 0; i < direction->GetSegmentCount(); i++)
                {
                    std::shared_ptr<DurationDirectionSegment> segment = direction->GetSegment(i);

                    int pageIndex = GetPageIndex(segment->startMeasureIndex);
                    int systemIndex = GetSystemIndex(segment->startMeasureIndex);
                    Vec2<float> pagePosition = pagePositions[pageIndex];
                    Vec2<float> systemPosition = systemPositions[systemIndex];

                    float startMeasurePositionX = GetMeasurePositionX(segment->startMeasureIndex) + systemPosition.x + pagePosition.x;
                    float endMeasurePositionX = GetMeasurePositionX(segment->endMeasureIndex) + systemPosition.x + pagePosition.x;

                    // staff y position
                    float staffPositionY = 0.0f;
                    if (staffIndex == 0) {
                        staffPositionY = 0.0f;
                    } else if (staffIndex >= 1) {
                        staffPositionY += staff->measures[segment->startMeasureIndex]->staffDistance + instrument->staves[staffIndex - 1]->GetMiddleHeight(ls);
                    }

                    float instPositionY = GetInstrumentPositionY(segment->startMeasureIndex, instrumentIndex) + systemPosition.y + pagePosition.y;

                    segment->UpdateBoundingBox(staffPositionY + instPositionY, startMeasurePositionX, endMeasurePositionX);
                }
            }

            int measureIndex = 0;
            for (const auto& measure : staff->measures) {

                float instPositionY = GetInstrumentPositionY(measureIndex, instrumentIndex) + systemPositions[GetSystemIndex(measureIndex)].y + pagePositions[GetPageIndex(measureIndex)].y;
                //float instPositionY = systemPositions[GetSystemIndex(measureIndex)].y;

                // staff y position
                float staffPositionY = 0.0f;
                if (staffIndex == 0) {
                    staffPositionY = 0.0f;
                } else if (staffIndex >= 1) {
                    staffPositionY += staff->measures[measureIndex]->staffDistance + instrument->staves[staffIndex - 1]->GetMiddleHeight(ls);
                }

                //LOGD("instPosY: %f, staffPosY: %f", instPositionY, staffPositionY);

                int pageIndex = GetPageIndex(measureIndex);
                int systemIndex = GetSystemIndex(measureIndex);
                if (pageIndex < pagePositions.size() && systemIndex < systemPositions.size())
                {
                    Vec2<float> pagePosition = pagePositions[pageIndex];
                    Vec2<float> systemPosition = systemPositions[systemIndex];

                    //LOGD("pagePosition: x: %f, y: %f, systemPosition: x: %f, y: %f", pagePosition.x, pagePosition.y, systemPosition.x, systemPosition.y);

                    float measurePositionX = GetMeasurePositionX(measureIndex);

                    float measureHeight;

                    if (staff->type == Staff::StaffType::Standard)
                    {
                        measureHeight = float(staff->lines - 1) * displayConstants.lineSpacing;
                    }
                    else
                    {
                        measureHeight = float(staff->lines - 1) * displayConstants.tabLineSpacing;
                    }

                    //LOGW("measurePositionX: %f", measurePositionX);
                    measure->UpdateBoundingBoxes(displayConstants, {measurePositionX + systemPosition.x + pagePosition.x, instPositionY + staffPositionY }, measureHeight); // this line crashes
                }

                measureIndex++;
            }

            staffIndex++;
        }

        instrumentIndex++;
    }
}

void Song::RenderBoundingBoxes(RenderData& renderData, const std::vector<Vec2<float>>& pagePositions, const std::vector<Vec2<float>>& systemPositions)
{
    for (const auto& instrument : instruments)
    {
        for (const auto& staff : instrument->staves)
        {
            staff->RenderDebug(renderData);
        }
    }

    int systemIndex = 0;
    for (const auto& system : systems)
    {
        //bb.size.y = 40.0f;

        int instrumentIndex = 0;
        for (const auto& instrument : instruments)
        {
            int staffIndex = 0;
            for (const auto& staff : instrument->staves)
            {
                int start = system->beginningMeasureIndex;
                int end = system->endingMeasureIndex;

                BoundingBox bb = staff->GetTotalBoundingBox(displayConstants, systemIndex);
                bb.position += systemPositions[systemIndex];
                bb.position += instrument->systemPositionData[systemIndex];
                bb.position += staff->systemPositionData[systemIndex];

                bb.size.x = 1050.0f;

                /*bb.size.y = staff->GetTotalHeight(displayConstants, start, end);
                bb.position.y -= staff->GetAboveHeight(displayConstants, start, end);*/

                bb.Render(renderData, (int)0xFF00FF00);

                staffIndex++;
            }

            BoundingBox instrumentBoundingBox = instrument->GetTotalBoundingBox(displayConstants, systemIndex);
            //instrumentBoundingBox.position.y -= instrument->GetAboveHeight(displayConstants, start, end);
            instrumentBoundingBox.position += systemPositions[systemIndex];
            instrumentBoundingBox.position += instrument->systemPositionData[systemIndex];
            instrumentBoundingBox.size.x = 1050.0f;
            instrumentBoundingBox.Render(renderData, (int)0xFFFF0000);

            instrumentIndex++;
        }

        BoundingBox systemBoundingBox = GetSystemBoundingBox(systemIndex);
        systemBoundingBox.position += systemPositions[systemIndex];
        systemBoundingBox.size.x = 1050.0f;
        systemBoundingBox.Render(renderData, (int)0xFF0000FF);

        systemIndex++;
    }

    {
        systemIndex = 0;
        for (const auto& system : systems)
        {
            int instrumentIndex = 0;
            for (const auto& instrument : instruments)
            {
                int staffIndex = 0;
                for (const auto& staff : instrument->staves)
                {
                    int start = system->beginningMeasureIndex;
                    int end = system->endingMeasureIndex;

                    BoundingBox bb = staff->GetTotalBoundingBox(displayConstants, systemIndex);
                    bb.position += systemPositions[systemIndex];
                    bb.position += instrument->systemPositionData[systemIndex];
                    bb.position += staff->systemPositionData[systemIndex];

                    bb.position.x = systemPositions[systemIndex].x;
                    bb.size.x = 1050.0f;

                    /*for (int m = start; m <= end; m++)
                    {
                        BoundingBox boundingBox = bb;

                        boundingBox.position.x += GetMeasurePositionX(m);
                        boundingBox.size.x = staff->measures[m]->boundingBox.size.x;

                        boundingBox.Render(renderData, 0xFFFF00FF);
                    }*/

                    staffIndex++;
                }

                instrumentIndex++;
            }

            systemIndex++;
        }
    }
}

void Song::ResolveCollisions()
{
    for (const auto& instrument : instruments)
    {
        for (const auto& staff : instrument->staves)
        {

            for (const auto& direction : staff->durationDirections)
            {
                for (int i = 0; i < direction->GetSegmentCount(); i++)
                {
                    std::shared_ptr<DurationDirectionSegment> segment = direction->GetSegment(i);
                    BoundingBox oldBoundingBox = segment->boundingBox;
                    ResolveCollisionsWith(segment->boundingBox, GetPageIndex(segment->startMeasureIndex));
                    segment->Move(segment->boundingBox.position - oldBoundingBox.position);
                }

                direction->Move({ 0.0f, 0.0f }); // TODO: this is just an easy fix for bracket directions (needs to be removed)
            }

            int measureIndex = 0;
            int pageIndex = -1;
            for (const auto& measure : staff->measures)
            {

                if (DoesMeasureStartNewPage(measureIndex))
                    pageIndex++;

                ResolveCollisionsWith(measure->boundingBox, pageIndex);
                ResolveCollisionsWith(measure->measureNumber.boundingBox, pageIndex);
                ResolveCollisionsWith(measure->clef.boundingBox, pageIndex);

                for (const auto& note : measure->notes)
                {
                    ResolveCollisionsWith(note->boundingBox, pageIndex);

                    if (note->fermata)
                    {
                        BoundingBox oldBoundingBox = note->fermata->boundingBox;
                        ResolveCollisionsWith(note->fermata->boundingBox, pageIndex);
                        note->fermata->Move(note->fermata->boundingBox.position - oldBoundingBox.position);
                    }

                    for (const auto& ornament : note->ornaments)
                    {
                        if (ornament)
                        {
                            BoundingBox oldBoundingBox = ornament->boundingBox;
                            ResolveCollisionsWith(ornament->boundingBox, pageIndex);
                            ornament->Move(ornament->boundingBox.position - oldBoundingBox.position);
                        }
                    }

                    for (auto& lyric : note->lyrics)
                    {
                        ResolveCollisionsWith(lyric.boundingBox, pageIndex);
                    }
                }

                for (const auto& noteChord : measure->noteChords)
                {
                    ResolveCollisionsWith(noteChord->boundingBox, pageIndex);
                }

                for (auto& beamGroup : measure->beams)
                {
                    ResolveCollisionsWith(beamGroup.boundingBox, pageIndex);
                }

                for (const auto& tuplet : measure->tuplets)
                {
                    ResolveCollisionsWith(tuplet->boundingBox, pageIndex);
                }

                for (Direction& direction : measure->directions) {

                    for (auto& words : direction.words)
                    {
                        BoundingBox oldBoundingBox = words.boundingBox;
                        ResolveCollisionsWith(words.boundingBox, pageIndex);
                        words.Move(words.boundingBox.position - oldBoundingBox.position);
                    }

                    for (auto& rehearsal : direction.rehearsals)
                    {
                        BoundingBox oldBoundingBox = rehearsal.boundingBox;
                        ResolveCollisionsWith(rehearsal.boundingBox, pageIndex);
                        rehearsal.Move(rehearsal.boundingBox.position - oldBoundingBox.position);
                    }

                    for (auto& dynamic : direction.dynamics)
                    {
                        BoundingBox oldBoundingBox = dynamic.boundingBox;
                        ResolveCollisionsWith(dynamic.boundingBox, pageIndex);
                        dynamic.Move(dynamic.boundingBox.position - oldBoundingBox.position);
                    }

                    if (direction.dynamicWedge != nullptr)
                    {
                        BoundingBox oldBoundingBox = direction.dynamicWedge->boundingBox;
                        ResolveCollisionsWith(direction.dynamicWedge->boundingBox, pageIndex);
                        direction.dynamicWedge->Move(direction.dynamicWedge->boundingBox.position - oldBoundingBox.position);
                    }

                    if (direction.bracketDirection != nullptr)
                    {
                        BoundingBox oldBoundingBox = direction.bracketDirection->boundingBox;
                        ResolveCollisionsWith(direction.bracketDirection->boundingBox, pageIndex);
                        direction.bracketDirection->Move(direction.bracketDirection->boundingBox.position - oldBoundingBox.position);
                    }

                    if (direction.metronomeMark != nullptr)
                    {
                        BoundingBox oldBoundingBox = direction.metronomeMark->boundingBox;
                        ResolveCollisionsWith(direction.metronomeMark->boundingBox, pageIndex);
                        direction.metronomeMark->Move(direction.metronomeMark->boundingBox.position - oldBoundingBox.position);
                    }

                    if (direction.marker != nullptr)
                    {
                        BoundingBox oldBoundingBox = direction.marker->boundingBox;
                        ResolveCollisionsWith(direction.marker->boundingBox, pageIndex);
                        direction.marker->Move(direction.marker->boundingBox.position - oldBoundingBox.position);
                    }
                }

                for (Chord& chord : measure->chords)
                {
                    BoundingBox oldBoundingBox = chord.boundingBox;
                    ResolveCollisionsWith(chord.boundingBox, pageIndex);
                    chord.Move(chord.boundingBox.position - oldBoundingBox.position);
                }

                measureIndex++;
            }
        }
    }
}

void Song::ResolveCollisionsWith(BoundingBox& box, int pageIndex)
{
    for (const auto& instrument : instruments) {
        for (const auto& staff : instrument->staves) {
            int measureIndex = 0;
            int currentPageIndex = -1;

            for (const auto& direction : staff->durationDirections)
            {
                for (int i = 0; i < direction->GetSegmentCount(); i++)
                {
                    ResolveCollisionWith(direction->GetSegment(i), box);
                }

                direction->Move({ 0.0f, 0.0f }); // TODO: this is just an easy fix for bracket directions (needs to be removed)
            }

            for (const auto& measure : staff->measures) {

                if (measure->startNewPage)
                    currentPageIndex++;

                if (currentPageIndex == pageIndex)
                {
                    for (const auto& note : measure->notes)
                    {
                        for (const auto& ornament : note->ornaments)
                        {
                            if (ornament)
                            {
                                ResolveCollisionWith(ornament, box);
                            }
                        }

                        for (auto& lyric : note->lyrics)
                        {
                            ResolveCollisionWith(&lyric, box);
                        }
                    }

                    for (const auto& tuplet : measure->tuplets)
                    {
                        ResolveCollisionWith(tuplet, box);
                    }

                    for (Direction& direction : measure->directions)
                    {

                        for (auto& words : direction.words)
                        {
                            BoundingBox oldBoundingBox = words.boundingBox;
                            BoundingBox::ResolveOverlap(box, words.boundingBox);
                            words.Move(words.boundingBox.position - oldBoundingBox.position);
                        }

                        for (auto& rehearsal : direction.rehearsals)
                        {
                            /*LOGV("rehearsal text: %s, box: %s | rehearsal.box: %s", rehearsal.text.string.c_str(), box.GetPrintableString().c_str(), rehearsal.boundingBox.GetPrintableString().c_str());
                            if (BoundingBox::DoBoundingBoxesOverlap(box, rehearsal.boundingBox))
                            {
                                LOGW("rehearsal text: %s", rehearsal.text.string.c_str());
                            }*/
                            BoundingBox oldBoundingBox = rehearsal.boundingBox;
                            BoundingBox::ResolveOverlap(box, rehearsal.boundingBox);
                            rehearsal.Move(rehearsal.boundingBox.position - oldBoundingBox.position);
                        }

                        if (direction.metronomeMark != nullptr)
                        {
                            ResolveCollisionWith(direction.metronomeMark, box);
                            //LOGW("metronomeMark!: %s", direction.metronomeMark->tempo.c_str());
                            //BoundingBox::ResolveOverlap(box, direction.metronomeMark->boundingBox);
                        }

                        for (auto& dynamic : direction.dynamics)
                        {
                            ResolveCollisionWith(&dynamic, box);
                        }

                        if (direction.dynamicWedge != nullptr)
                        {
                            ResolveCollisionWith(direction.dynamicWedge, box);
                        }

                        if (direction.bracketDirection != nullptr)
                        {
                            ResolveCollisionWith(direction.bracketDirection, box);
                        }

                        if (direction.marker)
                        {
                            ResolveCollisionWith(direction.marker, box);
                        }
                    }

                    for (Chord& chord : measure->chords)
                    {
                        ResolveCollisionWith(&chord, box);
                    }
                }

                measureIndex++;
            }
        }
    }
}

void Song::ResolveCollisionWith(const std::shared_ptr<VisibleElement>& element, BoundingBox& box)
{
    BoundingBox oldBoundingBox = element->boundingBox;
    BoundingBox::ResolveOverlap(box, element->boundingBox);
    element->Move(element->boundingBox.position - oldBoundingBox.position);
}

void Song::ResolveCollisionWith(VisibleElement* element, BoundingBox& box)
{
    BoundingBox oldBoundingBox = element->boundingBox;
    BoundingBox::ResolveOverlap(box, element->boundingBox);
    element->Move(element->boundingBox.position - oldBoundingBox.position);
}

std::vector<Vec2<float>> Song::GetSystemPositions() const
{
    std::vector<Vec2<float>> systemPositions;

    int systemIndex = 0;

    Vec2<float> systemPosition;

    float previousSystemPositionY = 0.0f;

    for (const auto& system : systems)
    {
        int measureIndex = GetFirstMeasureInSystem(systemIndex);
        int lastInstrumentIndex = (int)instruments.size() - 1;

        if (measureIndex >= GetMeasureCount())
        {
            LOGI("measureIndex: %d, count: %d", measureIndex, GetMeasureCount());
            break;
        }

        if (DoesMeasureStartNewPage(measureIndex)) // if it is the first system on the page
        {
            systemPosition.y = displayConstants.topMargin + system->layout.topSystemDistance;
        }
        else
        {
            float lastInstrumentPositionY = GetInstrumentPositionY(measureIndex, lastInstrumentIndex);

            systemPosition.y = previousSystemPositionY + lastInstrumentPositionY + system->layout.systemDistance + instruments[lastInstrumentIndex]->GetMiddleHeight(10.0f, 13.33f, measureIndex, measureIndex + 1);
        }

        systemPosition.x = displayConstants.leftMargin + system->layout.systemLeftMargin;

        systemPositions.push_back(systemPosition);

        previousSystemPositionY = systemPosition.y;
        systemIndex++;
    }

    return systemPositions;
}

std::shared_ptr<Measure> Song::GetMeasureAtPoint(Vec2<float> point, const std::vector<Vec2<float>>& systemPositions) const
{
    int systemIndex = 0;
    for (const auto& system : systems)
    {
        int instrumentIndex = 0;
        for (const auto& instrument : instruments)
        {
            int staffIndex = 0;
            for (const auto& staff : instrument->staves)
            {
                int start = system->beginningMeasureIndex;
                int end = system->endingMeasureIndex;

                BoundingBox bb = staff->GetTotalBoundingBox(displayConstants, systemIndex);
                bb.position += systemPositions[systemIndex];
                bb.position += instrument->systemPositionData[systemIndex];
                bb.position += staff->systemPositionData[systemIndex];

                bb.position.x = systemPositions[systemIndex].x;
                bb.size.x = 1050.0f;

                if (bb.DoesOverlapWithPoint(point))
                {
                    for (int m = start; m <= end; m++)
                    {
                        BoundingBox boundingBox = bb;

                        boundingBox.position.x += GetMeasurePositionX(m);
                        boundingBox.size.x = staff->measures[m]->boundingBox.size.x;

                        if (boundingBox.DoesOverlapWithPoint(point))
                            return staff->measures[m];
                    }
                }

                staffIndex++;
            }

            instrumentIndex++;
        }

        systemIndex++;
    }

    return nullptr;
}

void Song::Transpose(const TranspositionRequest& transposeRequest)
{
   for (const auto& instrument : instruments)
   {
       TranspositionRequest newRequest = transposeRequest;
       if (transposeRequest.transposeTablatureType == TranspositionRequest::TransposeTablatureType::Auto)
       {
           if (instrument->UsesOpenStrings())
           {
               newRequest.transposeTablatureType = TranspositionRequest::TransposeTablatureType::UseCapo;
           }
           else
           {
               newRequest.transposeTablatureType = TranspositionRequest::TransposeTablatureType::NoCapo;
           }
       }

       instrument->Transpose(newRequest);
   }

   OnUpdate();
}

int Song::GetMeasureCount() const
{
    return systemMeasures.size();
}
