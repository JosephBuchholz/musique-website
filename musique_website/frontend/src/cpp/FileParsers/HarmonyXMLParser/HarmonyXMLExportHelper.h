#pragma once

#include <string>
#include "../../libs/tinyxml2/tinyxml2.h"
#include "../../MusicData/Types.h"

#include "../../Collisions/Vec2.h"

#include <memory>
#include "../../MusicData/BaseElements/TextualElement.h"

using namespace tinyxml2;

/**
 * A 'singleton' that houses helper functions for parsing musicxml using tinxml2.
 */
class HarmonyXMLExportHelper
{
public:

    // --- Conversions ---

    static const char* FromBoolToYesNo(bool value);

    // --- Exporters ---

    static void SetTextualAttributes(XMLElement* element, const TextualElement& object);
};