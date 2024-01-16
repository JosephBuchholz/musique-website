#include <iostream>
#include <chrono>

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include "Debugging/Debug.h"
#include "Rendering/SMuFLGlyph.h"
#include "Collisions/Vec2.h"
#include "Rendering/Renderer.h"
#include "MusicPlayers/MidiCallbacks.h"
#include "MusicData/Types.h"

#include "App.h"

using namespace std::chrono;



milliseconds previousTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
void MainLoop()
{
    milliseconds timeNow = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    double deltaTime = (timeNow - previousTime).count();

    App& app = App::GetInstance();

    app.OnUpdate(deltaTime);

    //LOGV("Main Loop!!!!: %lf", deltaTime);

    previousTime = timeNow;
}

EM_BOOL MouseWheelCallback(int eventType, const EmscriptenWheelEvent* wheelEvent, void* userData)
{
    App& app = App::GetInstance();

    MouseScrollEvent event;
    event.deltaX = wheelEvent->deltaX;
    event.deltaY = wheelEvent->deltaY;
    event.deltaZ = wheelEvent->deltaZ;

    return app.OnMouseScrollEvent(event);
}

int main()
{
    LOGI("Initilizing C++");

    const char* target = "#canvas";
    EMSCRIPTEN_RESULT result = emscripten_set_wheel_callback(target, nullptr, EM_TRUE, MouseWheelCallback);

    LOGD("got emscipten result: %d", result);

    App& app = App::GetInstance();

    app.LoadSongFromString("musicxml", R"V0G0N0(<score-partwise version="4.0">
<work>
<work-title>All My Hope ft. Tauren Wells</work-title>
</work>
<identification>
<creator type="composer">Crowder</creator>
<encoding>
<software>MuseScore 4.0.2</software>
<encoding-date>2023-05-29</encoding-date>
<supports element="accidental" type="yes"/>
<supports element="beam" type="yes"/>
<supports element="print" attribute="new-page" type="yes" value="yes"/>
<supports element="print" attribute="new-system" type="yes" value="yes"/>
<supports element="stem" type="yes"/>
</encoding>
</identification>
<defaults>
<scaling>
<millimeters>6.99912</millimeters>
<tenths>40</tenths>
</scaling>
<page-layout>
<page-height>1596.77</page-height>
<page-width>1233.87</page-width>
<page-margins type="even">
<left-margin>85.725</left-margin>
<right-margin>85.725</right-margin>
<top-margin>85.725</top-margin>
<bottom-margin>85.725</bottom-margin>
</page-margins>
<page-margins type="odd">
<left-margin>85.725</left-margin>
<right-margin>85.725</right-margin>
<top-margin>85.725</top-margin>
<bottom-margin>85.725</bottom-margin>
</page-margins>
</page-layout>
<word-font font-family="Edwin" font-size="10"/>
<lyric-font font-family="Edwin" font-size="10"/>
</defaults>
<credit page="1">
<credit-type>title</credit-type>
<credit-words default-x="616.935" default-y="1511.05" justify="center" valign="top" font-size="22">All My Hope ft. Tauren Wells</credit-words>
</credit>
<credit page="1">
<credit-type>composer</credit-type>
<credit-words default-x="1148.14" default-y="1411.05" justify="right" valign="bottom">Crowder</credit-words>
</credit>
<credit page="1">
<credit-words default-x="85.725" default-y="1511.05" justify="left" valign="top" font-size="14">Piano</credit-words>
</credit>
<part-list>
<score-part id="P1">
<part-name>Piano</part-name>
<part-abbreviation>Pno.</part-abbreviation>
<score-instrument id="P1-I1">
<instrument-name>Piano</instrument-name>
</score-instrument>
<midi-device id="P1-I1" port="1"/>
<midi-instrument id="P1-I1">
<midi-channel>1</midi-channel>
<midi-program>1</midi-program>
<volume>78.7402</volume>
<pan>0</pan>
</midi-instrument>
</score-part>
</part-list>
<part id="P1">
<measure number="1" width="230.23">
<print>
<system-layout>
<system-margins>
<left-margin>50.00</left-margin>
<right-margin>0.00</right-margin>
</system-margins>
<top-system-distance>241.49</top-system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>48.06</staff-distance>
</staff-layout>
</print>
<attributes>
<divisions>12</divisions>
<key>
<fifths>-4</fifths>
</key>
<time>
<beats>3</beats>
<beat-type>4</beat-type>
</time>
<staves>2</staves>
<clef number="1">
<sign>F</sign>
<line>4</line>
</clef>
<clef number="2">
<sign>F</sign>
<line>4</line>
</clef>
</attributes>
<direction placement="above">
<direction-type>
<metronome parentheses="no" default-x="-37.68" default-y="58.15" relative-y="20.00">
<beat-unit>quarter</beat-unit>
<per-minute>104</per-minute>
</metronome>
</direction-type>
<staff>1</staff>
<sound tempo="104"/>
</direction>
<direction>
<direction-type>
<words default-y="32.98" relative-y="20.00" font-weight="bold" font-size="12">Swing</words>
</direction-type>
<staff>1</staff>
</direction>
<direction placement="above">
<direction-type>
<rehearsal default-x="-20.56" default-y="83.93" relative-y="30.00" font-weight="bold" font-size="14">Intro</rehearsal>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<direction placement="below">
<direction-type>
<dynamics default-x="5.32" default-y="-49.44" relative-y="-25.00">
<mf/>
</dynamics>
</direction-type>
<staff>1</staff>
<sound dynamics="88.89"/>
</direction>
<note default-x="134.45" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="134.45" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="134.45" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="134.45" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="177.74" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="200.24" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="2" width="153.54">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="42.06" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="42.06" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="42.06" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="104.50" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="104.50" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="123.74" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="42.06" default-y="-153.06">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="104.50" default-y="-148.06">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="3" width="124.04">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="11.05" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
<slur type="start" placement="below" number="2"/>
<slur type="start" placement="below" number="3"/>
</notations>
</note>
<note default-x="31.72" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<slur type="stop" number="1"/>
<slur type="stop" number="2"/>
<slur type="stop" number="3"/>
</notations>
</note>
<note default-x="75.01" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="94.24" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="31.72" default-y="-143.06">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
</measure>
<measure number="4" width="136.57">
<note default-x="11.05" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
<slur type="start" placement="below" number="2"/>
</notations>
</note>
<note default-x="31.72" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<slur type="stop" number="1"/>
<slur type="stop" number="2"/>
</notations>
</note>
<note default-x="50.96" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="87.54" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="106.78" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="31.72" default-y="-143.06">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
</measure>
<measure number="5" width="108.98">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="56.49" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="78.98" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="6" width="153.54">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="42.06" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="42.06" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="42.06" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="104.50" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="104.50" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="123.74" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="42.06" default-y="-153.06">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="104.50" default-y="-148.06">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="7" width="105.52">
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="56.49" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="56.49" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="56.49" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="75.72" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-138.06">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="8" width="232.57">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>-0.00</right-margin>
</system-margins>
<system-distance>132.92</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>46.17</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="112.33" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="112.33" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="144.78" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="162.27" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
</notations>
</note>
<note default-x="182.94" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<slur type="stop" number="1"/>
</notations>
</note>
<note default-x="204.57" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="112.33" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="163.86" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="9" width="133.17">
<barline location="left">
<bar-style>heavy-light</bar-style>
<repeat direction="forward"/>
</barline>
<direction placement="above">
<direction-type>
<rehearsal default-x="-32.01" default-y="25.95" relative-y="30.00" font-weight="bold" font-size="14">Verse 1&2</rehearsal>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="32.01" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="32.01" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="32.01" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="32.01" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="80.68" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="103.17" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="10" width="159.51">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="45.64" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="45.64" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="45.64" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="108.09" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="108.09" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="129.71" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="45.64" default-y="-151.17">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="108.09" default-y="-146.17">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="11" width="137.30">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="11.05" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
<slur type="start" placement="below" number="2"/>
<slur type="start" placement="below" number="3"/>
</notations>
</note>
<note default-x="31.72" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<slur type="stop" number="1"/>
<slur type="stop" number="2"/>
<slur type="stop" number="3"/>
</notations>
</note>
<note default-x="80.38" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="107.50" default-y="-5.00">
<pitch>
<step>G</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<accidental>flat</accidental>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<articulations>
<accent/>
</articulations>
</notations>
</note>
<note default-x="107.50" default-y="5.00">
<chord/>
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="31.72" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
</measure>
<measure number="12" width="143.19">
<note default-x="13.20" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="54.33" default-y="-20.00">
<rest/>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<staff>1</staff>
</note>
<note default-x="75.73" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<accidental>natural</accidental>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
</notations>
</note>
<note default-x="96.39" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>3</duration>
<voice>1</voice>
<type>16th</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<beam number="2">forward hook</beam>
<notations>
<slur type="stop" number="1"/>
</notations>
</note>
<note default-x="111.39" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>9</duration>
<voice>1</voice>
<type>eighth</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<tie type="stop"/>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="54.33" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="13" width="114.36">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="61.86" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="84.36" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="14" width="142.33">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="45.64" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="45.64" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="45.64" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="108.09" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="108.09" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="45.64" default-y="-151.17">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="108.09" default-y="-146.17">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="15" width="233.27">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>0.00</right-margin>
</system-margins>
<system-distance>115.42</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>46.17</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="113.89" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="113.89" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="113.89" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="165.10" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="165.10" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="165.10" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="182.60" default-y="-25.00">
<grace slash="yes"/>
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
</notations>
</note>
<note default-x="203.27" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<slur type="stop" number="1"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="113.89" default-y="-136.17">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="165.10" default-y="-136.17">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="203.27" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="16" width="151.17">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="47.34" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="81.48" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="81.48" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="81.48" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="121.37" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="109.48" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="121.37" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="121.37" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="64.41" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="17" width="117.17">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="64.41" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="87.17" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="18" width="162.34">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="47.34" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="47.34" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="47.34" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="109.78" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="109.78" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="132.54" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="47.34" default-y="-151.17">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="109.78" default-y="-146.17">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="19" width="163.18">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="56.48" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
</note>
<note default-x="84.93" default-y="-20.00">
<rest/>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>1</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="113.39" default-y="-50.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="133.39" default-y="-45.00">
<pitch>
<step>F</step>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="56.48" default-y="-106.17">
<rest/>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<staff>2</staff>
</note>
<note default-x="84.93" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>2</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="113.39" default-y="-106.17">
<rest/>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>2</staff>
</note>
<note default-x="133.39" default-y="-106.17">
<rest/>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>2</staff>
<notations>
<tuplet type="stop"/>
</notations>
</note>
</measure>
<measure number="20" width="134.94">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
</root>
<kind text="m7b5">half-diminished</kind>
</harmony>
<note default-x="19.34" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<accidental>natural</accidental>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="53.48" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="99.00" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="99.00" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="99.00" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="19.34" default-y="-141.17">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<tie type="start"/>
<voice>5</voice>
<type>quarter</type>
<accidental>natural</accidental>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="53.48" default-y="-141.17">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="76.24" default-y="-141.17">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
<note default-x="99.00" default-y="-141.17">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="21" width="100.35">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="64.41" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>up</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="22" width="290.31">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>0.00</right-margin>
</system-margins>
<system-distance>115.44</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>48.06</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus2">suspended-second</kind>
</harmony>
<note default-x="125.79" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="113.89" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="125.79" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus4">suspended-fourth</kind>
</harmony>
<note default-x="202.35" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="202.35" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="237.25" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="260.51" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="125.79" default-y="-138.06">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="219.80" default-y="-138.06">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="23" width="132.05">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="65.55" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tuplet type="start" bracket="no"/>
</notations>
</note>
<note default-x="83.90" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">continue</beam>
</note>
<note default-x="102.25" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="65.55" default-y="-108.06">
<rest/>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<staff>2</staff>
</note>
</measure>
<measure number="24" width="199.97">
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus2">suspended-second</kind>
</harmony>
<note default-x="29.96" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="18.06" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="29.96" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus4">suspended-fourth</kind>
</harmony>
<note default-x="106.51" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="106.51" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="141.41" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="164.68" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="29.96" default-y="-138.06">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="123.96" default-y="-138.06">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<barline location="right">
<bar-style>light-light</bar-style>
</barline>
</measure>
<measure number="25" width="118.81">
<direction placement="above">
<direction-type>
<rehearsal default-x="-13.20" default-y="25.95" relative-y="30.00" font-weight="bold" font-size="14">Chorus</rehearsal>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<direction placement="below">
<direction-type>
<dynamics default-x="5.32" default-y="-49.44" relative-y="-25.00">
<mf/>
</dynamics>
</direction-type>
<staff>1</staff>
<sound dynamics="88.89"/>
</direction>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="65.55" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="88.81" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="26" width="163.60">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="48.10" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="48.10" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="48.10" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="110.54" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="110.54" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="133.81" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-158.06">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="48.10" default-y="-153.06">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="110.54" default-y="-148.06">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="27" width="157.68">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="11.05" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
<slur type="start" placement="below" number="2"/>
</notations>
</note>
<note default-x="31.72" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<slur type="stop" number="1"/>
<slur type="stop" number="2"/>
</notations>
</note>
<note default-x="54.98" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="99.22" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
</note>
<note default-x="127.68" default-y="-20.00">
<rest/>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="31.72" default-y="-143.06">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="99.22" default-y="-143.06">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="127.68" default-y="-143.06">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="28" width="241.85">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>0.00</right-margin>
</system-margins>
<system-distance>115.42</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>46.17</staff-distance>
</staff-layout>
</print>
<note default-x="102.33" default-y="-15.00">
<grace slash="yes"/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<slur type="start" placement="below" number="1"/>
<slur type="start" placement="below" number="2"/>
</notations>
</note>
<note default-x="123.00" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<slur type="stop" number="1"/>
<slur type="stop" number="2"/>
</notations>
</note>
<note default-x="145.27" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="178.66" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="212.05" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="123.00" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>24</duration>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="195.35" default-y="-136.17">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="29" width="129.08">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="63.29" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tuplet type="start" bracket="no"/>
</notations>
</note>
<note default-x="81.29" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">continue</beam>
</note>
<note default-x="99.28" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="63.29" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="30" width="115.18">
<harmony print-frame="no">
<root>
<root-step>F</root-step>
</root>
<kind text="m">minor</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="46.59" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="46.59" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="79.98" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="79.98" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-166.17">
<pitch>
<step>F</step>
<octave>1</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="63.29" default-y="-166.17">
<pitch>
<step>F</step>
<octave>1</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="31" width="149.76">
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="18.06" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="18.06" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="18.06" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="84.84" default-y="-20.00">
<rest/>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>1</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="102.41" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="119.97" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="18.06" default-y="-136.17">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="51.45" default-y="-116.17">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="84.84" default-y="-136.17">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="32" width="149.68">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="46.59" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="79.98" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="79.98" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="79.98" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="119.88" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="107.98" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="119.88" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="119.88" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="63.29" default-y="-141.17">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="33" width="115.78">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="63.29" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="85.79" default-y="-20.00">
<rest/>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="63.29" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="85.79" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="34" width="161.09">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="46.59" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="46.59" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="46.59" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="109.04" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="109.04" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="131.30" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-156.17">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="46.59" default-y="-151.17">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="109.04" default-y="-146.17">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="35" width="306.85">
<print new-page="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>-0.00</right-margin>
</system-margins>
<top-system-distance>106.26</top-system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>55.77</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="112.33" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="112.33" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="144.11" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<tie type="stop"/>
<voice>1</voice>
<type>quarter</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="144.11" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<tie type="stop"/>
<voice>1</voice>
<type>quarter</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="191.76" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
</note>
<note default-x="223.54" default-y="-20.00">
<rest/>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>1</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="251.99" default-y="-50.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="277.06" default-y="-45.00">
<pitch>
<step>F</step>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<forward>
<duration>6</duration>
</forward>
<note default-x="144.11" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>2</voice>
<type>quarter</type>
<stem>up</stem>
<staff>1</staff>
</note>
<backup>
<duration>18</duration>
</backup>
<note default-x="112.33" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="191.76" default-y="-115.77">
<rest/>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<staff>2</staff>
</note>
<note default-x="223.54" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>2</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="251.99" default-y="-115.77">
<rest/>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>2</staff>
</note>
<note default-x="277.06" default-y="-115.77">
<rest/>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>2</staff>
<notations>
<tuplet type="stop"/>
</notations>
</note>
</measure>
<measure number="36" width="204.01">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
</root>
<kind text="m7b5">half-diminished</kind>
</harmony>
<note default-x="19.34" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<accidental>natural</accidental>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="67.00" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="130.54" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="130.54" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="130.54" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="start"/>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="162.31" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="162.31" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<forward>
<duration>30</duration>
</forward>
<note default-x="174.21" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>2</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="19.34" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<tie type="start"/>
<voice>5</voice>
<type>quarter</type>
<accidental>natural</accidental>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="67.00" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="98.77" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
<note default-x="130.54" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="37" width="164.61">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="84.69" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tuplet type="start" bracket="no"/>
</notations>
</note>
<note default-x="109.75" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">continue</beam>
</note>
<note default-x="134.81" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="38" width="219.51">
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus2">suspended-second</kind>
</harmony>
<note default-x="29.96" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="18.06" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="29.96" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus4">suspended-fourth</kind>
</harmony>
<note default-x="106.51" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="106.51" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="154.17" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="185.94" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="29.96" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="130.34" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="39" width="167.44">
<barline location="left">
<ending number="1, 3" type="start" default-y="79.18">1,3</ending>
</barline>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="84.69" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="116.46" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>3</duration>
<voice>1</voice>
<type>16th</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">continue</beam>
<beam number="2">begin</beam>
</note>
<note default-x="137.64" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>3</duration>
<voice>1</voice>
<type>16th</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<beam number="2">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="84.69" default-y="-115.77">
<rest/>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<staff>2</staff>
</note>
</measure>
<measure number="40" width="293.55">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>0.00</right-margin>
</system-margins>
<system-distance>151.89</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>55.77</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus2">suspended-second</kind>
</harmony>
<note default-x="116.73" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="104.83" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="116.73" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus4">suspended-fourth</kind>
</harmony>
<note default-x="193.29" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="193.29" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="225.36" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<direction>
<direction-type>
<words relative-x="-179.02" relative-y="60.31">go to Ending 3rd time</words>
</direction-type>
<staff>1</staff>
</direction>
<note default-x="246.74" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="116.73" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="209.32" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<barline location="right">
<bar-style>light-heavy</bar-style>
<ending number="1, 3" type="stop"/>
<repeat direction="backward"/>
</barline>
</measure>
<measure number="41" width="112.48">
<barline location="left">
<ending number="2" type="start" default-y="103.13">2.</ending>
</barline>
<direction>
<direction-type>
<words default-y="32.98" relative-y="20.00" font-weight="bold" font-size="12">Straight</words>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="61.30" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="82.68" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<barline location="right">
<ending number="2" type="discontinue"/>
</barline>
</measure>
<measure number="42" width="155.40">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="34.58" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<note default-x="55.96" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="77.34" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<note default-x="98.72" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="120.10" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<barline location="right">
<bar-style>light-light</bar-style>
</barline>
</measure>
<measure number="43" width="75.98">
<direction>
<direction-type>
<words default-y="32.98" relative-y="20.00" font-weight="bold" font-size="12">Swing</words>
</direction-type>
<staff>1</staff>
</direction>
<direction placement="above">
<direction-type>
<rehearsal default-x="-13.20" default-y="53.93" relative-y="30.00" font-weight="bold" font-size="14">Verse 3</rehearsal>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
</measure>
<measure number="44" width="75.98">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
</measure>
<measure number="45" width="75.98">
<direction placement="below">
<direction-type>
<dynamics default-x="3.10" default-y="-40.89" relative-y="-25.00">
<p/>
</dynamics>
</direction-type>
<staff>1</staff>
<sound dynamics="54.44"/>
</direction>
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
</measure>
<measure number="46" width="121.09">
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<direction placement="below">
<direction-type>
<dynamics default-x="5.32" default-y="-49.44" relative-y="-25.00">
<mf/>
</dynamics>
</direction-type>
<staff>1</staff>
<sound dynamics="88.89"/>
</direction>
<note default-x="61.30" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>3</duration>
<voice>1</voice>
<type>16th</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<beam number="2">begin</beam>
</note>
<note default-x="76.30" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>3</duration>
<voice>1</voice>
<type>16th</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">continue</beam>
<beam number="2">end</beam>
</note>
<note default-x="91.30" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
</measure>
<measure number="47" width="75.98">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
</measure>
<measure number="48" width="75.98">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
</measure>
<measure number="49" width="190.10">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>-0.00</right-margin>
</system-margins>
<system-distance>139.43</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>55.77</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="113.89" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="113.89" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="113.89" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="113.89" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="50" width="126.61">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="18.06" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="-4.66"/>
</notations>
</note>
<note default-x="18.06" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="-4.66"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<forward>
<duration>12</duration>
</forward>
<note default-x="46.55" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>2</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="85.68" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>2</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="18.06" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="51" width="127.98">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>24</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="71.89" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="97.98" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
</measure>
<measure number="52" width="170.66">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="52.33" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="52.33" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="52.33" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="114.77" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="114.77" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="140.86" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="52.33" default-y="-160.77">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="114.77" default-y="-155.77">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="53" width="167.72">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>18</duration>
<voice>1</voice>
<type>quarter</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="62.80" default-y="-20.00">
<rest/>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<staff>1</staff>
</note>
<note default-x="88.89" default-y="-20.00">
<rest/>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>1</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="117.35" default-y="-50.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="137.93" default-y="-45.00">
<pitch>
<step>F</step>
<octave>2</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="62.80" default-y="-115.77">
<rest/>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<staff>2</staff>
</note>
<note default-x="88.89" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>up</stem>
<staff>2</staff>
<notations>
<tuplet type="start" bracket="yes"/>
</notations>
</note>
<note default-x="117.35" default-y="-115.77">
<rest/>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>2</staff>
</note>
<note default-x="137.93" default-y="-115.77">
<rest/>
<duration>4</duration>
<voice>5</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<staff>2</staff>
<notations>
<tuplet type="stop"/>
</notations>
</note>
</measure>
<measure number="54" width="151.57">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
</root>
<kind text="m7b5">half-diminished</kind>
</harmony>
<note default-x="19.34" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<accidental>natural</accidental>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="58.47" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="58.47" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="58.47" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="110.64" default-y="-20.00">
<pitch>
<step>D</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="110.64" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="110.64" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="19.34" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<tie type="start"/>
<voice>5</voice>
<type>quarter</type>
<accidental>natural</accidental>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
<note default-x="58.47" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>6</duration>
<tie type="stop"/>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">begin</beam>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="84.55" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>5</voice>
<type>eighth</type>
<stem>up</stem>
<staff>2</staff>
<beam number="1">end</beam>
</note>
<note default-x="110.64" default-y="-150.77">
<pitch>
<step>D</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="55" width="127.78">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="71.89" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="97.98" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="56" width="440.97">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>0.00</left-margin>
<right-margin>0.00</right-margin>
</system-margins>
<system-distance>139.43</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>55.77</staff-distance>
</staff-layout>
</print>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus2">suspended-second</kind>
</harmony>
<note default-x="125.79" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="113.89" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<note default-x="125.79" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-23.46" default-y="-4.70"/>
</notations>
</note>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus4">suspended-fourth</kind>
</harmony>
<note default-x="219.80" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="219.80" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="313.82" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="376.49" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="125.79" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="266.81" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>18</duration>
<voice>5</voice>
<type>quarter</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="57" width="281.37">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>24</duration>
<voice>1</voice>
<type>half</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="154.22" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="216.90" default-y="-35.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>up</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="58" width="340.08">
<direction>
<direction-type>
<words default-y="31.40" relative-x="-29.30" relative-y="16.75">to Chorus</words>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus2">suspended-second</kind>
</harmony>
<note default-x="24.90" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="13.00" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="24.90" default-y="-10.00">
<chord/>
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="sus4">suspended-fourth</kind>
</harmony>
<note default-x="118.91" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
<notations>
<articulations>
<staccato/>
</articulations>
</notations>
</note>
<note default-x="118.91" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>12</duration>
<voice>1</voice>
<type>quarter</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="212.92" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
</note>
<note default-x="212.92" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="275.60" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>6</duration>
<voice>1</voice>
<type>eighth</type>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="24.90" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="118.91" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
<notations>
<articulations>
<staccato/>
</articulations>
</notations>
</note>
<note default-x="212.92" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="1" width="240.46">
<print new-system="yes">
<system-layout>
<system-margins>
<left-margin>50.00</left-margin>
<right-margin>-0.00</right-margin>
</system-margins>
<system-distance>139.43</system-distance>
</system-layout>
<staff-layout number="2">
<staff-distance>55.77</staff-distance>
</staff-layout>
</print>
<attributes>
<divisions>12</divisions>
<staves>2</staves>
</attributes>
<direction placement="above">
<direction-type>
<rehearsal default-x="-25.00" default-y="50.71" relative-y="30.00" font-weight="bold" font-size="14">Ending</rehearsal>
</direction-type>
<staff>1</staff>
</direction>
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<direction>
<direction-type>
<words default-y="28.15" relative-y="20.00">rit.</words>
</direction-type>
<staff>1</staff>
</direction>
<direction placement="below">
<direction-type>
<dynamics default-x="5.32" default-y="-43.39" relative-y="-25.00">
<mp/>
</dynamics>
</direction-type>
<staff>1</staff>
<sound dynamics="71.11"/>
</direction>
<note default-x="112.33" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="112.33" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<note default-x="112.33" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="112.33" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="2" width="146.18">
<harmony print-frame="no">
<root>
<root-step>E</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="18.06" default-y="-30.00">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<fermata type="upright" default-y="0.96" relative-y="5.00"/>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="18.06" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<note default-x="18.06" default-y="-5.00">
<chord/>
<pitch>
<step>G</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<arpeggiate default-x="-11.56" default-y="0.34"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="18.06" default-y="-145.77">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>36</duration>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<fermata type="upright" relative-y="5.00"/>
</notations>
</note>
</measure>
<measure number="3" width="329.43">
<harmony print-frame="no">
<root>
<root-step>D</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="13.20" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tuplet type="start" bracket="no"/>
</notations>
</note>
<note default-x="48.14" default-y="-10.00">
<pitch>
<step>F</step>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">continue</beam>
</note>
<note default-x="83.07" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<harmony print-frame="no">
<root>
<root-step>C</root-step>
</root>
<kind text="m">minor</kind>
<degree>
<degree-value>5</degree-value>
<degree-alter>1</degree-alter>
<degree-type>alter</degree-type>
</degree>
</harmony>
<note default-x="118.01" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tuplet type="start" bracket="no"/>
</notations>
</note>
<note default-x="152.95" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">continue</beam>
</note>
<note default-x="187.88" default-y="-15.00">
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
</notations>
</note>
<harmony print-frame="no">
<root>
<root-step>B</root-step>
<root-alter>-1</root-alter>
</root>
<kind text="m7">minor-seventh</kind>
</harmony>
<note default-x="222.82" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">begin</beam>
<notations>
<tuplet type="start" bracket="no"/>
</notations>
</note>
<note default-x="257.76" default-y="0.00">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">continue</beam>
</note>
<note default-x="292.69" default-y="-20.00">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>4</duration>
<voice>1</voice>
<type>eighth</type>
<time-modification>
<actual-notes>3</actual-notes>
<normal-notes>2</normal-notes>
</time-modification>
<stem>down</stem>
<staff>1</staff>
<beam number="1">end</beam>
<notations>
<tuplet type="stop"/>
<fermata type="upright" relative-y="5.00"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-150.77">
<pitch>
<step>D</step>
<alter>-1</alter>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="118.01" default-y="-155.77">
<pitch>
<step>C</step>
<octave>2</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
<note default-x="222.82" default-y="-160.77">
<pitch>
<step>B</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>12</duration>
<voice>5</voice>
<type>quarter</type>
<stem>up</stem>
<staff>2</staff>
</note>
</measure>
<measure number="4" width="146.18">
<harmony print-frame="no">
<root>
<root-step>A</root-step>
<root-alter>-1</root-alter>
</root>
<kind>major</kind>
</harmony>
<note default-x="18.06" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<arpeggiate default-x="-11.56" default-y="5.34"/>
</notations>
</note>
<note default-x="18.06" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<arpeggiate default-x="-11.56" default-y="5.34"/>
</notations>
</note>
<note default-x="18.06" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="start"/>
<arpeggiate default-x="-11.56" default-y="5.34"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="18.06" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<tie type="start"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="start"/>
</notations>
</note>
</measure>
<measure number="5" width="150.17">
<note default-x="13.20" default-y="-25.00">
<pitch>
<step>C</step>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
<fermata type="upright" default-y="6.00" relative-y="5.00"/>
</notations>
</note>
<note default-x="13.20" default-y="-15.00">
<chord/>
<pitch>
<step>E</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<note default-x="13.20" default-y="0.00">
<chord/>
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>3</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>1</voice>
<type>half</type>
<dot/>
<stem>down</stem>
<staff>1</staff>
<notations>
<tied type="stop"/>
</notations>
</note>
<backup>
<duration>36</duration>
</backup>
<note default-x="13.20" default-y="-165.77">
<pitch>
<step>A</step>
<alter>-1</alter>
<octave>1</octave>
</pitch>
<duration>36</duration>
<tie type="stop"/>
<voice>5</voice>
<type>half</type>
<dot/>
<stem>up</stem>
<staff>2</staff>
<notations>
<tied type="stop"/>
<fermata type="upright" relative-y="5.00"/>
</notations>
</note>
<barline location="right">
<bar-style>light-heavy</bar-style>
</barline>
</measure>
</part>
</score-partwise>
)V0G0N0");


    app.StartRendering();

    emscripten_set_main_loop(MainLoop, 60, false);

    return 0;
}

bool OnButtonEvent(int buttonType, int eventType)
{
    App& app = App::GetInstance();

    ButtonEvent buttonEvent;
    buttonEvent.type = InputEvent::InputEventType::Button;
    buttonEvent.buttonType = (ButtonEvent::ButtonType)buttonType;
    buttonEvent.eventType = (ButtonEvent::EventType)eventType;

    return app.OnButtonEvent(buttonEvent);
}


void AddAudioCallbacksToCpp(int writeMidiFP)
{
    MidiCallbacks& midiCallbacks = MidiCallbacks::GetInstance();

    midiCallbacks.WriteMidiCallback = reinterpret_cast<void (*)(const uint8_t*, int)>(writeMidiFP);
}

void AddFunctionsToCpp(int clearFP, int drawLineFP, int drawTextFP, int drawUTF16TextFP, int drawGlyphFP, int drawCubicCurveFP, int measureTextFP, int measureUTF16TextFP, int measureGlyphFP, int startPDFRenderFP, int endPDFRenderFP, int startNewPDFPageFP)
{
    LOGI("Adding functions (from c++)");

    Renderer& renderer = Renderer::GetInstance();

    renderer.ClearCallback = reinterpret_cast<void (*)()>(clearFP);

    renderer.DrawLineCallback = reinterpret_cast<void (*)(float, float, float, float, const char*)>(drawLineFP);
    renderer.DrawTextCallback = reinterpret_cast<void (*)(const char*, float, float, const char*)>(drawTextFP);
    renderer.DrawUTF16TextCallback = reinterpret_cast<void (*)(const uint16_t*, float, float, const char*)>(drawUTF16TextFP);
    renderer.DrawGlyphCallback = reinterpret_cast<void (*)(uint16_t, float, float, const char*)>(drawGlyphFP);
    renderer.DrawCubicCurveCallback = reinterpret_cast<void (*)(float, float, float, float, float, float, float, float, const char*)>(drawCubicCurveFP);

    renderer.MeasureTextCallback = reinterpret_cast<float* (*)(const char*, const char*)>(measureTextFP);
    renderer.MeasureUTF16TextCallback = reinterpret_cast<float* (*)(const uint16_t*, const char*)>(measureUTF16TextFP);
    renderer.MeasureGlyphCallback = reinterpret_cast<float* (*)(uint16_t, const char*)>(measureGlyphFP);

    renderer.StartPDFRenderCallback = reinterpret_cast<void (*)()>(startPDFRenderFP);
    renderer.EndPDFRenderCallback = reinterpret_cast<void (*)()>(endPDFRenderFP);
    renderer.StartNewPDFPageCallback = reinterpret_cast<void (*)()>(startNewPDFPageFP);
}

void callJsFunction()
{
    LOGI_TAG(LOG_TAG, "calling js function from c++");

    //std::shared_ptr<App> app = std::make_shared<App>();

    //app->LoadSongFromString("txt", "key C \n#v \nC.. Csus4.. C\nHow I _long to _breathe the air of _Heaven \n");

    #if 0
    app->LoadSongFromString("txt", R"V0G0N(
key C\n

#v\n

C.. Csus4.. C\n

How I _long to _breathe the air of _Heaven\n

Am.. F.. G

Where pain is _gone, and _mercy fills the _streets

C/E.. F.. C. G. Am..

To look _upon the _one who bled to _save _me _

F.. G.. C... Csus4. C

And walk with _Him for _all eterni_ty

#c

F.. G.. C... C/E.

There will be a _day when _all will bow be_fore Him. There will _be a

F.. Am.. G... C/E.

_day when _death will be no_  more. Standing _face to

F.. G.. C. Em. F..

_face with _He who died and _rose _a_gain

F.. G.. C. C/E. F.. Am.. G..

(1) _Holy, _Holy is the _Lord

(to Verse 2)

F.. G.. C... Csus4. C

(2) _Holy, _Holy is the _Lord

(to Verse 3)

#v

C.. Csus4.. C

And every _prayer we _prayed in desper_ation,

Am.. F.. G

The songs of _faith we _sang through doubt and _fear

C/E.. F.. C. G. Am..

In the _end we will _see that it was _worth _it _

F.. G.. C... Csus4. C

When He re_turns to _wipe away _our tears

#v

C.. Csus4.. C

And on that _day we _join the resur_rection

Am.. F.. G

And stand be_side the _heroes of the _faith

C/E.. F.. C. G. Am..

And with one _voice a _thousand gener_a_tions,

F.. G.. C

(1) Sing _worthy is the _Lamb who was _slain

(repeat)

F.. G.. Am.. C..

(2) Sing _worthy is the _Lamb who was _slain

F.. G.. C... Csus4. C

For_ever _He shall re_ign

#c

F.. G.. C... C/E.

So let it be to_day we _shout the hymn of _Heaven. With angles _and the

F.. Am.. G... C/E.

_Saints we _raise a mighty _roar. Glory _to our 

F.. G.. C. Em. F..

_God who _gave us life bey_ond _the _grave

F.. G.. C

(1) _Holy, _Holy is the _Lord

F.. G.. Am.. C/E..

(2) _Holy, _Holy is the _Lord

F.. G.. Am.. C/E..

_Holy, _Holy is the _Lord

F.. G.. C

_Holy, _Holy is the _Lord

)V0G0N");

#endif


    /*Renderer& renderer = Renderer::GetInstance();
    renderer.DrawLine({ 10.0f, 10.0f }, { 200.0f, 10.0f });

    Paint linePaint;

    linePaint.color = 0xFF00FFFF;
    linePaint.strokeWidth = 10.0f;

    renderer.DrawLine({ 50.0f, 500.0f }, { 500.0f, 550.0f }, linePaint);

    Paint textPaint;

    textPaint.color = 0x0000FFFF;
    textPaint.textSize = 50.0f;

    renderer.DrawText("Hello World from C++!!", { 20.0f, 50.0f }, textPaint);
    renderer.DrawGlyph(SMuFLID::fClef, { 300.0f, 300.0f }, textPaint);
    renderer.DrawGlyph(SMuFLID::gClef, { 350.0f, 300.0f });
    renderer.DrawGlyph(SMuFLID::cClef, { 400.0f, 300.0f }, linePaint);
    renderer.DrawGlyph(SMuFLID::flag256thDown, { 400.0f, 400.0f });*/
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("addFunctionsToCpp", &AddFunctionsToCpp);
    emscripten::function("addAudioCallbacksToCpp", &AddAudioCallbacksToCpp);
    emscripten::function("callJsFunction", &callJsFunction);
    emscripten::function("onButtonEvent", &OnButtonEvent);
}