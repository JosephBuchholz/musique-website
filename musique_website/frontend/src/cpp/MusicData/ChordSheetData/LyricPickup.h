#pragma once

#include <vector>

#include "CSLyric.h"

class CSLyric;

class LyricPickup
{
public:

    std::vector<std::shared_ptr<CSLyric>> lyrics;
};

