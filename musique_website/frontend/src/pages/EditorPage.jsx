import { useEffect, useRef } from "react";
import Header from "../components/Header";
import Module from '../cpp.js';

export default function EditorPage() {


  const canvasRef = useRef(null);

  useEffect(() => {

    function addFont(fontName, fontUrl) {
      var newFont = new FontFace(fontName, fontUrl)
      newFont.load().then(() => {
        document.fonts.add(newFont);
      });
    }

    addFont("plainFont", "url(/static/fonts/times.ttf)");
    addFont("italicFont", "url(/static/fonts/timesi.ttf)");
    addFont("boldFont", "url(/static/fonts/timesbd.ttf)");
    addFont("boldItalicFont", "url(/static/fonts/timesbi.ttf)");

    addFont("tablatureFont", "url(/static/fonts/open_sans.ttf)");

    addFont("musicFont", "url(/static/fonts/bravura.otf)");
    //addFont("musicTextFont", "url(/static/fonts/bravura_text.otf)");

    let canvas = canvasRef.current;
    if (canvas == null) throw Error("canvas is null");

    let context = canvas.getContext("2d");
    if (context == null) throw Error("canvas is null");

    context.font = "48px musicFont";

    function drawLine(startX, startY, endX, endY) {
      context.beginPath();
      context.moveTo(startX, startY);
      context.lineTo(endX, endY);
      context.stroke();
    }

    function drawGlyph(codePoint, posX, posY) {
      context.font = "48px musicFont";
      context.fillText(String.fromCodePoint(codePoint), posX, posY);
    }

    

    console.log("Hello outside");

    Module()
      .then(module => {
        console.log('Module created!');

        var strLen = module.stringLength("Hello World");
        console.log("string length: " + strLen)

        function drawText(text, posX, posY) {
          context.font = "48px plainFont";
          var textString = module.UTF8ToString(text);
          context.fillText(textString, posX, posY);
        }

        var drawLineFP = module.addFunction(drawLine, "viiii");
        var drawTextFP = module.addFunction(drawText, "viii");
        var drawGlyphFP = module.addFunction(drawGlyph, "viii");

        module.addFunctionsToCpp(drawLineFP, drawTextFP, drawGlyphFP);

        module.callJsFunction();

        //drawGlyph(0xE050, 300, 300);
    });
  });

  return (
    <>
      <div className="flex flex-col h-screen">
        <Header></Header>

        <div className="flex-1 flex justify-center items-center">
          <h1 className="text-3xl font-bold text-blue-950">
            Hello, this is the editor!
          </h1>

          {/*<Canvas></Canvas>*/}

          <canvas ref={canvasRef} className="border-2" width="776" height="600"></canvas>
        </div>
      </div>
    </>
  );
}

function Canvas() {
  const canvasRef = useRef(null);

  useEffect(() => {
    
  });

  return (
    <>
      <canvas ref={canvasRef} className="border-2"></canvas>
    </>
  );
}
