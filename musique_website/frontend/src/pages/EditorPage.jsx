/**
 * Definition of the page for the editor.
 */

import { useEffect, useRef, useState } from "react";
import Header from "../components/Header";
import Module from "../cpp.js";
import * as Tone from "tone";
import handleMidiMessage from "../audio/midi.js";
import * as Renderer from "../rendering/canvas_renderer.js";
import { jsPDF } from "jspdf";
import PDFRenderer from "../rendering/pdf_renderer.js";
import { getUser } from "../userauth";
import { useParams } from "react-router-dom";
import {
    MMToTenths,
    getASeriesPageWidthMM,
    getASeriesPageHeightMM,
    getBSeriesPageWidthMM,
    getBSeriesPageHeightMM,
} from "../converters/converters.js";
import ActionBar from "../components/editor_components/ActionBar.jsx";
import {
    ComponentsSidebar,
    Sidebar,
} from "../components/editor_components/SideBars.jsx";
import ButtonTray from "../components/editor_components/ButtonTray.jsx";
import { PagePropertiesModal } from "../components/editor_components/Modals.jsx";
import {
    ButtonType,
    ButtonEventType,
    PointerEventType,
    KeyboardEventType,
} from "../events/event_types.js";

function writeMidi(bytes, size) {
    var byteArray = new Uint8Array(size);

    for (let i = 0; i < size; i++) {
        var byte = module.getValue(bytes + i, "i8");
        byteArray[i] = byte;
    }

    handleMidiMessage(byteArray);
}

var module;
var moduleIsCreated = false;

var onModuleCreatedCallback = () => {
    console.log("nothing!!!");
};

async function createModule() {
    var createdNew = false;

    if (!moduleIsCreated) {
        // create module
        await Module().then((mod) => {
            console.log("Creating Module!");

            module = mod;
            moduleIsCreated = true;

            console.log("Module created!");

            var writeMidiFP = module.addFunction(writeMidi, "vii");

            module.addAudioCallbacksToCpp(writeMidiFP);

            createdNew = true;
        });
    }

    // else already exits
    return new Promise(function (resolve, reject) {
        resolve(createdNew);
    });
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

function loadSong(data, filetype, mod) {
    var ptr = mod.stringToNewUTF8(data);
    var fileType = mod.stringToNewUTF8(filetype);
    var result = mod.loadSong(ptr, fileType);
}

/**
 * The page for the editor.
 *
 * @returns The JSX for this component.
 */
export default function EditorPage() {
    const canvasRef = useRef(null);
    const pdfCanvasRef = useRef(null);
    const canvasDiv = useRef(null);
    const propertiesList = useRef(null);

    const [editableProperties, setEditableProperties] = useState("");
    const [user, setUser] = useState(null);

    const { id } = useParams();

    useEffect(() => {
        getUser().then((user) => {
            setUser(user);
        });
    }, []);

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

        createModule().then((createdNew) => {
            if (!createdNew) return;

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

            onModuleCreatedCallback(module);
        });

        // --- Event Listeners ---

        function keydownEventListener(event) {
            if (moduleIsCreated) {
                module.onKeyboardEvent(
                    KeyboardEventType.Down.value,
                    module.stringToNewUTF8(event.key)
                );
            }
        }

        function keyupEventListener(event) {
            if (moduleIsCreated) {
                module.onKeyboardEvent(
                    KeyboardEventType.Up.value,
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
        document.addEventListener("keyup", keyupEventListener, true);
        canvas.addEventListener("pointerdown", pointerDownEventListener);
        document.addEventListener("pointermove", pointerMoveEventListener);
        document.addEventListener("pointerup", pointerUpEventListener);

        return () => {
            document.removeEventListener("keydown", keydownEventListener, true);
            document.removeEventListener("keyup", keyupEventListener, true);
            canvas.removeEventListener("pointerdown", pointerDownEventListener);
            document.removeEventListener(
                "pointermove",
                pointerMoveEventListener
            );
            document.removeEventListener("pointerup", pointerUpEventListener);
        };
    });

    useEffect(() => {
        fetch(`/usersongs/getfile?id=${id}&file_index=${0}`)
            .then((response) => response.text())
            .then((data) => {
                console.log(
                    "Fetched song file -------------------------------------"
                );

                onModuleCreatedCallback = (mod) => {
                    console.log("MODULE CREATED AND GETTING DATA (callback)");
                    loadSong(data, "harmonyxml", mod);
                };

                if (moduleIsCreated) {
                    console.log(
                        "MODULE CREATED AND GETTING DATA (module already created)"
                    );
                    onModuleCreatedCallback(module);
                }
            });
    }, [id]);

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
            <div className="w-screen h-screen flex flex-col">
                <Header user={user}></Header>

                <ActionBar></ActionBar>

                <div className="h-full w-full flex flex-row overflow-auto">
                    <ComponentsSidebar
                        onPagePropsButtonClicked={() => {
                            setShowPagePropsModal(true);
                        }}
                    />

                    <div className="w-full">
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
                </div>

                <ButtonTray />

                {currentModal}
            </div>
        </>
    );
}
