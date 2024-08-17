#ifndef MUSIQUE_SYLLABLEGROUP_H
#define MUSIQUE_SYLLABLEGROUP_H

#include <vector>

#include "CSLyric.h"

class CSLyric;

class SyllableGroup
{
public:

    std::vector<std::shared_ptr<CSLyric>> lyrics;
};

#endif //MUSIQUE_SYLLABLEGROUP_H
