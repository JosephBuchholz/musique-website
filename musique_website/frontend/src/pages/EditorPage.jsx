import { useEffect, useRef, useState } from "react";
import Header from "../components/Header";
import Module from "../cpp.js";
import * as Tone from "tone";
import handleMidiMessage from "../audio/midi.js";
import * as Renderer from "../rendering/rendering.js";

function writeMidi(bytes, size) {
    var byteArray = new Uint8Array(size);

    for (let i = 0; i < size; i++) {
        var byte = module.getValue(bytes + i, "i8");
        byteArray[i] = byte;
    }

    handleMidiMessage(byteArray);
}

var module;
var moduleIsCreated = false;

function createModule() {
    if (!moduleIsCreated) {
        // create module
        return Module().then((mod) => {
            console.log("Creating Module!");

            module = mod;
            moduleIsCreated = true;

            console.log("Module created!");

            var writeMidiFP = module.addFunction(writeMidi, "vii");

            module.addAudioCallbacksToCpp(writeMidiFP);
        });
    }

    // else already exits
    return new Promise(function (resolve, reject) {
        resolve(module);
    });
}

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

        context.font = "48px musicFont";

        createModule().then((mod) => {
            console.log("adding rendering functions!!");

            function drawLineCpp(startX, startY, endX, endY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                Renderer.drawLine(context, startX, startY, endX, endY, paint);
            }

            function drawTextCpp(textStrPtr, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF8ToString(textStrPtr);

                Renderer.drawText(context, text, posX, posY, paint);
            }

            function drawUTF16TextCpp(textStrPtr, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF16ToString(textStrPtr);

                Renderer.drawText(context, text, posX, posY, paint);
            }

            function drawGlyphCpp(codePoint, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                Renderer.drawGlyph(context, codePoint, posX, posY, paint);
            }

            function drawCubicCurveCpp(
                posX1,
                posY1,
                posX2,
                posY2,
                posX3,
                posY3,
                posX4,
                posY4,
                paintStrPtr
            ) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                Renderer.drawCubicCurve(
                    context,
                    posX1,
                    posY1,
                    posX2,
                    posY2,
                    posX3,
                    posY3,
                    posX4,
                    posY4,
                    paint
                );
            }

            function measureTextCpp(textStrPtr, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF8ToString(textStrPtr);

                var textMetrics = Renderer.measureText(context, text, paint);

                const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / Renderer.scale, // posX
                    -textMetrics.actualBoundingBoxAscent / Renderer.scale, // posY
                    (textMetrics.actualBoundingBoxLeft +
                        textMetrics.actualBoundingBoxRight) /
                        Renderer.scale, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        Renderer.scale, // height
                ]);

                var buffer = arrayToCppPtr(boundingBoxArray, "float");

                return buffer;
            }

            function measureUTF16TextCpp(textStrPtr, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF16ToString(textStrPtr);

                var textMetrics = Renderer.measureText(context, text, paint);

                const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / Renderer.scale, // posX
                    -textMetrics.actualBoundingBoxAscent / Renderer.scale, // posY
                    (textMetrics.actualBoundingBoxLeft +
                        textMetrics.actualBoundingBoxRight) /
                        Renderer.scale, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        Renderer.scale, // height
                ]);

                var buffer = arrayToCppPtr(boundingBoxArray, "float");

                return buffer;
            }

            function measureGlyphCpp(codePoint, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var textMetrics = Renderer.measureGlyph(
                    context,
                    codePoint,
                    paint
                );

                // the array to return
                const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / Renderer.scale, // posX
                    -textMetrics.actualBoundingBoxAscent / Renderer.scale, // posY
                    textMetrics.width, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        Renderer.scale, // height
                ]);

                var buffer = arrayToCppPtr(boundingBoxArray, "float");

                return buffer;
            }

            function arrayToCppPtr(array, type) {
                // allocate buffer to hold the array
                var buffer = module._malloc(
                    array.length * array.BYTES_PER_ELEMENT
                );

                // copy the array into buffer
                array.forEach((value, index) => {
                    module.setValue(
                        buffer + array.BYTES_PER_ELEMENT * index,
                        value,
                        type
                    );
                });

                return buffer;
            }

            function clearCanvasCpp() {
                Renderer.clearCanvas(context);
            }

            var clearFP = module.addFunction(clearCanvasCpp, "v");

            var drawLineFP = module.addFunction(drawLineCpp, "vffffi");
            var drawTextFP = module.addFunction(drawTextCpp, "viffi");
            var drawUTF16TextFP = module.addFunction(drawUTF16TextCpp, "viffi");
            var drawGlyphFP = module.addFunction(drawGlyphCpp, "viffi");
            var drawCubicCurveFP = module.addFunction(
                drawCubicCurveCpp,
                "vffffffffi"
            );

            var measureTextFP = module.addFunction(measureTextCpp, "iii");
            var measureUTF16TextFP = module.addFunction(
                measureUTF16TextCpp,
                "iii"
            );
            var measureGlyphFP = module.addFunction(measureGlyphCpp, "iii");

            module.addFunctionsToCpp(
                clearFP,
                drawLineFP,
                drawTextFP,
                drawUTF16TextFP,
                drawGlyphFP,
                drawCubicCurveFP,
                measureTextFP,
                measureUTF16TextFP,
                measureGlyphFP
            );
        });
    });

    const [isPlaying, setIsPlaying] = useState(false);

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
                        id="canvas"
                        width="776"
                        height="600"
                    ></canvas>
                </div>

                <button
                    onClick={async () => {
                        await Tone.start();

                        if (moduleIsCreated) {
                            if (isPlaying) {
                                console.log("is playing so pausing");
                                setIsPlaying(false);
                                module.onButtonEvent(1, 3);
                            } else {
                                console.log("is paused so playing");
                                setIsPlaying(true);
                                module.onButtonEvent(1, 2);
                            }
                        }
                    }}
                >
                    Play
                </button>
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
