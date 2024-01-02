#include <iostream>

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

#include "Debugging/Debug.h"
#include "Rendering/SMuFLGlyph.h"
#include "Collisions/Vec2.h"
#include "Rendering/Renderer.h"
#include "MusicData/Types.h"

#include "App.h"

void AddFunctionsToCpp(int drawLineFP, int drawTextFP, int drawUTF16TextFP, int drawGlyphFP, int drawCubicCurveFP, int measureTextFP, int measureUTF16TextFP, int measureGlyphFP)
{
    LOGI("Adding functions (from c++)");

    Renderer& renderer = Renderer::GetInstance();
    renderer.DrawLineCallback = reinterpret_cast<void (*)(float, float, float, float, const char*)>(drawLineFP);
    renderer.DrawTextCallback = reinterpret_cast<void (*)(const char*, float, float, const char*)>(drawTextFP);
    renderer.DrawUTF16TextCallback = reinterpret_cast<void (*)(const uint16_t*, float, float, const char*)>(drawUTF16TextFP);
    renderer.DrawGlyphCallback = reinterpret_cast<void (*)(uint16_t, float, float, const char*)>(drawGlyphFP);
    renderer.DrawCubicCurveCallback = reinterpret_cast<void (*)(float, float, float, float, float, float, float, float, const char*)>(drawCubicCurveFP);

    renderer.MeasureTextCallback = reinterpret_cast<float* (*)(const char*, const char*)>(measureTextFP);
    renderer.MeasureUTF16TextCallback = reinterpret_cast<float* (*)(const uint16_t*, const char*)>(measureUTF16TextFP);
    renderer.MeasureGlyphCallback = reinterpret_cast<float* (*)(uint16_t, const char*)>(measureGlyphFP);
}

void callJsFunction()
{
    LOGI_TAG(LOG_TAG, "calling js function from c++");

    std::shared_ptr<App> app = std::make_shared<App>();

    //app->LoadSongFromString("txt", "key C \n#v \nC.. Csus4.. C\nHow I _long to _breathe the air of _Heaven \n");

    app->LoadSongFromString("musicxml", R"V0G0N0(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise version="4.0">
  <work>
    <work-title>One and Only</work-title>
    </work>
  <identification>
    <creator type="composer">Rend Collective</creator>
    <encoding>
      <software>MuseScore 4.0.2</software>
      <encoding-date>2023-05-30</encoding-date>
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
    <credit-words default-x="616.935" default-y="1511.05" justify="center" valign="top" font-size="22">One and Only</credit-words>
    </credit>
  <credit page="1">
    <credit-type>composer</credit-type>
    <credit-words default-x="1148.14" default-y="1411.05" justify="right" valign="bottom">Rend Collective</credit-words>
    </credit>
  <credit page="1">
    <credit-words default-x="85.725" default-y="1511.05" justify="left" valign="top" font-size="14">B♭ Tin Whistle</credit-words>
    </credit>
  <part-list>
    <score-part id="P1">
      <part-name>B♭ Irish Tin Whistle</part-name>
      <part-abbreviation>B♭ Tin Wh.</part-abbreviation>
      <score-instrument id="P1-I1">
        <instrument-name>B♭ Tin Whistle</instrument-name>
        </score-instrument>
      <midi-device id="P1-I1" port="1"></midi-device>
      <midi-instrument id="P1-I1">
        <midi-channel>1</midi-channel>
        <midi-program>79</midi-program>
        <volume>78.7402</volume>
        <pan>0</pan>
        </midi-instrument>
      </score-part>
    </part-list>
  <part id="P1">
    <measure number="1" width="265.34">
      <print>
        <system-layout>
          <system-margins>
            <left-margin>50.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <top-system-distance>170.00</top-system-distance>
          </system-layout>
        </print>
      <attributes>
        <divisions>8</divisions>
        <key>
          <fifths>-2</fifths>
          </key>
        <time>
          <beats>3</beats>
          <beat-type>4</beat-type>
          </time>
        <clef>
          <sign>G</sign>
          <line>2</line>
          <clef-octave-change>1</clef-octave-change>
          </clef>
        </attributes>
      <direction placement="above">
        <direction-type>
          <metronome parentheses="no" default-x="-37.68" relative-y="20.00">
            <beat-unit>quarter</beat-unit>
            <per-minute>64</per-minute>
            </metronome>
          </direction-type>
        <sound tempo="64"/>
        </direction>
      <direction placement="below">
        <direction-type>
          <dynamics default-x="5.20" default-y="-40.00" relative-y="-25.00">
            <mf/>
            </dynamics>
          </direction-type>
        <sound dynamics="88.89"/>
        </direction>
      <note default-x="109.95" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="139.48" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="160.97" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="190.49" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="206.02" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="235.54" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        <notations>
          <tied type="start"/>
          <ornaments>
            <mordent/>
            </ornaments>
          </notations>
        </note>
      </measure>
    <measure number="2" width="102.13">
      <note default-x="13.00" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="65.40" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      </measure>
    <measure number="3" width="168.39">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="42.52" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="64.02" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="93.54" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="109.07" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="138.59" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        <notations>
          <tied type="start"/>
          <ornaments>
            <mordent/>
            </ornaments>
          </notations>
        </note>
      </measure>
    <measure number="4" width="125.32">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <tie type="stop"/>
        <tie type="start"/>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          <tied type="start"/>
          </notations>
        </note>
      <note default-x="65.40" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>4</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="88.69" default-y="-20.00">
        <rest/>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="5" width="0.00">
      <attributes>
        <measure-style>
          <multiple-rest>7</multiple-rest>
          </measure-style>
        </attributes>
      <direction>
        <direction-type>
          </direction-type>
        </direction>
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="6" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="7" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="8" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="9" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="10" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="11" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="12" width="168.39">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="42.52" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="64.02" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="93.54" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="109.07" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="138.59" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        <notations>
          <tied type="start"/>
          <ornaments>
            <mordent/>
            </ornaments>
          </notations>
        </note>
      </measure>
    <measure number="13" width="184.52">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="87.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="111.26" default-y="-5.00">
        <grace/>
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
        <notations>
          <slur type="start" placement="below" number="1"/>
          </notations>
        </note>
      <note default-x="123.86" default-y="-10.00">
        <grace/>
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <notations>
          <slur type="stop" number="1"/>
          </notations>
        </note>
      <note default-x="140.18" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="14" width="152.39">
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-39.97" relative-y="30.00" font-weight="bold" font-size="14">Verse 2</rehearsal>
          </direction-type>
        </direction>
      <note default-x="11.05" default-y="-10.00">
        <grace/>
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
        <notations>
          <slur type="start" placement="below" number="1"/>
          </notations>
        </note>
      <note default-x="23.65" default-y="-15.00">
        <grace/>
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <notations>
          <slur type="stop" number="1"/>
          </notations>
        </note>
      <note default-x="39.97" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="92.82" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="122.59" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="15" width="81.79">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>up</stem>
        </note>
      </measure>
    <measure number="16" width="119.13">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="65.85" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="89.34" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="17" width="119.13">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="65.85" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="89.34" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="18" width="125.42">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="65.85" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="95.62" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="19" width="102.88">
      <note default-x="13.00" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>up</stem>
        </note>
      <note default-x="65.85" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      </measure>
    <measure number="20" width="143.43">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="30.50" default-y="-10.00">
        <grace/>
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
        <notations>
          <slur type="start" placement="below" number="1"/>
          </notations>
        </note>
      <note default-x="43.09" default-y="0.00">
        <grace/>
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <notations>
          <slur type="stop" number="1"/>
          </notations>
        </note>
      <note default-x="59.42" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="82.91" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      <note default-x="106.39" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="21" width="227.21">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="77.33" default-y="-10.00">
        <grace/>
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
        <notations>
          <slur type="start" placement="below" number="1"/>
          </notations>
        </note>
      <note default-x="89.93" default-y="-15.00">
        <grace/>
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <notations>
          <slur type="stop" number="1"/>
          </notations>
        </note>
      <note default-x="106.25" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="156.91" default-y="-20.00">
        <rest/>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        </note>
      <note default-x="179.42" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
        </note>
      <note default-x="197.42" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        </note>
      </measure>
    <measure number="22" width="106.52">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>up</stem>
        </note>
      <note default-x="63.65" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="23" width="0.00">
      <attributes>
        <measure-style>
          <multiple-rest>8</multiple-rest>
          </measure-style>
        </attributes>
      <direction>
        <direction-type>
          </direction-type>
        </direction>
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="24" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="25" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="26" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="27" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="28" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="29" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="30" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="31" width="164.91">
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-13.00" relative-y="30.00" font-weight="bold">Interlude</rehearsal>
          </direction-type>
        </direction>
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="41.54" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="63.03" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="91.57" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="106.58" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="135.12" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        <notations>
          <tied type="start"/>
          <ornaments>
            <mordent/>
            </ornaments>
          </notations>
        </note>
      </measure>
    <measure number="32" width="99.22">
      <note default-x="13.00" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="63.65" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      </measure>
    <measure number="33" width="164.91">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="41.54" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="63.03" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="91.57" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="106.58" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="135.12" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        <notations>
          <tied type="start"/>
          <ornaments>
            <mordent/>
            </ornaments>
          </notations>
        </note>
      </measure>
    <measure number="34" width="106.52">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="63.65" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="35" width="193.66">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-25.00" default-y="14.50" relative-y="30.00" font-weight="bold" font-size="14">Verse 3</rehearsal>
          </direction-type>
        </direction>
      <note default-x="87.33" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="136.28" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="163.86" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="36" width="96.38">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>up</stem>
        </note>
      <note default-x="61.95" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      </measure>
    <measure number="37" width="113.50">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="61.95" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="83.71" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="38" width="113.50">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="61.95" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="83.71" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="39" width="96.38">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="61.95" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      </measure>
    <measure number="40" width="113.50">
      <note default-x="13.00" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>half</type>
        <stem>up</stem>
        <notations>
          <tied type="start"/>
          </notations>
        </note>
      <note default-x="61.95" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>4</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>eighth</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="83.71" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="41" width="188.27">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="41.49" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>32nd</type>
        <stem>down</stem>
        <beam number="1">continue</beam>
        <beam number="2">begin</beam>
        <beam number="3">begin</beam>
        </note>
      <note default-x="56.49" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>32nd</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <beam number="3">end</beam>
        </note>
      <note default-x="71.49" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="99.98" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="114.98" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="143.47" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>32nd</type>
        <stem>down</stem>
        <beam number="1">continue</beam>
        <beam number="2">begin</beam>
        <beam number="3">begin</beam>
        </note>
      <note default-x="158.47" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>32nd</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <beam number="3">end</beam>
        </note>
      </measure>
    <measure number="42" width="113.50">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="61.95" default-y="-20.00">
        <rest/>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        </note>
      <note default-x="83.71" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="43" width="232.44">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="87.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="117.36" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="133.15" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="163.17" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="178.96" default-y="5.00">
        <pitch>
          <step>G</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="202.64" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="44" width="152.97">
      <note default-x="11.05" default-y="-5.00">
        <grace slash="yes"/>
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <voice>1</voice>
        <type>eighth</type>
        <stem>up</stem>
        </note>
      <note default-x="31.72" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="85.01" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="102.51" default-y="-20.00">
        <grace slash="yes"/>
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <voice>1</voice>
        <type>eighth</type>
        <stem>up</stem>
        </note>
      <note default-x="123.17" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="45" width="110.92">
      <note default-x="13.00" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="66.29" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="46" width="137.53">
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-13.00" relative-y="30.00" font-weight="bold" font-size="14">Chorus</rehearsal>
          </direction-type>
        </direction>
      <direction placement="below">
        <direction-type>
          <dynamics default-x="5.20" default-y="-41.69" relative-y="-25.00">
            <mp/>
            </dynamics>
          </direction-type>
        <sound dynamics="71.11"/>
        </direction>
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="48.53" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="84.05" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="107.74" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="47" width="137.53">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="48.53" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="84.05" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="107.74" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="48" width="137.53">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="48.53" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="84.05" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="107.74" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="49" width="119.77">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="66.29" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="89.97" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="50" width="208.24">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="87.33" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="121.50" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="155.67" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="178.45" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="51" width="133.91">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="47.17" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="81.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="104.11" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="52" width="133.91">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="47.17" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="81.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="104.11" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="53" width="150.91">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>12</duration>
        <voice>1</voice>
        <type>quarter</type>
        <dot/>
        <stem>down</stem>
        </note>
      <direction placement="below">
        <direction-type>
          <wedge type="crescendo" number="1" default-y="-63.35"/>
          </direction-type>
        </direction>
      <note default-x="56.31" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        </note>
      <note default-x="79.09" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="101.87" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>3</duration>
        <voice>1</voice>
        <type>16th</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">continue</beam>
        <beam number="2">begin</beam>
        </note>
      <note default-x="121.12" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>32nd</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
        <beam number="3">backward hook</beam>
        </note>
      <direction placement="below">
        <direction-type>
          <wedge type="stop" number="1"/>
          </direction-type>
        </direction>
      </measure>
    <measure number="54" width="133.91">
      <direction placement="below">
        <direction-type>
          <dynamics default-x="1.90" default-y="-43.35" relative-y="-25.00">
            <f/>
            </dynamics>
          </direction-type>
        <sound dynamics="106.67"/>
        </direction>
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="47.17" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="81.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="104.11" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="55" width="133.91">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="47.17" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="81.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="104.11" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="56" width="133.91">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="47.17" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="81.33" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="104.11" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="57" width="199.85">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="87.33" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="144.60" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="170.05" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="58" width="144.60">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="51.18" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="89.35" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="114.81" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="59" width="144.60">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="51.18" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="89.35" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="114.81" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="60" width="144.60">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        </note>
      <note default-x="51.18" default-y="-30.00">
        <pitch>
          <step>G</step>
          <octave>5</octave>
          </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem>up</stem>
        </note>
      <note default-x="89.35" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="114.81" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        </note>
      </measure>
    <measure number="61" width="134.12">
      <note default-x="13.00" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>12</duration>
        <voice>1</voice>
        <type>quarter</type>
        <dot/>
        <stem>down</stem>
        </note>
      <note default-x="61.40" default-y="-20.00">
        <rest/>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        </note>
      <note default-x="86.85" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="62" width="173.52">
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-13.00" default-y="18.13" relative-y="30.00" font-weight="bold" font-size="14">Interlude</rehearsal>
          </direction-type>
        </direction>
      <direction>
        <direction-type>
          <words relative-y="20.00">double-time</words>
          </direction-type>
        </direction>
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="45.26" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="62.23" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="94.50" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="111.46" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="143.73" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="63" width="87.39">
      <note default-x="13.00" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="64" width="237.98">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="87.33" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="114.70" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="136.20" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="164.69" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="179.69" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="208.18" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="65" width="76.37">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="66" width="163.64">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="40.37" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="61.86" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="90.36" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="105.35" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="133.85" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="67" width="76.37">
      <note default-x="13.00" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="68" width="157.14">
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="40.37" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="55.36" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="83.86" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="98.85" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="127.35" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="69" width="83.67">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="70" width="157.14">
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-13.00" relative-y="30.00" font-weight="bold" font-size="14">Verse 4</rehearsal>
          </direction-type>
        </direction>
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="40.37" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="55.36" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="83.86" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="98.85" default-y="5.00">
        <pitch>
          <step>G</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="127.35" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="71" width="76.37">
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="72" width="257.04">
      <print new-system="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>-0.00</right-margin>
            </system-margins>
          <system-distance>103.17</system-distance>
          </system-layout>
        </print>
      <note default-x="87.33" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="121.86" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="140.02" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="174.55" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="192.71" default-y="5.00">
        <pitch>
          <step>G</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="227.24" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="73" width="92.49">
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="74" width="187.04">
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="47.53" default-y="15.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="70.03" default-y="10.00">
        <pitch>
          <step>A</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="104.56" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="122.71" default-y="5.00">
        <pitch>
          <step>G</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="157.24" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="75" width="92.69">
      <note default-x="13.20" default-y="15.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>24</duration>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        </note>
      </measure>
    <measure number="76" width="182.70">
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="47.53" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="65.69" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="100.22" default-y="-8.98">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="118.38" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="152.91" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="77" width="116.94">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="74.29" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>8</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        <notations>
          <tied type="start"/>
          </notations>
        </note>
      </measure>
    <measure number="78" width="99.79">
      <note default-x="13.00" default-y="0.00">
        <pitch>
          <step>F</step>
          <octave>6</octave>
          </pitch>
        <duration>24</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>half</type>
        <dot/>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="79" width="0.00">
      <print new-page="yes">
        <system-layout>
          <system-margins>
            <left-margin>0.00</left-margin>
            <right-margin>0.00</right-margin>
            </system-margins>
          <top-system-distance>112.63</top-system-distance>
          </system-layout>
        </print>
      <attributes>
        <measure-style>
          <multiple-rest>16</multiple-rest>
          </measure-style>
        </attributes>
      <direction>
        <direction-type>
          </direction-type>
        </direction>
      <direction>
        <direction-type>
          </direction-type>
        </direction>
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="80" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="81" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="82" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="83" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="84" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="85" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="86" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="87" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="88" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="89" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="90" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="91" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="92" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="93" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      </measure>
    <measure number="94" width="0.00">
      <note default-x="0.00" default-y="0.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      <barline location="right">
        <bar-style>light-light</bar-style>
        </barline>
      </measure>
    <measure number="95" width="182.53">
      <direction placement="above">
        <direction-type>
          <rehearsal default-x="-13.00" default-y="0.55" relative-y="30.00" font-weight="bold" font-size="14">Ending</rehearsal>
          </direction-type>
        </direction>
      <direction placement="below">
        <direction-type>
          <dynamics default-x="5.20" default-y="-40.00" relative-y="-25.00">
            <mf/>
            </dynamics>
          </direction-type>
        <sound dynamics="88.89"/>
        </direction>
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="46.53" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="68.03" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="101.57" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="119.20" default-y="-5.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="152.74" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="96" width="114.00">
      <note default-x="13.00" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>16</duration>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        </note>
      <note default-x="72.52" default-y="-20.00">
        <rest/>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        </note>
      </measure>
    <measure number="97" width="182.53">
      <note default-x="13.00" default-y="-35.00">
        <pitch>
          <step>F</step>
          <octave>5</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>up</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="46.53" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>up</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="68.03" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="101.57" default-y="-10.00">
        <pitch>
          <step>D</step>
          <octave>6</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      <note default-x="119.20" default-y="-15.00">
        <pitch>
          <step>C</step>
          <octave>6</octave>
          </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot/>
        <stem>down</stem>
        <beam number="1">begin</beam>
        </note>
      <note default-x="152.74" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>16th</type>
        <stem>down</stem>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
        </note>
      </measure>
    <measure number="98" width="128.31">
      <note default-x="13.00" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>16</duration>
        <tie type="start"/>
        <voice>1</voice>
        <type>half</type>
        <stem>down</stem>
        <notations>
          <tied type="start"/>
          </notations>
        </note>
      <note default-x="72.52" default-y="-20.00">
        <pitch>
          <step>B</step>
          <alter>-1</alter>
          <octave>5</octave>
          </pitch>
        <duration>4</duration>
        <tie type="stop"/>
        <voice>1</voice>
        <type>eighth</type>
        <stem>down</stem>
        <notations>
          <tied type="stop"/>
          </notations>
        </note>
      <note default-x="98.98" default-y="-20.00">
        <rest/>
        <duration>4</duration>
        <voice>1</voice>
        <type>eighth</type>
        </note>
      </measure>
    <measure number="99" width="102.75">
      <note default-x="37.73" default-y="-10.00">
        <rest measure="yes"/>
        <duration>24</duration>
        <voice>1</voice>
        </note>
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        </barline>
      </measure>
    </part>
  </score-partwise>

)V0G0N0");

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

    app->StartRendering();

    app->OnUpdate(0.1f);

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
    emscripten::function("callJsFunction", &callJsFunction);
}