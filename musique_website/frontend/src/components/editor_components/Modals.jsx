/**
 * Contains Modal and PagePropertiesModal.
 */

import { TextButton, NumberField } from "./EditorButtons";
import { useRef } from "react";

/**
 * Modal that allows user to edit page properties.
 *
 * @param setShowModel Callback that notifies the caller when the modal has been canceled (will pass false on close)
 * @param onChange Callback that notifies the caller when the page properties have been changed by the user.
 * @param onApply Callback that notifies the caller when the user has accepted the values.
 * @param currentPageSizeValue The current page type (i.e. A4, Letter, ect.)
 * @param currentPageWidth The current page width in millimeters.
 * @param currentPageHeight The current page height in millimeters.
 * @returns The JSX for the modal.
 */
export function PagePropertiesModal({
    setShowModal,
    onChnage = (value, width, height) => {},
    onApply = () => {},
    currentPageSizeValue = "a4",
    currentPageWidth = 0.0,
    currentPageHeight = 0.0,
}) {
    var customizeSizeDisabled = !(currentPageSizeValue == "custom");

    return (
        <Modal>
            <div className="flex justify-end">
                <TextButton
                    onClick={() => {
                        setShowModal(false);
                    }}
                >
                    <img src="/static/icons/x_icon.svg" alt="close" />
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

/**
 * Basic modal class.
 *
 * @param children Content of the modal.
 * @param onClickOutside Callback, called when user clicks outside the modal.
 * @returns The JSX for this modal.
 */
export function Modal({ children, onClickOutside = () => {} }) {
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
