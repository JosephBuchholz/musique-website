import Renderer from "./renderer";
import { open_sansNormal } from "./open_sans-normal";
import { bravuraNormal } from "./bravura-normal";

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

        this.pdfDocument.addFileToVFS("bravura-normal.ttf", bravuraNormal);
        this.pdfDocument.addFont("bravura-normal.ttf", "bravura", "normal");
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
        }

        return "normal";
    }

    setFont(paint) {
        //console.log(this.pdfDocument.getFontList());

        var font = "Times";

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

    drawGlyph(codePoint, posX, posY, paint) {
        this.pdfDocument.setFont("bravura", "normal");

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
}
