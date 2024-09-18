/**
 * Contains some components specifically for the ButtonTray component.
 */

import { useState } from "react";
import { Button } from "./EditorButtons";

/**
 * Basic button component specifically for the ButtonTray component.
 */
export function TrayButton({ children, onClick = () => {} }) {
    return (
        <Button
            className="w-10 h-10 rounded-sm m-2 hover:border-2 hover:border-slate-200 active:border-blue-300"
            onClick={onClick}
        >
            <center>{children}</center>
        </Button>
    );
}

/**
 * Basic toggle button component specifically for the ButtonTray component.
 */
export function TrayToggleButton({ icon1, icon2, alt, onClick = () => {} }) {
    const [toggled, setToggled] = useState(false);

    var icon;
    if (toggled) icon = icon1;
    else icon = icon2;

    return (
        <Button
            className="w-10 h-10 rounded-sm m-2 hover:border-2 hover:border-slate-200 active:border-blue-300"
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
