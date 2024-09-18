/**
 * Contains some side bar components for the editor.
 */

import {
    TextField,
    NumberField,
    CheckboxField,
    TextButton,
} from "./EditorButtons.jsx";

/**
 * Side bar for the editor that allows the user to add components.
 */
export function ComponentsSidebar({ onPagePropsButtonClicked }) {
    return (
        <>
            <div className="w-1/4 h-full">
                <div className="border-l-2 bg-slate-50 w-full h-full overflow-y-auto">
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

                        <TextButton
                            onClick={() => {
                                if (moduleIsCreated) {
                                    module.onNewElement(6);
                                }
                            }}
                        >
                            Add Time Signature
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

/**
 * Side containing properties for currently selected components.
 */
export function Sidebar({ properties, onPropertiesChange }) {
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
            <div className="h-full w-1/4">
                <div className="border-l-2 bg-slate-50 w-full h-full overflow-y-auto">
                    <ul className="m-2 space-y-2">
                        <SidebarHeading>Properties</SidebarHeading>

                        {items}
                    </ul>
                </div>
            </div>
        </>
    );
}

/**
 * Basic heading for sidebars.
 */
export function SidebarHeading({ children }) {
    return (
        <>
            <p className="text-center font-medium">{children}</p>
            <hr />
        </>
    );
}
