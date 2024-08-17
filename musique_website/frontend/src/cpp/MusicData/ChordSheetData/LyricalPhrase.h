#pragma once

#include <vector>

#include "CSLyric.h"

class CSLyric;

class LyricalPhrase
{
public:

    std::vector<std::shared_ptr<CSLyric>> lyrics;
};
