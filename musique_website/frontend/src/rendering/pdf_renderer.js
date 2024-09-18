/**
 * Renderer functions for rendering to a PDF file.
 */

import Renderer from "./renderer";
import { open_sansNormal } from "./open_sans-normal";
import { leipizigNormal } from "./Leipzig-normal";

/**
 * Class with utilites to render to a PDF.
 */
export default class PDFRenderer extends Renderer {
    constructor(
        pdfDocument,
        pageWidth,
        pageHeight,
        pdfPageWidth,
        pdfPageHeight
    ) {
        super();
        this.pdfDocument = pdfDocument;
        this.pageWidth = pageWidth;
        this.pageHeight = pageHeight;
        this.pdfPageWidth = pdfPageWidth;
        this.pdfPageHeight = pdfPageHeight;
        this.canvasCurrentFont = "plainfont";

        var millimeters = 6.35;
        var tenths = 40.0;
        this.canvasScale = (millimeters / tenths) * 4;

        this.scale =
            super.getScale() *
            0.25 *
            (this.pdfPageWidth / (this.pageWidth * 0.25));

        this.width = this.pdfPageWidth;
        this.height =
            (1 / (this.pageWidth / this.pageHeight)) * this.pdfPageWidth;

        this.offsetY = (this.pdfPageHeight - this.height) / 2;

        this.pdfDocument.addFileToVFS("open_sans-normal.ttf", open_sansNormal);
        this.pdfDocument.addFont("open_sans-normal.ttf", "open_sans", "normal");

        //this.pdfDocument.addFileToVFS("bravura-normal.ttf", leipizig);
        //this.pdfDocument.addFont("bravura-normal.ttf", "bravura", "normal");

        this.pdfDocument.addFileToVFS("leipizig-normal.ttf", leipizigNormal);
        this.pdfDocument.addFont("leipizig-normal.ttf", "leipizig", "normal");
    }

    usePaint(paint) {
        this.pdfDocument.setLineWidth(paint.strokeWidth * this.scale);
        this.pdfDocument.setLineCap(paint.strokeCap);
    }

    // gets the correct font style string for the given font and bold/italic parameters
    getFontStyleString(font, isBold, isItalic) {
        if (font == "Times") {
            if (isBold && isItalic) {
                return "BoldItalic";
            } else if (isBold) {
                return "Bold";
            } else if (isItalic) {
                return "Italic";
            } else {
                return "Roman";
            }
        } else if ((font = "bravura")) {
            return "normal";
        } else if ((font = "leipizig")) {
            return "normal";
        }

        return "normal";
    }

    setFont(paint) {
        var font = "Times";
        if (paint.useMusicFont) font = "leipizig";

        // sets font and font style
        this.pdfDocument.setFont(
            font,
            this.getFontStyleString(font, paint.isBold, paint.isItalic)
        );

        this.pdfDocument.setFontSize(
            paint.textSize * 2.0 * this.scale * 2.83465
        );
    }

    drawLine(startX, startY, endX, endY, paint) {
        this.usePaint(paint);

        /*console.log({
            width: this.width,
            height: this.height,
            pdfWidth: this.pdfPageWidth,
            pdfHeight: this.pdfPageHeight,
            offsetY: this.offsetY,
            pageWidth: this.pageWidth,
            pageHeight: this.pageHeight,
            scale: this.scale,
            canvasScale: this.canvasScale,
        });*/

        if (paint.verticalEnds) {
            var width = paint.strokeWidth;

            this.pdfDocument.path([
                {
                    op: "m",
                    c: [
                        startX * this.scale,
                        (startY + width / 2) * this.scale + this.offsetY,
                    ],
                },
                {
                    op: "l",
                    c: [
                        endX * this.scale,
                        (endY + width / 2) * this.scale + this.offsetY,
                    ],
                },
                {
                    op: "l",
                    c: [
                        endX * this.scale,
                        (endY - width / 2) * this.scale + this.offsetY,
                    ],
                },
                {
                    op: "l",
                    c: [
                        startX * this.scale,
                        (startY - width / 2) * this.scale + this.offsetY,
                    ],
                },
                { op: "h", c: [] },
            ]);

            this.pdfDocument.fill();
        } else {
            this.pdfDocument.line(
                startX * this.scale,
                startY * this.scale + this.offsetY,
                endX * this.scale,
                endY * this.scale + this.offsetY
            );
        }
    }

    drawText(text, posX, posY, paint) {
        this.setFont(paint);

        this.pdfDocument.text(
            text,
            posX * this.scale,
            posY * this.scale + this.offsetY,
            {
                align: paint.align,
            }
        );
    }

    drawUTF16Text(text, posX, posY, paint) {
        this.drawText(text, posX, posY, paint);
    }

    drawGlyph(codePoint, posX, posY, paint) {
        this.pdfDocument.setFont("leipizig", "normal");

        this.pdfDocument.setFontSize(
            paint.glyphSizeFactor * 40.0 * this.scale * 2.83465
        );

        this.pdfDocument.text(
            String.fromCodePoint(codePoint),
            posX * this.scale,
            posY * this.scale + this.offsetY,
            {
                align: "left",
            }
        );
    }

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
    ) {
        this.usePaint(paint);

        this.pdfDocument.lines(
            [
                [
                    (posX2 - posX1) * this.scale,
                    (posY2 - posY1) * this.scale,
                    (posX3 - posX1) * this.scale,
                    (posY3 - posY1) * this.scale,
                    (posX4 - posX1) * this.scale,
                    (posY4 - posY1) * this.scale,
                ],
            ],
            posX1 * this.scale,
            posY1 * this.scale + this.offsetY
        );
    }

    clear() {
        console.log("clearing pdf canvas!!!");
    }

    usePaintCanvas(context, paint) {
        context.strokeStyle = "#" + paint.color.toString(16).padStart(8, "0");
        context.fillStyle = "#" + paint.color.toString(16).padStart(8, "0");
        context.lineWidth = paint.strokeWidth * this.canvasScale;
        context.lineCap = paint.strokeCap;
        context.textAlign = "left";
    }

    usePaintForGlyphCanvas(context, paint) {
        this.canvasCurrentFont = "musicFont";
        this.usePaintCanvas(context, paint);
        var fontSize = 40.0 * paint.glyphSizeFactor * this.canvasScale;
        context.font = fontSize.toString() + "px " + this.canvasCurrentFont;
    }

    measureText(context, text, paint) {
        this.canvasCurrentFont = "plainFont";
        this.usePaintCanvas(context, paint);

        if (paint.isBold && paint.isItalic)
            this.canvasCurrentFont = "boldItalicFont";
        else if (paint.isBold) this.canvasCurrentFont = "boldFont";
        else if (paint.isItalic) this.canvasCurrentFont = "italicFont";
        else if (paint.useMusicFont) this.canvasCurrentFont = "musicFont";
        else this.canvasCurrentFont = "plainFont";

        context.font =
            paint.textSize * 2.0 * this.canvasScale +
            "px " +
            this.canvasCurrentFont;
        context.textAlign = paint.align;

        var textMetrics = context.measureText(text);
        return textMetrics;
    }

    measureGlyph(context, codePoint, paint) {
        this.usePaintForGlyphCanvas(context, paint);

        var textMetrics = context.measureText(String.fromCodePoint(codePoint));

        return textMetrics;
    }
}
