/**
 * Contains the ButtonTray component.
 */

import { TrayButton, TrayToggleButton } from "./TrayButtons";

/**
 * A tray containing buttons for certain actions
 * like play and download.
 *
 * @returns The JSX for this component.
 */
export default function ButtonTray() {
    return (
        <div className="flex border-t-2 bg-slate-50">
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

                    fetch(
                        "/song/get?key=ABC123&method=get_file&id=38&file_index=0"
                    )
                        .then((response) => response.text())
                        .then((data) => {
                            console.log("DATA: " + data);
                            if (moduleIsCreated) {
                                loadSong(data, "harmonyxml");
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
                <img
                    src="/static/icons/download_icon.svg"
                    alt="download song"
                />
            </TrayButton>
        </div>
    );
}
