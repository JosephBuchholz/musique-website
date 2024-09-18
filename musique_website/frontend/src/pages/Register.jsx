/**
 * Definition for the register page.
 */

import Header from "../components/Header";
import { useNavigate } from "react-router-dom";
import { getUser } from "../userauth";
import { useEffect, useState } from "react";
import { ErrorMessage, InputField } from "../components/FormComponents";

/**
 * The register page, so that the user can create an account.
 *
 * @returns The JSX for this component.
 */
export default function RegisterPage() {
    const [username, setUsername] = useState("");
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [confirmPassword, setConfirmPassword] = useState("");
    const [loading, setLoading] = useState(false);
    const navigate = useNavigate();
    const [user, setUser] = useState(null);
    const [errors, setErrors] = useState(null);

    useEffect(() => {
        getUser().then((user) => {
            setUser(user);
        });
    }, []);

    async function handleSubmit(e) {
        setLoading(true);
        e.preventDefault();

        if (username.length > 20) {
            setErrors({
                username: ["Username must be at most 20 characters long."],
            });
        } else if (password.length < 6) {
            setErrors({
                password: ["Password must be at least 6 characters long."],
            });
        } else if (password != confirmPassword) {
            setErrors({ error: ["Passwords don't match"] });
        } else {
            try {
                var successful = true;

                await fetch("/user/register/", {
                    method: "POST",
                    headers: {
                        Accept: "application/json",
                        "Content-Type": "application/json",
                    },
                    body: JSON.stringify({
                        username: username,
                        email: email,
                        password: password,
                    }),
                })
                    .then((response) => {
                        if (response.status === 400) {
                            response.text().then((text) => {
                                setErrors(JSON.parse(text));
                            });
                            successful = false;
                        }
                    })
                    .catch((error) => {
                        alert(error);
                        successful = false;
                    });

                if (successful) {
                    navigate("/login");
                }
            } catch (error) {
                alert(error);
            } finally {
                setLoading(false);
            }
        }

        setLoading(false);
    }

    const usernameErrors = [];
    const emailErrors = [];
    const passwordErrors = [];
    const confirmPasswordErrors = [];
    const generalErrors = [];

    if (errors != null) {
        if (errors["username"] !== undefined) {
            errors["username"].forEach((e) => {
                usernameErrors.push(<ErrorMessage>{e}</ErrorMessage>);
            });
        }

        if (errors["email"] !== undefined) {
            errors["email"].forEach((e) => {
                emailErrors.push(<ErrorMessage>{e}</ErrorMessage>);
            });
        }

        if (errors["password"] !== undefined) {
            errors["password"].forEach((e) => {
                passwordErrors.push(<ErrorMessage>{e}</ErrorMessage>);
            });
        }

        if (errors["confirm_password"] !== undefined) {
            errors["confirm_password"].forEach((e) => {
                confirmPasswordErrors.push(<ErrorMessage>{e}</ErrorMessage>);
            });
        }

        if (errors["error"] !== undefined) {
            errors["error"].forEach((e) => {
                generalErrors.push(<ErrorMessage>{e}</ErrorMessage>);
            });
        }
    }

    return (
        <>
            <div className="flex flex-col h-screen">
                <Header user={user}></Header>

                <div className="flex flex-col self-center w-3/4 bg-white mt-10">
                    <form onSubmit={handleSubmit} className="flex flex-col">
                        <h1 className="text-3xl font-bold text-blue-950 self-center mt-3 mb-5">
                            Register
                        </h1>

                        <InputField
                            error={usernameErrors.length !== 0}
                            name="username"
                            type="text"
                            placeholder="Username"
                            onChange={(e) => {
                                setUsername(e.target.value);
                            }}
                        ></InputField>
                        {usernameErrors}

                        <InputField
                            error={emailErrors.length !== 0}
                            name="email"
                            type="text"
                            placeholder="Email"
                            onChange={(e) => {
                                setEmail(e.target.value);
                            }}
                        ></InputField>
                        {emailErrors}

                        <InputField
                            error={passwordErrors.length !== 0}
                            name="password"
                            type="password"
                            placeholder="Password"
                            onChange={(e) => {
                                setPassword(e.target.value);
                            }}
                        ></InputField>
                        {passwordErrors}

                        <InputField
                            error={confirmPasswordErrors.length !== 0}
                            name="confirm_password"
                            type="password"
                            placeholder="Confirm Password"
                            onChange={(e) => {
                                setConfirmPassword(e.target.value);
                            }}
                        ></InputField>
                        {confirmPasswordErrors}
                        {generalErrors}

                        <div className="self-center">
                            <button
                                className="mb-5 mt-5 bg-slate-300 p-3 rounded-full font-medium hover:bg-slate-200"
                                type="submit"
                                disabled={loading}
                            >
                                Register
                            </button>
                        </div>

                        <p className="text-sm text-center mb-5">
                            Already have an account?{" "}
                            <a
                                className="text-blue-500 font-medium cursor-pointer hover:text-blue-400"
                                onClick={() => {
                                    navigate("/login");
                                }}
                            >
                                Login
                            </a>
                        </p>
                    </form>
                </div>
            </div>
        </>
    );
}
