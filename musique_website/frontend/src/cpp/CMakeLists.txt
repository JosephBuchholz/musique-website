cmake_minimum_required(VERSION 3.19.2)

#set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_C_COMPILER gcc)
add_compile_options(-std=c++17 -fsanitize=undefined)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(musique)

add_executable(
        cpp

        # Provides a relative path to your source file(s).
        Main.cpp
        Rendering/Renderer.cpp
        MusicPlayers/MidiCallbacks.cpp
        App.cpp
        Callbacks.cpp
        Debugging/Logging.cpp
        Rendering/MusicRenderer.cpp
        MusicPlayers/MusicPlayer.cpp
        RenderMeasurement.cpp
        MusicPlayers/MidiPlayer.cpp
        MusicPlayers/Metronome.cpp
        TranspositionRequest.cpp

        Rendering/RenderData.cpp
        Rendering/Line.cpp

        MusicData/Song.cpp
        MusicData/SongData.cpp
        MusicData/Staff.cpp
        MusicData/Instrument.cpp
        MusicData/Pitch.cpp
        MusicData/ComplexLine.cpp
        MusicData/InstrumentBracket.cpp
        MusicData/SoundEvent.cpp

        MusicData/ChordSheetData/CSStaff.cpp
        MusicData/ChordSheetData/CSMeasure.cpp
        MusicData/ChordSheetData/CSChord.cpp
        MusicData/ChordSheetData/CSLyric.cpp


        MusicData/BaseElements/LineElement.cpp
        MusicData/BaseElements/TextualElement.cpp
        MusicData/BaseElements/VisibleElement.cpp
        MusicData/BaseElements/NumberElement.cpp
        MusicData/BaseElements/TextElement.cpp

        MusicData/Directions/Words.cpp
        MusicData/Directions/BracketDirection.cpp
        MusicData/Directions/Direction.cpp
        MusicData/Directions/DurationDirection.cpp
        MusicData/Directions/Dynamic.cpp
        MusicData/Directions/DynamicWedge.cpp
        MusicData/Directions/MetronomeMark.cpp
        MusicData/Directions/Rehearsal.cpp
        MusicData/Directions/TextDirection.cpp
        MusicData/Directions/Marker.cpp

        MusicData/Chords/Chord.cpp
        MusicData/Chords/ChordDiagram.cpp

        MusicData/Page/Page.cpp
        MusicData/Page/Credit.cpp
        MusicData/Page/PageNumber.cpp

        MusicData/Measures/Measure.cpp
        MusicData/Measures/KeySignature.cpp
        MusicData/Measures/Clef.cpp
        MusicData/Measures/Tuplet.cpp
        MusicData/Measures/MeasureNumber.cpp
        MusicData/Measures/TimeSignature.cpp
        MusicData/Measures/Arpeggio.cpp
        MusicData/Endings/Ending.cpp
        MusicData/Measures/MeasureRepeat.cpp
        MusicData/Measures/Barline.cpp
        MusicData/Measures/MultiMeasureRestSymbol.cpp

        MusicData/Notes/Note.cpp
        MusicData/Notes/Accidental.cpp
        MusicData/Notes/Lyric.cpp
        MusicData/Notes/AugmentationDot.cpp
        MusicData/Notes/NoteHead.cpp
        MusicData/Notes/Fermata.cpp
        MusicData/Notes/NoteChord.cpp
        MusicData/Notes/NoteFlag.cpp
        MusicData/Notes/NoteStem.cpp
        MusicData/Notes/TremoloSingle.cpp
        MusicData/Notes/Slur.cpp
        MusicData/Notes/TABSlur.cpp
        MusicData/Notes/Tie.cpp
        MusicData/Notes/GlissandoSlide.cpp

        MusicData/Notes/Beams/Beam.cpp
        MusicData/Notes/Beams/BeamGroup.cpp

        MusicData/Notes/Articulations/Accent.cpp
        MusicData/Notes/Articulations/Articulation.cpp
        MusicData/Notes/Articulations/BreathMark.cpp
        MusicData/Notes/Articulations/Caesura.cpp
        MusicData/Notes/Articulations/Slide.cpp
        MusicData/Notes/Articulations/StaccatoArticulation.cpp
        MusicData/Notes/Articulations/Stress.cpp

        MusicData/Notes/Techniques/Bowing.cpp
        MusicData/Notes/Techniques/GuitarTechnique.cpp
        MusicData/Notes/Techniques/Bend.cpp
        MusicData/Notes/Techniques/Technique.cpp

        MusicData/Notes/Ornaments/Ornaments.cpp
        MusicData/Notes/Ornaments/Mordent.cpp
        MusicData/Notes/Ornaments/TrillMark.cpp
        MusicData/Notes/Ornaments/Turn.cpp
        MusicData/Notes/Ornaments/Vibrato.cpp

        FileParsers/ChordSheetParser/ChordSheetParser.cpp

        FileParsers/MusicXMLParser/MusicXMLHelper.cpp
        FileParsers/MusicXMLParser/MusicXMLParser.cpp
        FileParsers/MusicXMLParser/XMLHelper.cpp
        FileParsers/MusicXMLParser/ParseError.cpp
        FileParsers/MusicXMLParser/NoteElementParser.cpp
        FileParsers/MusicXMLParser/BaseElementParser.cpp

        FileParsers/HarmonyXMLParser/HarmonyXMLParser.cpp
        FileParsers/HarmonyXMLParser/HarmonyXMLExporter.cpp
        FileParsers/HarmonyXMLParser/HarmonyXMLExportHelper.cpp

        Editor/Editor.cpp
        Editor/Properties.cpp
        Editor/EditorCommand.cpp
        Editor/ElementProperties.cpp

        FileParsers/Json/JsonHelper.cpp

        Utils/Math.cpp
        Utils/Converters.cpp

        Collisions/BoundingBox.cpp

        libs/tinyxml2/tinyxml2.cpp)

set_target_properties(cpp PROPERTIES LINK_FLAGS "-lembind -fsanitize=undefined -sMODULARIZE=1 -sALLOW_TABLE_GROWTH -sEXPORTED_RUNTIME_METHODS=addFunction,UTF8ToString,UTF16ToString,getValue,setValue,stringToNewUTF8 -sEXPORTED_FUNCTIONS=_malloc,_main")