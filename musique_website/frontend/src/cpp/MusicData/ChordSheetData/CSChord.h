#ifndef MUSIQUE_CSCHORD_H
#define MUSIQUE_CSCHORD_H

#include <string>
#include <functional>

#include "../../Rendering/RenderData.h"
#include "../BaseElements/VisibleElement.h"
#include "../Chords/Chord.h"

class CSChord : public VisibleElement
{
public:

    CSChord()
        : BaseElement(BaseElement::ElementType::CSChord) {}

    void Init(Vec2<float> pos);
    void Delete() override;

    void Render(RenderData& renderData, const Settings& settings, Vec2<float> measurePosition) const;

    Vec2<float> GetDimensions() const;

    void SetOnDeleteCallback(std::function<void()> callback) { onDeleteCallback = callback; }

public:

    Chord chordSymbol;

    float duration = 1.0f;

    float beatPosition = 0.0f;
    float beatPositionInSong = 0.0f;

    std::function<void()> onDeleteCallback = []() {};
};

#endif //MUSIQUE_CSCHORD_H
