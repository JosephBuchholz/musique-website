import Renderer from "./renderer";

export class CanvasRenderer extends Renderer {
    millimeters = 6.35;
    tenths = 40.0;
    scale = (millimeters / tenths) * 4;
    currentFont = "musicFont";

    constructor(context) {
        super();
        this.context = context;
    }

    usePaint(paint) {
        this.context.strokeStyle =
            "#" + paint.color.toString(16).padStart(8, "0");
        this.context.fillStyle =
            "#" + paint.color.toString(16).padStart(8, "0");
        this.context.lineWidth = paint.strokeWidth * scale;
        this.context.lineCap = paint.strokeCap;
        this.context.textAlign = "left";
    }

    usePaintForGlyph(paint) {
        this.currentFont = "musicFont";
        usePaint(paint);
        var fontSize = 40.0 * paint.glyphSizeFactor * scale;
        this.context.font = fontSize.toString() + "px " + this.currentFont;
    }

    setFont(paint) {
        if (paint.isBold && paint.isItalic) this.currentFont = "boldItalicFont";
        else if (paint.isBold) this.currentFont = "boldFont";
        else if (paint.isItalic) this.currentFont = "italicFont";
        else if (paint.useMusicFont) this.currentFont = "musicFont";
        else this.currentFont = "plainFont";

        this.context.font =
            paint.textSize * 2.0 * scale + "px " + this.currentFont;
        this.context.textAlign = paint.align;
    }

    drawLine(startX, startY, endX, endY, paint) {
        this.usePaint(paint);

        this.context.strokeStyle = "#000000FF";
        this.context.fillStyle = "#000000FF";

        if (paint.verticalEnds) {
            this.context.beginPath();
            this.context.moveTo(
                startX * scale,
                (startY + paint.strokeWidth / 2) * scale
            );

            this.context.lineTo(
                endX * scale,
                (endY + paint.strokeWidth / 2) * scale
            );

            this.context.lineTo(
                endX * scale,
                (endY - paint.strokeWidth / 2) * scale
            );

            this.context.lineTo(
                startX * scale,
                (startY - paint.strokeWidth / 2) * scale
            );

            this.context.fill();
        } else {
            this.context.beginPath();
            this.context.moveTo(startX * scale, startY * scale);
            this.context.lineTo(endX * scale, endY * scale);
            this.context.stroke();
        }
    }

    drawText(text, posX, posY, paint) {
        this.usePaint(paint);
        this.setFont(paint);

        this.context.fillStyle = "#000000FF";

        this.context.fillText(text, posX * scale, posY * scale);
    }

    drawUTF16Text(text, posX, posY, paint) {
        this.drawText(text, posX, posY, paint);
    }

    usePaintForGlyph(paint) {
        this.currentFont = "musicFont";
        this.usePaint(paint);
        var fontSize = 40.0 * paint.glyphSizeFactor * scale;
        this.context.font = fontSize.toString() + "px " + this.currentFont;
    }

    drawGlyph(codePoint, posX, posY, paint) {
        this.usePaintForGlyph(paint);

        this.context.fillText(
            String.fromCodePoint(codePoint),
            posX * scale,
            posY * scale
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

        this.context.strokeStyle = "#000000FF";
        this.context.fillStyle = "#000000FF";

        this.context.beginPath();
        this.context.moveTo(posX1 * scale, posY1 * scale);
        this.context.bezierCurveTo(
            posX2 * scale,
            posY2 * scale,
            posX3 * scale,
            posY3 * scale,
            posX4 * scale,
            posY4 * scale
        );
        this.context.stroke();

        if (paint.varyThickness) {
            var point1X = posX2 * scale;
            var point1Y = posY2 * scale;

            var point2X = posX3 * scale;
            var point2Y = posY3 * scale;

            var i = 0;
            while (i < 5) {
                point1Y -= paint.slurSpacing * scale;
                point2Y -= paint.slurSpacing * scale;

                this.context.beginPath();
                this.context.moveTo(posX1 * scale, posY1 * scale);
                this.context.bezierCurveTo(
                    point1X,
                    point1Y,
                    point2X,
                    point2Y,
                    posX4 * scale,
                    posY4 * scale
                );
                this.context.stroke();

                i++;
            }
        }
    }

    clear() {
        console.log("clearing canvas!!!");
        this.context.clearRect(
            0,
            0,
            this.context.canvas.width,
            this.context.canvas.height
        );
    }

    measureText(context, text, paint) {
        this.currentFont = "plainFont";
        this.usePaint(paint);
        this.setFont(paint);

        var textMetrics = this.context.measureText(text);

        return textMetrics;
    }

    measureGlyph(context, codePoint, paint) {
        this.usePaintForGlyph(paint);

        var textMetrics = this.context.measureText(
            String.fromCodePoint(codePoint)
        );

        return textMetrics;
    }

    drawBox(boxArray, offsetX, offsetY, paint) {
        var linePaint = paint;
        linePaint.strokeWidth = 2.0;

        var x = offsetX;
        var y = offsetY;

        this.drawLine(
            boxArray[0] + x,
            boxArray[1] + y,
            boxArray[0] + boxArray[2] + x,
            boxArray[1] + y,
            linePaint
        );

        this.drawLine(
            boxArray[0] + boxArray[2] + x,
            boxArray[1] + boxArray[3] + y,
            boxArray[0] + boxArray[2] + x,
            boxArray[1] + y,
            linePaint
        );

        this.drawLine(
            boxArray[0] + x,
            boxArray[1] + boxArray[3] + y,
            boxArray[0] + x,
            boxArray[1] + y,
            linePaint
        );

        this.drawLine(
            boxArray[0] + x,
            boxArray[1] + boxArray[3] + y,
            boxArray[0] + boxArray[2] + x,
            boxArray[1] + boxArray[3] + y,
            linePaint
        );
    }
}

var millimeters = 6.35;
var tenths = 40.0;
export var scale = (millimeters / tenths) * 4;
