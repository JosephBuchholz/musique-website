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
}
