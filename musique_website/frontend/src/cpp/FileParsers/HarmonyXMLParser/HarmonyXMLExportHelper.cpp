#include "HarmonyXMLExportHelper.h"

#include "HarmonyXMLDefaults.h"

const char* HarmonyXMLExportHelper::FromBoolToYesNo(bool value)
{
    if (value) {
        return "yes";
    }
    return "no";
}


void HarmonyXMLExportHelper::SetTextualAttributes(XMLElement* element, const TextualElement& object)
{
    if (object.fontWeight != HARMXML_DEFAULT_FONT_WEIGHT)
    {
        std::string fontWeightString = "";
        switch(object.fontWeight)
        {
            case FontWeight::Normal: fontWeightString = "normal"; break;
            case FontWeight::Bold: fontWeightString = "bold"; break;
            case FontWeight::None:
            default: fontWeightString = "none";
        }

        element->SetAttribute("font-weight", fontWeightString.c_str());
    }

    if (object.fontStyle != HARMXML_DEFAULT_FONT_STYLE)
    {
        std::string fontStyleString = "";
        switch(object.fontStyle)
        {
            case FontStyle::Normal: fontStyleString = "normal"; break;
            case FontStyle::Italic: fontStyleString = "italic"; break;
            case FontStyle::None:
            default: fontStyleString = "none";
        }

        element->SetAttribute("font-style", fontStyleString.c_str());
    }

    if (object.justify != HARMXML_DEFAULT_JUSTIFY)
    {
        std::string justifyString = "";
        switch(object.justify)
        {
            case Justify::Left: justifyString = "left"; break;
            case Justify::Center: justifyString = "center"; break;
            case Justify::Right: justifyString = "right"; break;
            case Justify::None:
            default: justifyString = "none";
        }

        element->SetAttribute("justify", justifyString.c_str());
    }

    if (object.fontSize.size != HARMXML_DEFUALT_FONT_SIZE)
    {
        element->SetAttribute("font-size", object.fontSize.size);
    }
}