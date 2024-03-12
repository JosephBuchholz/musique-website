export default class Renderer {
    millimeters = 6.35;
    tenths = 40.0;

    constructor() {
        this.scale = (this.millimeters / this.tenths) * 4;
    }

    getScale() {
        return this.scale;
    }

    drawLine(startX, startY, endX, endY, paint) {}
    drawText(text, posX, posY, paint) {}
    drawGlyph(codePoint, posX, posY, paint) {}
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
    ) {}
}
