#ifndef MUSIQUE_CSCHORD_H
#define MUSIQUE_CSCHORD_H

#include <string>
#include <functional>

#include "../../Rendering/RenderData.h"
#include "../BaseElements/VisibleElement.h"
#include "../Chords/Chord.h"
#include "../Notes/NoteHead.h"
#include "../Notes/NoteStem.h"
#include "../Notes/NoteFlag.h"
#include "../Notes/AugmentationDot.h"

class CSChord : public VisibleElement
{
public:

    CSChord()
        : BaseElement(BaseElement::ElementType::CSChord) {}

    void Init(const Settings& settings, Vec2<float> pos);
    void Delete() override;

    void Render(RenderData& renderData, const Settings& settings, Vec2<float> measurePosition) const;

    BoundingBox GetBoundingBox() const;
    Vec2<float> GetDimensions() const;

    void SetOnDeleteCallback(std::function<void()> callback) { onDeleteCallback = callback; }

public:

    Chord chordSymbol;

    float duration = 1.0f;

    float beatPosition = 0.0f;
    float beatPositionInSong = 0.0f;

    std::unique_ptr<NoteHead> noteHead;
    std::unique_ptr<NoteStem> noteStem;
    std::unique_ptr<NoteFlag> noteFlag;
    std::unique_ptr<AugmentationDot> augDot;

    BaseElement* parent = nullptr;

    std::function<void()> onDeleteCallback = []() {};
};

#endif //MUSIQUE_CSCHORD_H
