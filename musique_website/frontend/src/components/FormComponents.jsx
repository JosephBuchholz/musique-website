/**
 * Some basic components for login/signup forms.
 */

/**
 * Basic error message component.
 */
export function ErrorMessage({ children }) {
    return (
        <p className="ml-2 mr-2 mb-5 text-red-600 font-medium">{children}</p>
    );
}

/**
 * Basic input field.
 */
export function InputField({
    name,
    type,
    placeholder,
    onChange,
    error = false,
}) {
    var className = "border-slate-300 border-2 m-2";
    if (error) className = "border-red-500 border-2 m-2";

    return (
        <input
            className={className}
            name={name}
            type={type}
            placeholder={placeholder}
            onChange={onChange}
        ></input>
    );
}
