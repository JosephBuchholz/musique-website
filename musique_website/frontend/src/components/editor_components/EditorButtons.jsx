/**
 * Contains some basic components for the editor.
 */

/**
 * Basic text field component.
 */
export function TextField({
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

/**
 * Basic number field component.
 */
export function NumberField({
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

/**
 * Basic checkbox component.
 */
export function CheckboxField({
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

/**
 * Basic button component.
 */
export function Button({ children, onClick = () => {}, className = "" }) {
    className += " bg-white text-black";

    return (
        <button className={className} onClick={onClick}>
            {children}
        </button>
    );
}

/**
 * Basic text button component.
 */
export function TextButton({ children, onClick = () => {} }) {
    return (
        <button
            className="rounded-sm bg-emerald-500 text-black p-4 m-2 hover:border-2 hover:border-emerald-600 active:border-emerald-800"
            onClick={onClick}
        >
            {children}
        </button>
    );
}
