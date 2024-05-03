#pragma once

#include <vector>
#include <string>

#include "../Collisions/Vec2.h"

class BaseProperty
{
public:
    std::string name;
};

class PositionProperties
{
public:
    Vec2<float> position;
};

class TextProperties
{
public:
    std::string text;
    bool isBold = false;
    bool isItalic = false;
    float size = 10.0f;
    uint32_t color = 0x000000FF;
};

class Properties
{
public:
    std::vector<TextProperties> textProperties;
    std::vector<PositionProperties> positionProperties;
};