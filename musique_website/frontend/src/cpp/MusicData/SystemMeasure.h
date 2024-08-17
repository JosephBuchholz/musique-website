#ifndef MUSIQUE_SYSTEMMEASURE_H
#define MUSIQUE_SYSTEMMEASURE_H


//#include "Measures/MeasureNumber.h"
//#include "Measures/TimeSignature.h"

class TimeSignature;

class SystemMeasure
{
public:

    uint32_t measureIndex = 0; // TODO: remove this memeber
    bool pageBreak = false;
    bool systemBreak = false;
    bool isPickupMeasure = false; // must be the first measure of a system/section

    std::unique_ptr<TimeSignature> timeSignature;

    //MeasureNumber measureNumber; // the number that is displayed for this measure
};

#endif //MUSIQUE_SYSTEMMEASURE_H
