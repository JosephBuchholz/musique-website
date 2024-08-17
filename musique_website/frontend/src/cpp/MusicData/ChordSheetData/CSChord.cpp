#include "CSChord.h"

void CSChord::Render(RenderData& renderData, const Settings& settings, Vec2<float> measurePosition) const
{
    //LOGI_TAG("CSChord.cpp", "Rendering chord!!!!: mpos: %s | pos: %s", measurePosition.GetPrintableString().c_str(), position.GetPrintableString().c_str());
    chordSymbol.Render(renderData, settings, measurePosition + position);

    Vec2<float> noteHeadPosition = measurePosition + position;
    noteHeadPosition.y -= 30.0f;
    noteHead->Render(renderData, noteHeadPosition);

    if (noteStem)
    {
        noteStem->Render(renderData, noteHeadPosition, nullptr, false, false, noteHead->GetDimensions(settings.displayConstants).x);

        if (noteFlag)
            noteFlag->Render(renderData, { noteHeadPosition.x + noteStem->stemPositionX, noteHeadPosition.y + noteStem->stemEndY });
    }

    if (augDot)
    {
        augDot->Render(renderData, noteHeadPosition);
    }
}

void CSChord::Init(const Settings& settings, Vec2<float> pos)
{
    position = pos;

    chordSymbol.justify = Justify::Left;

    noteHead->sizeFactor = 0.8f;

    if (noteStem)
    {
        noteStem->stemStartY = 0.0f;
        noteStem->stemEndY = -15.0f;
        noteStem->stemPositionX = noteHead->GetNoteHeadWidth(settings.displayConstants) - (1.25f / 2.0f);
    }

    if (noteHead->noteDuration == NoteValue::Eighth || noteHead->noteDuration == NoteValue::Sixteenth)
    {
        noteFlag = std::make_unique<NoteFlag>();
        
        noteFlag->noteDuration = noteHead->noteDuration;
        noteFlag->type = NoteFlag::Type::Up;
        noteFlag->size = 0.6f;
    }

    if (augDot)
    {
        float x = noteHead->GetNoteHeadWidth(settings.displayConstants);

        if (noteFlag)
            x += 6.0f;

        augDot->CalculatePositionAsPaged(settings.displayConstants, false, false, x);
    }

    /*augDot = nullptr;
    noteStem = nullptr;
    noteFlag = nullptr;
    noteHead = nullptr;

    if (!noteHead)
        noteHead = std::make_unique<NoteHead>();

    noteHead->sizeFactor = 0.8f;
    
    if (duration == 4.0f || duration == 6.0f)
        noteHead->noteDuration = NoteValue::Whole;
    else if (duration == 3.0f)
    {
        noteHead->noteDuration = NoteValue::Half;

        augDot = std::make_unique<AugmentationDot>();
        augDot->CalculatePositionAsPaged(settings.displayConstants, false, false, noteHead->GetNoteHeadWidth(settings.displayConstants));
    }
    else if (duration == 2.0f)
    {
        noteHead->noteDuration = NoteValue::Half;
    }
    else if (duration == 1.5f)
    {
        noteHead->noteDuration = NoteValue::Quarter;

        augDot = std::make_unique<AugmentationDot>();
        augDot->CalculatePositionAsPaged(settings.displayConstants, false, false, noteHead->GetNoteHeadWidth(settings.displayConstants));
    }
    else if (duration == 1.0f)
    {
        noteHead->noteDuration = NoteValue::Quarter;
    }
    else if (duration == 0.75f)
    {
        noteHead->noteDuration = NoteValue::Eighth;

        augDot = std::make_unique<AugmentationDot>();
        augDot->CalculatePositionAsPaged(settings.displayConstants, false, false, noteHead->GetNoteHeadWidth(settings.displayConstants) + 5.0f);
    }
    else if (duration == 0.5f)
    {
        noteHead->noteDuration = NoteValue::Eighth;
    }
    else if (duration == 0.25f)
        noteHead->noteDuration = NoteValue::Sixteenth;

    if (noteHead->noteDuration != NoteValue::Whole)
    {
        if (!noteStem)
            noteStem = std::make_unique<NoteStem>();
        
        noteStem->stemType = NoteStem::StemType::Up;
        noteStem->stemStartY = 0.0f;
        noteStem->stemEndY = -15.0f;
        noteStem->stemPositionX = noteHead->GetNoteHeadWidth(settings.displayConstants) - (1.25f / 2.0f);
    }

    if (noteHead->noteDuration == NoteValue::Eighth || noteHead->noteDuration == NoteValue::Sixteenth)
    {
        if (!noteFlag)
            noteFlag = std::make_unique<NoteFlag>();
        
        noteFlag->noteDuration = noteHead->noteDuration;
        noteFlag->type = NoteFlag::Type::Up;
        noteFlag->size = 0.6f;
    }*/
}

BoundingBox CSChord::GetBoundingBox() const
{
    BoundingBox bb = chordSymbol.GetBoundingBoxRelativeToParent();
    bb.position += position;
    return bb;
}

Vec2<float> CSChord::GetDimensions() const
{
    return GetBoundingBox().size;
}

void CSChord::Delete()
{
    onDeleteCallback();
}
