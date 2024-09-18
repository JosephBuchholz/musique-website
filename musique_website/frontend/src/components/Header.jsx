/**
 * Defines the Header component.
 */

import { useState } from "react";
import { useEffect } from "react";
import { Link } from "react-router-dom";
import { getUser, isAuthenticated } from "../userauth";

/**
 * Header for display on all pages on the website.
 */
export default function Header({ user = null }) {
    return <NavigationBar user={user}></NavigationBar>;
}

/**
 * Basic navigation bar.
 */
function NavigationBar({ user }) {
    return (
        <>
            <div className="flex bg-gray-100 top-0 w-screen h-16 justify-between z-50">
                <div className="flex justify-center items-center text-center">
                    <Link
                        to="/"
                        className="text-3xl font-bold text-black m-6 p-1"
                    >
                        Harmonically
                    </Link>
                </div>

                <ul className="flex justify-center items-center mr-8">
                    <NavLink to="/songlist">Songs</NavLink>
                    {user === null ? (
                        <>
                            <NavLink to="/login">Login</NavLink>
                            <NavLink to="/register">Register</NavLink>
                        </>
                    ) : (
                        <>
                            <NavLink to="/logout">Logout</NavLink>
                            <h1>{user.username}</h1>
                        </>
                    )}
                </ul>
            </div>
        </>
    );
}

/**
 * A navigation link.
 */
function NavLink({ to, children }) {
    return (
        <>
            <li className="text-center m-6 p-1">
                <Link
                    to={to}
                    className="hover:text-blue-600 transition-all font-semibold"
                >
                    {children}
                </Link>
            </li>
        </>
    );
}
