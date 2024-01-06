export var scale = 1.0;
var currentFont = "musicFont";

export function usePaint(context, paint) {
    context.strokeStyle = "#" + paint.color.toString(16).padStart(8, "0");
    context.fillStyle = "#" + paint.color.toString(16).padStart(8, "0");
    context.lineWidth = paint.strokeWidth * scale;
    context.lineCap = paint.strokeCap;
    context.textAlign = "left";
}

export function usePaintForGlyph(context, paint) {
    currentFont = "musicFont";
    usePaint(context, paint);
    var fontSize = 40.0 * paint.glyphSizeFactor * scale;
    context.font = fontSize.toString() + "px " + currentFont;
}

export function setFont(context, paint) {
    if (paint.isBold && paint.isItalic) currentFont = "boldItalicFont";
    else if (paint.isBold) currentFont = "boldFont";
    else if (paint.isItalic) currentFont = "italicFont";
    else if (paint.useMusicFont) currentFont = "musicFont";
    else currentFont = "plainFont";

    context.font = paint.textSize * 2.0 * scale + "px " + currentFont;
    context.textAlign = paint.align;
}

export function clearCanvas(context) {
    console.log("clearing canvas!!!");
    context.clearRect(0, 0, context.canvas.width, context.canvas.height);
}

export function drawLine(context, startX, startY, endX, endY, paint) {
    usePaint(context, paint);

    context.strokeStyle = "#000000FF";
    context.fillStyle = "#000000FF";

    if (paint.verticalEnds) {
        context.beginPath();
        context.moveTo(
            startX * scale,
            (startY + paint.strokeWidth / 2) * scale
        );

        context.lineTo(endX * scale, (endY + paint.strokeWidth / 2) * scale);

        context.lineTo(endX * scale, (endY - paint.strokeWidth / 2) * scale);

        context.lineTo(
            startX * scale,
            (startY - paint.strokeWidth / 2) * scale
        );

        context.fill();
    } else {
        context.beginPath();
        context.moveTo(startX * scale, startY * scale);
        context.lineTo(endX * scale, endY * scale);
        context.stroke();
    }
}

export function drawText(context, text, posX, posY, paint) {
    usePaint(context, paint);
    setFont(context, paint);

    context.fillStyle = "#000000FF";

    context.fillText(text, posX * scale, posY * scale);
}

export function drawGlyph(context, codePoint, posX, posY, paint) {
    usePaintForGlyph(context, paint);

    context.fillText(
        String.fromCodePoint(codePoint),
        posX * scale,
        posY * scale
    );
}

export function drawCubicCurve(
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
) {
    usePaint(context, paint);

    context.strokeStyle = "#000000FF";
    context.fillStyle = "#000000FF";

    context.beginPath();
    context.moveTo(posX1 * scale, posY1 * scale);
    context.bezierCurveTo(
        posX2 * scale,
        posY2 * scale,
        posX3 * scale,
        posY3 * scale,
        posX4 * scale,
        posY4 * scale
    );
    context.stroke();

    if (paint.varyThickness) {
        var point1X = posX2 * scale;
        var point1Y = posY2 * scale;

        var point2X = posX3 * scale;
        var point2Y = posY3 * scale;

        var i = 0;
        while (i < 5) {
            point1Y -= paint.slurSpacing * scale;
            point2Y -= paint.slurSpacing * scale;

            context.beginPath();
            context.moveTo(posX1 * scale, posY1 * scale);
            context.bezierCurveTo(
                point1X,
                point1Y,
                point2X,
                point2Y,
                posX4 * scale,
                posY4 * scale
            );
            context.stroke();

            i++;
        }
    }
}

export function measureText(context, text, paint) {
    currentFont = "plainFont";
    usePaint(context, paint);

    var textMetrics = context.measureText(text);
    return textMetrics;
}

export function measureGlyph(context, codePoint, paint) {
    usePaintForGlyph(context, paint);

    var textMetrics = context.measureText(String.fromCodePoint(codePoint));

    return textMetrics;
}
