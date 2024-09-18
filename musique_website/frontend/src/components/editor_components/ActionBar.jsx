/**
 * Contains ActionBar component.
 */

import { TrayButton } from "./TrayButtons";

/**
 * An action bar with buttons to allow the user to do things like:
 * change note duration and turn a note into a rest.
 *
 * @returns The JSX for this component.
 */
export default function ActionBar() {
    return (
        <div className="w-full flex border-b-2 bg-slate-50">
            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(10);
                    }
                }}
            >
                <img
                    src="/static/icons/whole_note_icon.svg"
                    alt="change to whole note"
                />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(11);
                    }
                }}
            >
                <img
                    src="/static/icons/half_note_icon.svg"
                    alt="change to half note"
                />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(12);
                    }
                }}
            >
                <img
                    src="/static/icons/quarter_note_icon.svg"
                    alt="change to quarter note"
                />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(13);
                    }
                }}
            >
                Eighth
                {/*<img
                    src="/static/icons/quarter_note_icon.svg"
                    alt="change to eighth note"
            />*/}
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(14);
                    }
                }}
            >
                16th
                {/*<img
                    src="/static/icons/quarter_note_icon.svg"
                    alt="change to sixteenth note"
            />*/}
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(15);
                    }
                }}
            >
                Dot
                {/*<img
                    src="/static/icons/quarter_note_icon.svg"
                    alt="make dotted note"
            />*/}
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(16);
                    }
                }}
            >
                <img src="/static/icons/rest_icon.svg" alt="make rest" />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(17);
                    }
                }}
            >
                <img src="/static/icons/undo_icon.svg" alt="undo" />
            </TrayButton>

            <TrayButton
                onClick={async () => {
                    if (moduleIsCreated) {
                        module.onNewElement(18);
                    }
                }}
            >
                <img src="/static/icons/redo_icon.svg" alt="redo" />
            </TrayButton>
        </div>
    );
}
