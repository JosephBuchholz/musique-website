#ifndef MUSIQUE_SYSTEMMEASURE_H
#define MUSIQUE_SYSTEMMEASURE_H

//#include "Measures/MeasureNumber.h"

class SystemMeasure
{
public:

    uint32_t measureIndex = 0; // TODO: remove this memeber
    bool pageBreak = false;
    bool systemBreak = false;

    //MeasureNumber measureNumber; // the number that is displayed for this measure
};

#endif //MUSIQUE_SYSTEMMEASURE_H
