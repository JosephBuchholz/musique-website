/**
 * Definition for the home page.
 */

import Header from "../components/Header";
import { useEffect, useState } from "react";
import { getUser } from "../userauth";

/**
 * The home page.
 *
 * @returns The JSX for this component.
 */
export default function HomePage() {
    const [user, setUser] = useState(null);

    useEffect(() => {
        getUser().then((user) => {
            setUser(user);
        });
    }, []);

    return (
        <>
            <div className="flex flex-col h-screen">
                <Header user={user}></Header>

                <div className="flex-1 flex justify-center items-center">
                    <h1 className="text-3xl font-bold text-blue-950">Hello!</h1>
                </div>
            </div>
        </>
    );
}
