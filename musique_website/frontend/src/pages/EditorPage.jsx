import { useEffect, useRef, useState } from "react";
import Header from "../components/Header";
import Module from "../cpp.js";
import * as Tone from "tone";
import handleMidiMessage from "../audio/midi.js";
import * as Renderer from "../rendering/canvas_renderer.js";
import { jsPDF } from "jspdf";
import PDFRenderer from "../rendering/pdf_renderer.js";

function getASeriesPageWidthMM(size) {
    return ((1 / Math.pow(2, 1 / 4)) * 1000.0) / Math.pow(2, size / 2);
}

function getASeriesPageHeightMM(size) {
    return getASeriesPageWidthMM(size) * Math.sqrt(2);
}

function getBSeriesPageWidthMM(size) {
    return getASeriesPageWidthMM(size) * Math.pow(2, 1 / 4);
}

function getBSeriesPageHeightMM(size) {
    return getASeriesPageHeightMM(size) * Math.pow(2, 1 / 4);
}

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
    static Export = new ButtonType(5);

    constructor(value) {
        this.value = value;
    }
}

class ButtonEventType {
    static None = new ButtonEventType(0);
    static Pressed = new ButtonEventType(1);
    static ToggledOn = new ButtonEventType(2);
    static ToggledOff = new ButtonEventType(3);

    constructor(value) {
        this.value = value;
    }
}

class PointerEventType {
    static None = new PointerEventType(0);
    static Down = new PointerEventType(1);
    static Up = new PointerEventType(2);
    static Move = new PointerEventType(3);

    constructor(value) {
        this.value = value;
    }
}

class KeyboardEventType {
    static None = new KeyboardEventType(0);
    static Down = new KeyboardEventType(1);
    static Up = new KeyboardEventType(2);

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

var scalingMM = 6.35;
var scalingTenths = 40;

function MMToTenths(value) {
    return (scalingTenths / scalingMM) * value;
}

function TenthsToMM(value) {
    return (scalingMM / scalingTenths) * value;
}

var pageSize = "a4";

var pageWidthMM = 210;
var pageHeightMM = 297;

var pageWidthTenths = MMToTenths(pageWidthMM);
var pageHeightTenths = MMToTenths(pageHeightMM);

var pageWidthModalProps = pageWidthMM;
var pageHeightModalProps = pageHeightMM;

function setPageSizeMM(width, height) {
    pageWidthMM = width;
    pageHeightMM = height;

    pageWidthTenths = MMToTenths(width);
    pageHeightTenths = MMToTenths(height);
}

export default function EditorPage() {
    const canvasRef = useRef(null);
    const pdfCanvasRef = useRef(null);
    const canvasDiv = useRef(null);
    const propertiesList = useRef(null);

    const [editableProperties, setEditableProperties] = useState("");

    //const pageWidthTenths = 1233.87;
    //const pageHeightTenths = 1596.77;

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
        canvas.onselectstart = () => {
            return false;
        };

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

                renderer.drawUTF16Text(text, posX, posY, paint);
            }

            function drawGlyphCpp(codePoint, posX, posY, paintStrPtr) {
                var paintString = module.UTF8ToString(paintStrPtr);
                var paint = JSON.parse(paintString);

                renderer.drawGlyph(codePoint, posX, posY, paint);
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

                renderer.drawCubicCurve(
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

                var textMetrics = renderer.measureText(context, text, paint);

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

                var textMetrics = renderer.measureText(context, text, paint);

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

                var textMetrics = renderer.measureGlyph(
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
                renderer.clear();
            }

            const pdfPageWidth = pageWidthMM;
            const pdfPageHeight = pageHeightMM;

            var isFirstPage = true;

            function startPDFRender() {
                context = pdfContext;

                isRenderingPDF = true;
                isFirstPage = true;

                if (pageSize == "custom") {
                    pdfDocument = new jsPDF("p", "mm", [
                        pageWidthMM,
                        pageHeightMM,
                    ]);
                } else {
                    pdfDocument = new jsPDF("p", "mm", pageSize);
                }

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

                //pdfDocument.text("Hello world!", 10, 10);
                pdfDocument.save("song.pdf");

                renderer = new Renderer.CanvasRenderer(context);
            }

            function startNewPDFPage() {
                if (!isRenderingPDF) console.log("ERROR!!!");

                var img = pdfCanvasRef.current.toDataURL();

                var width = pdfPageWidth;
                var height = (1 / (pageWidth / pageHeight)) * pdfPageWidth;

                var posY = (pdfPageHeight - height) / 2;

                if (!isFirstPage) {
                    if (pageSize == "custom") {
                        pdfDocument.addPage("p", "mm", [
                            pageWidthMM,
                            pageHeightMM,
                        ]);
                    } else {
                        pdfDocument.addPage("p", "mm", pageSize);
                    }
                } else {
                    // a page was already added when document was created
                    isFirstPage = false;
                }
            }

            function downloadText(nameStr, dataStr) {
                var name = module.UTF8ToString(nameStr);
                var data = module.UTF8ToString(dataStr);

                const file = new File([data], name, {
                    type: "text/plain",
                });

                const link = document.createElement("a");
                const url = URL.createObjectURL(file);

                link.href = url;
                link.download = file.name;
                document.body.appendChild(link);
                link.click();

                document.body.removeChild(link);
                window.URL.revokeObjectURL(url);
            }

            function updateProperties(propertiesStr) {
                console.log("updating properties!!");
                setEditableProperties(
                    JSON.parse(module.UTF8ToString(propertiesStr))
                );
            }

            function updateCanvasSize() {
                canvas.width = canvasDiv.current.offsetWidth;
                canvas.height = canvasDiv.current.offsetHeight;
                module.onCanvasResize(canvas.width, canvas.height);
            }
            canvas.width = canvasDiv.current.offsetWidth;
            canvas.height = canvasDiv.current.offsetHeight;
            window.addEventListener("resize", updateCanvasSize);

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

            var downloadTextFP = module.addFunction(downloadText, "vii");
            var updatePropertiesFP = module.addFunction(updateProperties, "vi");
            module.addCallbackFunctionsToCpp(
                downloadTextFP,
                updatePropertiesFP
            );

            module.onCanvasResize(canvas.width, canvas.height);
        });

        // --- Event Listeners ---

        function keydownEventListener(event) {
            const key = event.key;
            console.log("key: " + key);

            if (moduleIsCreated) {
                module.onKeyboardEvent(
                    KeyboardEventType.Down.value,
                    module.stringToNewUTF8(event.key)
                );
            }
        }

        function pointerDownEventListener(event) {
            if (moduleIsCreated) {
                var millimeters = 6.35;
                var tenths = 40.0;
                var scale = (millimeters / tenths) * 4;

                module.onPointerEvent(
                    PointerEventType.Down.value,
                    event.offsetX / scale,
                    event.offsetY / scale
                );
            }
        }

        function pointerMoveEventListener(event) {
            if (moduleIsCreated) {
                var millimeters = 6.35;
                var tenths = 40.0;
                var scale = (millimeters / tenths) * 4;

                var rect = canvas.getBoundingClientRect();

                module.onPointerEvent(
                    PointerEventType.Move.value,
                    (event.pageX - rect.left) / scale,
                    (event.pageY - rect.top) / scale
                );
            }
        }

        function pointerUpEventListener(event) {
            if (moduleIsCreated) {
                var millimeters = 6.35;
                var tenths = 40.0;
                var scale = (millimeters / tenths) * 4;

                var rect = canvas.getBoundingClientRect();

                module.onPointerEvent(
                    PointerEventType.Up.value,
                    (event.pageX - rect.left) / scale,
                    (event.pageY - rect.top) / scale
                );
            }
        }

        document.addEventListener("keydown", keydownEventListener, true);
        canvas.addEventListener("pointerdown", pointerDownEventListener);
        document.addEventListener("pointermove", pointerMoveEventListener);
        document.addEventListener("pointerup", pointerUpEventListener);

        return () => {
            document.removeEventListener("keydown", keydownEventListener, true);
            canvas.removeEventListener("pointerdown", pointerDownEventListener);
            document.removeEventListener(
                "pointermove",
                pointerMoveEventListener
            );
            document.removeEventListener("pointerup", pointerUpEventListener);
        };
    });

    const [showPagePropsModal, setShowPagePropsModal] = useState(false);
    const [pageSizePagePropsModel, setPageSizePagePropsModel] = useState("a4");
    const [pageWidthCustom, setPageWidthCustom] = useState(1.0);
    const [pageHeightCustom, setPageHeightCustom] = useState(1.0);

    function setPageSizeInchesProps(width, height) {
        pageWidthModalProps = Math.round(inchesToMM(width));
        pageHeightModalProps = Math.round(inchesToMM(height));
    }

    function setPageSizeASeriesProps(size) {
        pageWidthModalProps = Math.round(getASeriesPageWidthMM(size));
        pageHeightModalProps = Math.round(getASeriesPageHeightMM(size));
    }

    function setPageSizeBSeriesProps(size) {
        pageWidthModalProps = Math.round(getBSeriesPageWidthMM(size));
        pageHeightModalProps = Math.round(getBSeriesPageHeightMM(size));
    }

    function inchesToMM(value) {
        return value * 25.4;
    }

    if (pageSizePagePropsModel == "a2") {
        setPageSizeASeriesProps(2);
    } else if (pageSizePagePropsModel == "a3") {
        setPageSizeASeriesProps(3);
    } else if (pageSizePagePropsModel == "a4") {
        setPageSizeASeriesProps(4);
    } else if (pageSizePagePropsModel == "a5") {
        setPageSizeASeriesProps(5);
    } else if (pageSizePagePropsModel == "b4") {
        setPageSizeBSeriesProps(4);
    } else if (pageSizePagePropsModel == "b5") {
        setPageSizeBSeriesProps(5);
    } else if (pageSizePagePropsModel == "letter") {
        setPageSizeInchesProps(8.5, 11);
    } else if (pageSizePagePropsModel == "legal") {
        setPageSizeInchesProps(8.5, 14);
    } else if (pageSizePagePropsModel == "tabloid") {
        setPageSizeInchesProps(11, 17);
    } else if (pageSizePagePropsModel == "ledger") {
        setPageSizeInchesProps(17, 11);
    }

    var currentModal = null;
    if (showPagePropsModal) {
        currentModal = (
            <PagePropertiesModal
                setShowModal={(s) => {
                    if (!s) {
                        pageWidthModalProps = pageWidthMM;
                        pageHeightModalProps = pageHeightMM;
                        if (pageSizePagePropsModel == "custom") {
                            setPageWidthCustom(pageWidthMM);
                            setPageHeightCustom(pageHeightMM);
                        }
                        setPageSizePagePropsModel(pageSize);
                    }

                    setShowPagePropsModal(s);
                }}
                onChnage={(value, width, height) => {
                    setPageWidthCustom(width);
                    setPageHeightCustom(height);
                    setPageSizePagePropsModel(value);
                }}
                onApply={() => {
                    pageSize = pageSizePagePropsModel;
                    if (pageSize == "custom") {
                        setPageSizeMM(pageWidthCustom, pageHeightCustom);
                    } else {
                        setPageSizeMM(
                            pageWidthModalProps,
                            pageHeightModalProps
                        );
                    }

                    setShowPagePropsModal(false);

                    if (moduleIsCreated) {
                        module.onUpdatePageSize(
                            module.stringToNewUTF8(pageSize),
                            parseFloat(pageWidthMM),
                            parseFloat(pageHeightMM),
                            parseFloat(pageWidthTenths),
                            parseFloat(pageHeightTenths)
                        );
                    }
                }}
                currentPageSizeValue={pageSizePagePropsModel}
                currentPageWidth={
                    pageSizePagePropsModel == "custom"
                        ? pageWidthCustom
                        : pageWidthModalProps
                }
                currentPageHeight={
                    pageSizePagePropsModel == "custom"
                        ? pageHeightCustom
                        : pageHeightModalProps
                }
            ></PagePropertiesModal>
        );
    }

    return (
        <>
            <div className="flex flex-col h-screen">
                <Header></Header>

                <div className="fixed top-0 left-1/4 w-1/2 h-full pt-16 pb-20">
                    <div ref={canvasDiv} className="border-2 w-full h-full">
                        <canvas
                            ref={canvasRef}
                            className="w-full h-full"
                            id="canvas"
                            width="776"
                            height="600"
                        ></canvas>
                    </div>

                    <canvas
                        ref={pdfCanvasRef}
                        className="border-2 hidden"
                        id="pdf_canvas"
                        width={pageWidth}
                        height={pageHeight}
                    ></canvas>
                </div>

                <ComponentsSidebar
                    onPagePropsButtonClicked={() => {
                        setShowPagePropsModal(true);
                    }}
                />

                <ButtonTray />

                <Sidebar
                    properties={editableProperties}
                    onPropertiesChange={(newProperties) => {
                        setEditableProperties(newProperties);

                        if (moduleIsCreated) {
                            var ptr = module.stringToNewUTF8(
                                JSON.stringify(newProperties)
                            );
                            module.onPropertiesUpdated(ptr);
                        }
                    }}
                />

                {currentModal}
            </div>
        </>
    );
}

function PagePropertiesModal({
    setShowModal,
    onChnage = (value, width, height) => {},
    onApply = () => {},
    currentPageSizeValue = "a4",
    currentPageWidth = 0.0,
    currentPageHeight = 0.0,
}) {
    var customizeSizeDisabled = !(currentPageSizeValue == "custom");

    return (
        <Modal
            onClickOutside={() => {
                setShowModal(false);
            }}
        >
            <div className="flex justify-end">
                <TextButton
                    onClick={() => {
                        setShowModal(false);
                    }}
                >
                    X
                </TextButton>
            </div>

            <div>
                <label for="pageSize">Page Size: </label>
                <select
                    name="pageSize"
                    id="pageSize"
                    onChange={(e) => {
                        console.log(e.target.value);
                        var width = currentPageWidth;
                        var height = currentPageHeight;

                        onChnage(e.target.value, width, height);
                    }}
                    value={currentPageSizeValue}
                >
                    <option value="a2">A2</option>
                    <option value="a3">A3</option>
                    <option value="a4">A4</option>
                    <option value="a5">A5</option>
                    <option value="b4">B4</option>
                    <option value="b5">B5</option>
                    <option value="letter">Letter</option>
                    <option value="legal">Legal</option>
                    <option value="tabloid">Tabloid</option>
                    <option value="ledger">Ledger</option>
                    <option value="custom">Custom</option>
                </select>

                <NumberField
                    label="Width (mm)"
                    value={currentPageWidth}
                    disabled={customizeSizeDisabled}
                    onChange={(e) => {
                        onChnage(
                            currentPageSizeValue,
                            e.target.value,
                            currentPageHeight
                        );
                    }}
                ></NumberField>
                <NumberField
                    label="Height (mm)"
                    value={currentPageHeight}
                    disabled={customizeSizeDisabled}
                    onChange={(e, k1, k2) => {
                        onChnage(
                            currentPageSizeValue,
                            currentPageWidth,
                            e.target.value
                        );
                    }}
                ></NumberField>
            </div>

            <div className="flex justify-end">
                <TextButton onClick={onApply}>Apply</TextButton>

                <TextButton
                    onClick={() => {
                        setShowModal(false);
                    }}
                >
                    Cancel
                </TextButton>
            </div>
        </Modal>
    );
}

function ButtonTray() {
    return (
        <div className="fixed bottom-0 w-3/4 flex border-t-2 bg-slate-50">
            <TrayToggleButton
                icon1="/static/icons/pause_icon.svg"
                icon2="/static/icons/play_icon.svg"
                alt="play button"
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
            />

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onButtonEvent(
                            ButtonType.DownloadPDF.value,
                            ButtonEventType.Pressed.value
                        );
                    }
                }}
            >
                <img src="/static/icons/printer_icon.svg" alt="download pdf" />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    console.log("Get/Load Song button pressed!!!");

                    /*fetch("/song/get?key=ABC123&method=all")
                            .then((response) => response.json())
                            .then((data) => {
                                console.log(data);
                            });*/

                    fetch(
                        "/song/get?key=ABC123&method=get_file&id=38&file_index=0"
                        //"/song/get?key=ABC123&method=get_file&id=13&file_index=0"
                    )
                        .then((response) => response.text())
                        .then((data) => {
                            if (moduleIsCreated) {
                                var ptr = module.stringToNewUTF8(data);
                                var fileType =
                                    module.stringToNewUTF8("harmonyxml");
                                var result = module.loadSong(ptr, fileType);
                                console.log("Loaded song: " + result);
                            }
                        });
                }}
            >
                <img src="/static/icons/plus.svg" alt="get song" />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    console.log("Export button pressed!!!");

                    if (moduleIsCreated) {
                        module.onButtonEvent(
                            ButtonType.Export.value,
                            ButtonEventType.Pressed.value
                        );
                    }
                }}
            >
                Export Song
            </TrayButton>
        </div>
    );
}

function ComponentsSidebar({ onPagePropsButtonClicked }) {
    return (
        <>
            <div className="fixed top-0 left-0 w-1/4 h-full pt-16">
                <div className="border-l-2 bg-slate-50 w-full h-full">
                    <ul className="m-2 space-y-2">
                        <SidebarHeading>Components</SidebarHeading>

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(0);
                                }
                            }}
                        >
                            Add Lyric
                        </TextButton>

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(1);
                                }
                            }}
                        >
                            Add Chord
                        </TextButton>

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(2);
                                }
                            }}
                        >
                            Insert Measure Before
                        </TextButton>

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(3);
                                }
                            }}
                        >
                            Insert Measure After
                        </TextButton>

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(4);
                                }
                            }}
                        >
                            Append Measure
                        </TextButton>

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(5);
                                }
                            }}
                        >
                            Add Rehearsal/Section Marking
                        </TextButton>

                        <TextButton onClick={onPagePropsButtonClicked}>
                            Page Properties
                        </TextButton>
                    </ul>
                </div>
            </div>
        </>
    );
}

function Modal({ children, onClickOutside }) {
    const outsideDivRef = useRef(null);

    return (
        <div
            ref={outsideDivRef}
            className="fixed top-0 left-0 z-50 w-full h-full flex justify-center items-center bg-black bg-opacity-35"
            onClick={(e) => {
                // test for clicking of just this div (not its children)
                if (e.target == e.currentTarget) {
                    onClickOutside();
                }
            }}
        >
            <div className="bg-white rounded-md shadow-lg max-w-[80%] max-h-[80%] overflow-auto p-4">
                {children}
            </div>
        </div>
    );
}

function Sidebar({ properties, onPropertiesChange }) {
    var items = [];
    for (var heading in properties) {
        var value = properties[heading];

        if (typeof value == "object") {
            items.push(<SidebarHeading>{heading}</SidebarHeading>);

            for (var propertyKey in value) {
                var property = value[propertyKey];

                if (property.type == "bool") {
                    items.push(
                        <li>
                            <CheckboxField
                                label={property.name}
                                value={property.value}
                                k1={heading}
                                k2={propertyKey}
                                onChange={async (event, k1, k2) => {
                                    var newProperties = JSON.parse(
                                        JSON.stringify(properties)
                                    ); // deep copy

                                    newProperties[k1][k2].value =
                                        event.target.checked;

                                    onPropertiesChange(newProperties);
                                }}
                            ></CheckboxField>
                        </li>
                    );
                } else if (property.type == "text") {
                    items.push(
                        <li>
                            <TextField
                                label={property.name}
                                value={property.value}
                                k1={heading}
                                k2={propertyKey}
                                onChange={async (event, k1, k2) => {
                                    var newProperties = JSON.parse(
                                        JSON.stringify(properties)
                                    ); // deep copy
                                    newProperties[k1][k2].value =
                                        event.target.value;
                                    onPropertiesChange(newProperties);
                                }}
                            ></TextField>
                        </li>
                    );
                } else if (property.type == "float") {
                    items.push(
                        <li>
                            <NumberField
                                label={property.name}
                                value={property.value}
                                k1={heading}
                                k2={propertyKey}
                                onChange={async (event, k1, k2) => {
                                    var newProperties = JSON.parse(
                                        JSON.stringify(properties)
                                    ); // deep copy
                                    newProperties[k1][k2].value = parseFloat(
                                        event.target.value
                                    );
                                    onPropertiesChange(newProperties);
                                }}
                            ></NumberField>
                        </li>
                    );
                } else if (property.type == "int") {
                    items.push(
                        <li>
                            <NumberField
                                label={property.name}
                                value={property.value}
                                k1={heading}
                                k2={propertyKey}
                                onChange={async (event, k1, k2) => {
                                    var newProperties = JSON.parse(
                                        JSON.stringify(properties)
                                    ); // deep copy
                                    newProperties[k1][k2].value = parseInt(
                                        event.target.value
                                    );
                                    onPropertiesChange(newProperties);
                                }}
                            ></NumberField>
                        </li>
                    );
                }
            }
        }
    }

    return (
        <>
            <div className="fixed top-0 right-0 w-1/4 h-full pt-16">
                <div className="border-l-2 bg-slate-50 w-full h-full overflow-auto">
                    <ul className="m-2 space-y-2">
                        <SidebarHeading>Properties</SidebarHeading>

                        {items}
                    </ul>
                </div>
            </div>
        </>
    );
}

function SidebarHeading({ children }) {
    return (
        <>
            <p className="text-center font-medium">{children}</p>
            <hr />
        </>
    );
}

function TextField({
    label,
    value = "",
    k1 = "",
    k2 = "",
    onChange = (e, k) => {},
    className = "",
}) {
    className +=
        " h-7 rounded-sm border-2 hover:border-blue-200 active:border-blue-500";

    return (
        <label className="m-2">
            {label}
            <input
                className={className}
                value={value}
                onChange={(e) => {
                    onChange(e, k1, k2);
                }}
                type="text"
            />
        </label>
    );
}

function NumberField({
    label,
    value = 0,
    k1 = "",
    k2 = "",
    onChange = () => {},
    className = "",
    disabled = false,
}) {
    className +=
        " h-7 rounded-sm border-2 hover:border-blue-200 active:border-blue-500";

    return (
        <label className="m-2">
            {label}
            <input
                className={className}
                value={value}
                onChange={(e) => {
                    onChange(e, k1, k2);
                }}
                type="number"
                disabled={disabled}
            />
        </label>
    );
}

function CheckboxField({
    label,
    value = false,
    k1 = "",
    k2 = "",
    onChange = () => {},
    className = "",
}) {
    className +=
        " h-7 rounded-sm border-2 hover:border-blue-200 active:border-blue-500";

    return (
        <label className="m-2">
            {label}
            <input
                className={className}
                checked={value}
                onChange={(e) => {
                    onChange(e, k1, k2);
                }}
                type="checkbox"
            />
        </label>
    );
}

function Button({ children, onClick = () => {}, className = "" }) {
    className += " bg-white text-black";

    return (
        <button className={className} onClick={onClick}>
            {children}
        </button>
    );
}

function TextButton({ children, onClick = () => {} }) {
    return (
        <button
            className="rounded-sm bg-emerald-500 text-black p-4 m-2 hover:border-2 hover:border-emerald-600 active:border-emerald-800"
            onClick={onClick}
        >
            {children}
        </button>
    );
}

function TrayButton({ children, onClick = () => {} }) {
    return (
        <Button
            className="w-16 h-16 rounded-sm m-2 hover:border-2 hover:border-slate-200 active:border-blue-300"
            onClick={onClick}
        >
            <center>{children}</center>
        </Button>
    );
}

function TrayToggleButton({ icon1, icon2, alt, onClick = () => {} }) {
    const [toggled, setToggled] = useState(false);

    var icon;
    if (toggled) icon = icon1;
    else icon = icon2;

    return (
        <Button
            className="w-16 h-16 rounded-sm m-2 hover:border-2 hover:border-slate-200 active:border-blue-300"
            onClick={() => {
                setToggled(!toggled);
                onClick();
            }}
        >
            <center>
                <img src={icon} alt={alt}></img>
            </center>
        </Button>
    );
}
