#ifndef MUSIQUE_CSLYRIC_H
#define MUSIQUE_CSLYRIC_H

#include <functional>

#include "../BaseElements/TextualElement.h"
#include "../../Settings.h"
#include "../BaseElements/TextElement.h"
#include "SyllableGroup.h"
#include "LyricalPhrase.h"
#include "LyricPickup.h"

class CSLyric : public TextualElement
{
public:

    CSLyric()
        : BaseElement(BaseElement::ElementType::CSLyric) {}

    void Init(const Settings& settings, Vec2<float> pos);
    void Delete() override;

    void Render(RenderData& renderData, const Settings& settings, Vec2<float> parentPosition, bool isPickup = false) const;

    void UpdateBoundingBox();

    void SetOnDeleteCallback(std::function<void()> callback) { onDeleteCallback = callback; }

public:

    float beatPosition = 0.0f;
    float duration = 1.0f;

    TextElement lyricText;

    std::shared_ptr<SyllableGroup> parentSyllableGroup;
    std::shared_ptr<LyricalPhrase> parentLyricalPhrase;

    bool isPickupToNextMeasure = false;
    bool startsPickup = false;
    Vec2<float> pickupPosition = { 0.0f, 0.0f };
    std::shared_ptr<LyricPickup> parentLyricPickup;

    std::function<void()> onDeleteCallback = []() {};
};

#endif //MUSIQUE_CSLYRIC_H