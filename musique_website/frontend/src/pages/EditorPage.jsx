import { useEffect, useRef } from "react";
import Header from "../components/Header";
import Module from "../cpp.js";

export default function EditorPage() {
    const canvasRef = useRef(null);

    useEffect(() => {
        function addFont(fontName, fontUrl) {
            var newFont = new FontFace(fontName, fontUrl);
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

        var currentFont = "musicFont";

        context.font = "48px musicFont";

        function usePaint(paint) {
            context.strokeStyle =
                "#" + paint.color.toString(16).padStart(8, "0");
            context.fillStyle = "#" + paint.color.toString(16).padStart(8, "0");
            context.lineWidth = paint.strokeWidth;
            context.font = paint.textSize.toString() + "px " + currentFont;
        }

        function drawLine(startX, startY, endX, endY, paint) {
            usePaint(paint);

            context.beginPath();
            context.moveTo(startX, startY);
            context.lineTo(endX, endY);
            context.stroke();
        }

        function drawText(text, posX, posY, paint) {
            currentFont = "plainFont";
            usePaint(paint);

            context.fillText(text, posX, posY);
        }

        function drawGlyph(codePoint, posX, posY, paint) {
            currentFont = "musicFont";
            usePaint(paint);

            context.fillText(String.fromCodePoint(codePoint), posX, posY);
        }

        Module().then((module) => {
            console.log("Module created!");

            function drawLineCpp(startX, startY, endX, endY, paintStrPtr) {
                var testPaint = {
                    color: 0xff0000ff,
                    strokeWidth: 1.0,
                };

                console.log("test paint object: " + JSON.stringify(testPaint));

                var paintString = module.UTF8ToString(paintStrPtr);
                console.log("paint object: " + paintString);

                var paint = JSON.parse(paintString);

                drawLine(startX, startY, endX, endY, paint);
            }

            function drawTextCpp(textStrPtr, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF8ToString(textStrPtr);

                drawText(text, posX, posY, paint);
            }

            function drawGlyphCpp(codePoint, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                drawGlyph(codePoint, posX, posY, paint);
            }

            var drawLineFP = module.addFunction(drawLineCpp, "viiiii");
            var drawTextFP = module.addFunction(drawTextCpp, "viiii");
            var drawGlyphFP = module.addFunction(drawGlyphCpp, "viiii");

            module.addFunctionsToCpp(drawLineFP, drawTextFP, drawGlyphFP);

            module.callJsFunction();
        });
    });

    return (
        <>
            <div className="flex flex-col h-screen">
                <Header></Header>

                <h1 className="text-3xl font-bold text-blue-950">
                    Hello, this is the editor!
                </h1>

                <div className="flex-1 flex justify-center items-center">
                    <canvas
                        ref={canvasRef}
                        className="border-2"
                        width="776"
                        height="600"
                    ></canvas>
                </div>
            </div>
        </>
    );
}

function Canvas() {
    const canvasRef = useRef(null);

    useEffect(() => {});

    return (
        <>
            <canvas ref={canvasRef} className="border-2"></canvas>
        </>
    );
}
