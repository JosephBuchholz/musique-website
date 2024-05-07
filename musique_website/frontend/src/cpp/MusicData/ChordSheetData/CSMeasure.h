#ifndef MUSIQUE_CSMEASURE_H
#define MUSIQUE_CSMEASURE_H

#include <vector>

#include "CSChord.h"
#include "CSLyric.h"

class CSMeasure : public VisibleElement
{
public:

    CSMeasure()
        : BaseElement(BaseElement::ElementType::CSMeasure) {}

    void Init();
    void Delete() override;

    void Render(RenderData& renderData, const Settings& settings, Vec2<float> parentPosition) const;

    BoundingBox GetTotalBoundingBox(const MusicDisplayConstants& displayConstants) const;

    int GetMeasureIndex() const;

private:

    float GetPositionXFromBeatPositionOfChords(float beatPosition) const;
    std::shared_ptr<CSChord> GetChordFromBeatPosition(float beatPosition);

public:
    float width = 0.0f;

    bool isFirstMeasureOfSystem = false;

    int divisions = 0;

    std::vector<std::shared_ptr<CSChord>> chords;
    std::vector<std::shared_ptr<CSLyric>> lyrics;

    std::shared_ptr<BaseElement> parent;
};

#endif //MUSIQUE_CSMEASURE_H
