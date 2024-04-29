#pragma once

#include <string>
#include <memory>

#include "../../MusicData/Song.h"

class HarmonyXMLExporter
{
public:

    /**
     * Exports a HarmonyXML formatted file (.harmonyxml)
     *
     * @param song[in] The song object to be exported.
     * @return The data exported.
     */
    static std::string ExportHarmonyXML(const std::shared_ptr<Song>& song);
};