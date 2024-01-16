import Renderer from "./renderer";

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
    }

    usePaint(paint) {
        this.pdfDocument.setLineWidth(paint.strokeWidth * super.getScale());
        this.pdfDocument.setLineCap(paint.strokeCap);
        this.pdfDocument.setFontA;
    }

    drawLine(startX, startY, endX, endY, paint) {
        this.usePaint(paint);

        var width = this.pdfPageWidth;
        var height =
            (1 / (this.pageWidth / this.pageHeight)) * this.pdfPageWidth;

        var posY = (this.pdfPageHeight - height) / 2;

        var scale = super.getScale();

        this.pdfDocument.line(
            startX *
                scale *
                0.25 *
                (this.pdfPageWidth / (this.pageWidth * 0.25)),
            startY *
                scale *
                0.25 *
                (this.pdfPageWidth / (this.pageWidth * 0.25)) +
                posY,
            endX * scale * 0.25 * (this.pdfPageWidth / (this.pageWidth * 0.25)),
            endY *
                scale *
                0.25 *
                (this.pdfPageWidth / (this.pageWidth * 0.25)) +
                posY
        );
    }

    drawText(text, posX, posY, paint) {
        var width = this.pdfPageWidth;
        var height =
            (1 / (this.pageWidth / this.pageHeight)) * this.pdfPageWidth;

        var y = (this.pdfPageHeight - height) / 2;

        var scale = super.getScale();

        this.pdfDocument.setFont("Times-Roman");

        this.pdfDocument.setFontSize(
            paint.textSize *
                2.0 *
                scale *
                0.25 *
                (this.pdfPageWidth / (this.pageWidth * 0.25)) *
                2.83465
        );

        this.pdfDocument.text(
            text,
            posX * scale * 0.25 * (this.pdfPageWidth / (this.pageWidth * 0.25)),
            posY *
                scale *
                0.25 *
                (this.pdfPageWidth / (this.pageWidth * 0.25)) +
                y,
            {
                align: paint.textAlign,
            }
        );
    }
}
