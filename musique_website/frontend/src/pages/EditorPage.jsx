import { useEffect, useRef, useState } from "react";
import Header from "../components/Header";
import Module from "../cpp.js";
import * as Tone from "tone";
import handleMidiMessage from "../audio/midi.js";
import * as Renderer from "../rendering/rendering.js";
import { jsPDF } from "jspdf";
import PDFRenderer from "../rendering/pdf_renderer.js";

function writeMidi(bytes, size) {
    var byteArray = new Uint8Array(size);

    for (let i = 0; i < size; i++) {
        var byte = module.getValue(bytes + i, "i8");
        byteArray[i] = byte;
    }

    handleMidiMessage(byteArray);
}

class ButtonType {
    static None = new ButtonType(0);
    static Play = new ButtonType(1);
    static Reset = new ButtonType(2);
    static Metronome = new ButtonType(3);
    static DownloadPDF = new ButtonType(4);

    constructor(value) {
        this.value = value;
    }
}

class ButtonEventType {
    static None = new ButtonType(0);
    static Pressed = new ButtonType(1);
    static ToggledOn = new ButtonType(2);
    static ToggledOff = new ButtonType(3);

    constructor(value) {
        this.value = value;
    }
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
    const pdfCanvasRef = useRef(null);

    const pageWidthTenths = 1233.87;
    const pageHeightTenths = 1596.77;

    const pageWidth = pageWidthTenths * Renderer.scale;
    const pageHeight = pageHeightTenths * Renderer.scale;

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

        let mainContext = canvas.getContext("2d");
        if (mainContext == null) throw Error("canvas is null");

        let pdfCanvas = pdfCanvasRef.current;
        if (pdfCanvas == null) throw Error("canvas is null");

        let pdfContext = pdfCanvas.getContext("2d");
        if (pdfContext == null) throw Error("canvas is null");

        var context = mainContext;
        var isRenderingPDF = false;

        context.font = "48px musicFont";

        var renderer = new Renderer.CanvasRenderer(context);

        var pdfDocument;

        createModule().then((mod) => {
            console.log("adding rendering functions!!");

            function drawLineCpp(startX, startY, endX, endY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                renderer.drawLine(startX, startY, endX, endY, paint);
            }

            function drawTextCpp(textStrPtr, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF8ToString(textStrPtr);

                renderer.drawText(text, posX, posY, paint);
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
                Renderer.clearCanvas(pdfContext);
            }

            const pdfPageWidth = 210;
            const pdfPageHeight = 297;

            function startPDFRender() {
                context = pdfContext;

                isRenderingPDF = true;
                console.log("Starting to render PDF!!!!");

                pdfDocument = new jsPDF("p", "mm", "a4");

                renderer = new PDFRenderer(
                    pdfDocument,
                    pageWidth,
                    pageHeight,
                    pdfPageWidth,
                    pdfPageHeight
                );
            }

            function endPDFRender() {
                context = mainContext;
                isRenderingPDF = false;

                var img = pdfCanvasRef.current.toDataURL();

                var width = pdfPageWidth;
                var height = (1 / (pageWidth / pageHeight)) * pdfPageWidth;

                var posY = (pdfPageHeight - height) / 2;

                pdfDocument.addImage(img, "PNG", 0, posY, width, height);

                //pdfDocument.text("Hello world!", 10, 10);
                pdfDocument.save("song.pdf");

                renderer = new Renderer.CanvasRenderer(context);

                console.log("Ending the render of PDF!");
            }

            function startNewPDFPage() {
                if (!isRenderingPDF) console.log("ERROR!!!");

                var img = pdfCanvasRef.current.toDataURL();

                var width = pdfPageWidth;
                var height = (1 / (pageWidth / pageHeight)) * pdfPageWidth;

                var posY = (pdfPageHeight - height) / 2;

                pdfDocument.addImage(img, "PNG", 0, posY, width, height);

                pdfDocument.addPage("p", "mm", "a4");

                console.log("Start new PDF page");
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

            var startPDFRenderFP = module.addFunction(startPDFRender, "v");
            var endPDFRenderFP = module.addFunction(endPDFRender, "v");
            var startNewPDFPageFP = module.addFunction(startNewPDFPage, "v");

            module.addFunctionsToCpp(
                clearFP,
                drawLineFP,
                drawTextFP,
                drawUTF16TextFP,
                drawGlyphFP,
                drawCubicCurveFP,
                measureTextFP,
                measureUTF16TextFP,
                measureGlyphFP,
                startPDFRenderFP,
                endPDFRenderFP,
                startNewPDFPageFP
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

                    <canvas
                        ref={pdfCanvasRef}
                        className="border-2 hidden"
                        id="pdf_canvas"
                        width={pageWidth}
                        height={pageHeight}
                    ></canvas>
                </div>

                <button
                    onClick={async () => {
                        await Tone.start();

                        if (moduleIsCreated) {
                            if (isPlaying) {
                                console.log("is playing so pausing");
                                setIsPlaying(false);
                                module.onButtonEvent(
                                    ButtonType.Play.value,
                                    ButtonEventType.ToggledOff.value
                                );
                            } else {
                                console.log("is paused so playing");
                                setIsPlaying(true);
                                module.onButtonEvent(
                                    ButtonType.Play.value,
                                    ButtonEventType.ToggledOn.value
                                );
                            }
                        }
                    }}
                >
                    Play
                </button>

                <button
                    onClick={() => {
                        const doc = new jsPDF("p", "mm", "a4");

                        var img = pdfCanvasRef.current.toDataURL();

                        doc.addImage(img, "PNG", 0, 0, 210, 297);

                        doc.text("Hello world!", 10, 10);
                        doc.save("a4.pdf");
                    }}
                >
                    PDF
                </button>

                <button
                    onClick={async () => {
                        if (moduleIsCreated) {
                            module.onButtonEvent(
                                ButtonType.DownloadPDF.value,
                                ButtonEventType.Pressed.value
                            );

                            /*const doc = new jsPDF("p", "mm", "a4");

                            var img = pdfCanvasRef.current.toDataURL();

                            doc.addImage(img, "PNG", 0, 0, 210, 297);

                            doc.text("Hello world!", 10, 10);
                            doc.save("a4.pdf");*/
                        }
                    }}
                >
                    Render PDF
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
