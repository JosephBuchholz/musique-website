import { useEffect, useState } from "react";
import Header from "../components/Header";
import { useNavigate } from "react-router-dom";
import Cookies from "js-cookie";
import { getUser } from "../userauth";
import { ErrorMessage, InputField } from "../components/FormComponents";

export default function LoginPage() {
    const [username, setUsername] = useState("");
    const [password, setPassword] = useState("");
    const [loading, setLoading] = useState(false);
    const navigate = useNavigate();
    const [user, setUser] = useState(null);
    const [errors, setErrors] = useState(null);

    useEffect(() => {
        getUser().then((user) => {
            setUser(user);
        });
    }, []);

    const csrftoken = Cookies.get("csrftoken");

    async function handleSubmit(e) {
        setLoading(true);
        e.preventDefault();

        try {
            var successful = true;

            await fetch("/user/login/", {
                method: "POST",
                headers: {
                    Accept: "application/json",
                    "Content-Type": "application/json",
                    "X-CSRFToken": csrftoken,
                    mode: "same-orgin", // Do not send CSRF token ot another domain
                },
                body: JSON.stringify({
                    username: username,
                    password: password,
                }),
            })
                .then((response) => {
                    if (response.status === 403) {
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
                navigate("/home");
            }
        } catch (error) {
            alert(error);
        } finally {
            setLoading(false);
        }

        setLoading(false);
    }

    const usernameErrors = [];
    const passwordErrors = [];
    const generalErrors = [];

    if (errors != null) {
        if (errors["username"] !== undefined) {
            errors["username"].forEach((e) => {
                usernameErrors.push(<ErrorMessage>{e}</ErrorMessage>);
            });
        }

        if (errors["password"] !== undefined) {
            errors["password"].forEach((e) => {
                passwordErrors.push(<ErrorMessage>{e}</ErrorMessage>);
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
                            Login
                        </h1>

                        <InputField
                            error={usernameErrors.length !== 0}
                            name="username"
                            type="text"
                            placeholder="Username/Email"
                            onChange={(e) => {
                                setUsername(e.target.value);
                            }}
                        ></InputField>
                        {usernameErrors}

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
                        {generalErrors}

                        <div className="self-center">
                            <button
                                className="mb-5 mt-5 bg-slate-300 p-3 rounded-full font-medium hover:bg-slate-200"
                                type="submit"
                                disabled={loading}
                            >
                                Login
                            </button>
                        </div>

                        <p className="text-sm text-center mb-5">
                            Don't have an account?{" "}
                            <a
                                className="text-blue-500 font-medium cursor-pointer hover:text-blue-400"
                                onClick={() => {
                                    navigate("/register");
                                }}
                            >
                                Register
                            </a>
                        </p>
                    </form>
                </div>
            </div>
        </>
    );
}
