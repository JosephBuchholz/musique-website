/**
 * Definition of the not found page.
 */

/**
 * The not found page. Displayed when
 * the user tries to access a page
 * that does not exist.
 *
 * @returns The JSX for this component.
 */
export default function NotFoundPage() {
    return (
        <>
            <div className="flex flex-col h-screen">
                <h1 className="text-3xl font-bold text-blue-950">
                    404 Not Found
                </h1>
            </div>
        </>
    );
}
