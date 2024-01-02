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

        var scale = 0.6;

        function usePaint(paint) {
            context.strokeStyle =
                "#" + paint.color.toString(16).padStart(8, "0");
            context.fillStyle = "#" + paint.color.toString(16).padStart(8, "0");
            context.lineWidth = paint.strokeWidth * scale;
            context.lineCap = paint.strokeCap;
            context.textAlign = "left";
        }

        function setFont(paint) {
            if (paint.isBold && paint.isItalic) currentFont = "boldItalicFont";
            else if (paint.isBold) currentFont = "boldFont";
            else if (paint.isItalic) currentFont = "italicFont";
            else if (paint.useMusicFont) currentFont = "musicFont";
            else currentFont = "plainFont";

            context.font = paint.textSize * 2.0 * scale + "px " + currentFont;
            context.textAlign = paint.align;
        }

        function drawLine(startX, startY, endX, endY, paint) {
            usePaint(paint);

            context.beginPath();
            context.moveTo(startX * scale, startY * scale);
            context.lineTo(endX * scale, endY * scale);
            context.stroke();
        }

        function drawText(text, posX, posY, paint) {
            usePaint(paint);
            setFont(paint);

            context.fillStyle = "#000000FF";

            context.fillText(text, posX * scale, posY * scale);
        }

        function drawGlyph(codePoint, posX, posY, paint) {
            currentFont = "musicFont";
            usePaint(paint);
            var fontSize = 40.0 * paint.glyphSizeFactor * scale;
            context.font = fontSize.toString() + "px " + currentFont;

            context.fillText(
                String.fromCodePoint(codePoint),
                posX * scale,
                posY * scale
            );
        }

        function drawCubicCurve(
            posX1,
            posY1,
            posX2,
            posY2,
            posX3,
            posY3,
            posX4,
            posY4,
            paint
        ) {
            usePaint(paint);

            context.beginPath();
            context.moveTo(posX1, posY1);
            context.bezierCurveTo(posX2, posY2, posX3, posY3, posX4, posY4);
            context.fill();
        }

        function measureText(text, paint) {
            currentFont = "plainFont";
            usePaint(paint);

            var textMetrics = context.measureText(text);
            return textMetrics;
        }

        function measureGlyph(codePoint, paint) {
            currentFont = "musicFont";
            usePaint(paint);
            var fontSize = 40.0 * paint.glyphSizeFactor * scale;
            context.font = fontSize.toString() + "px " + currentFont;

            var textMetrics = context.measureText(
                String.fromCodePoint(codePoint)
            );
            return textMetrics;
        }

        Module().then((module) => {
            console.log("Module created!");

            function drawLineCpp(startX, startY, endX, endY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                /*console.log(
                    "drawing line: paint: " +
                        paintString +
                        " startX: " +
                        startX +
                        " startY: " +
                        startY +
                        " endX: " +
                        endX +
                        " endY: " +
                        endY
                );*/

                drawLine(startX, startY, endX, endY, paint);
            }

            function drawTextCpp(textStrPtr, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF8ToString(textStrPtr);

                drawText(text, posX, posY, paint);
            }

            function drawUTF16TextCpp(textStrPtr, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF16ToString(textStrPtr);

                console.log(
                    "Drawing utf16 text: " +
                        text +
                        ", at: " +
                        posX +
                        ", " +
                        posY
                );

                drawText(text, posX, posY, paint);
            }

            function drawGlyphCpp(codePoint, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                /*console.log(
                    "Drawing glyph: " +
                        " | codePoint: " +
                        codePoint +
                        " paint object: " +
                        paintString +
                        " | positionX " +
                        posX +
                        " positionY " +
                        posY
                );*/

                drawGlyph(codePoint, posX, posY, paint);
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

                drawCubicCurve(
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

                var textMetrics = measureText(text, paint);

                const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / scale, // posX
                    -textMetrics.actualBoundingBoxAscent / scale, // posY
                    (textMetrics.actualBoundingBoxLeft +
                        textMetrics.actualBoundingBoxRight) /
                        scale, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        scale, // height
                ]);

                var buffer = arrayToCppPtr(boundingBoxArray, "float");

                return buffer;
            }

            function measureUTF16TextCpp(textStrPtr, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var text = module.UTF16ToString(textStrPtr);

                var textMetrics = measureText(text, paint);

                const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / scale, // posX
                    -textMetrics.actualBoundingBoxAscent / scale, // posY
                    (textMetrics.actualBoundingBoxLeft +
                        textMetrics.actualBoundingBoxRight) /
                        scale, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        scale, // height
                ]);

                var buffer = arrayToCppPtr(boundingBoxArray, "float");

                return buffer;
            }

            function measureGlyphCpp(codePoint, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                var textMetrics = measureGlyph(codePoint, paint);

                /*const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / scale, // posX
                    -textMetrics.actualBoundingBoxAscent / scale, // posY
                    (textMetrics.actualBoundingBoxLeft +
                        textMetrics.actualBoundingBoxRight) /
                        scale, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        scale, // height
                ]);*/

                // the array to return
                const boundingBoxArray = new Float32Array([
                    -textMetrics.actualBoundingBoxLeft / scale, // posX
                    -textMetrics.actualBoundingBoxAscent / scale, // posY
                    textMetrics.width * 2, // width
                    (textMetrics.actualBoundingBoxAscent +
                        textMetrics.actualBoundingBoxDescent) /
                        scale, // height
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
                drawLineFP,
                drawTextFP,
                drawUTF16TextFP,
                drawGlyphFP,
                drawCubicCurveFP,
                measureTextFP,
                measureUTF16TextFP,
                measureGlyphFP
            );

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
