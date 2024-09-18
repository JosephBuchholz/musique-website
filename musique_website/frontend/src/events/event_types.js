/**
 * Event types to communicate with C++ side.
 */

/**
 * Button types.
 */
export class ButtonType {
    static None = new ButtonType(0);
    static Play = new ButtonType(1);
    static Reset = new ButtonType(2);
    static Metronome = new ButtonType(3);
    static DownloadPDF = new ButtonType(4);
    static Export = new ButtonType(5);

    constructor(value) {
        this.value = value;
    }
}

/**
 * Event types for buttons (including toggle buttons).
 */
export class ButtonEventType {
    static None = new ButtonEventType(0);
    static Pressed = new ButtonEventType(1);
    static ToggledOn = new ButtonEventType(2);
    static ToggledOff = new ButtonEventType(3);

    constructor(value) {
        this.value = value;
    }
}

/**
 * Pointer event types.
 */
export class PointerEventType {
    static None = new PointerEventType(0);
    static Down = new PointerEventType(1);
    static Up = new PointerEventType(2);
    static Move = new PointerEventType(3);

    constructor(value) {
        this.value = value;
    }
}

/**
 * Keyboard event types (such as key up or key down).
 */
export class KeyboardEventType {
    static None = new KeyboardEventType(0);
    static Down = new KeyboardEventType(1);
    static Up = new KeyboardEventType(2);

    constructor(value) {
        this.value = value;
    }
}
