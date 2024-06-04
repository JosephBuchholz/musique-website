import { useEffect, useState } from "react";
import Header from "../components/Header";
import { useNavigate } from "react-router-dom";
import Cookies from "js-cookie";
import { getUser } from "../userauth";

export default function LogoutPage() {
    const [loading, setLoading] = useState(true);
    const navigate = useNavigate();
    const [user, setUser] = useState(null);

    useEffect(() => {
        getUser().then((user) => {
            setUser(user);
        });
    }, []);

    const csrftoken = Cookies.get("csrftoken");

    useEffect(() => {
        try {
            var successful = true;

            fetch("/user/logout/", {
                method: "POST",
                headers: {
                    Accept: "application/json",
                    "Content-Type": "application/json",
                    "X-CSRFToken": csrftoken,
                    mode: "same-orgin", // Do not send CSRF token ot another domain
                },
            })
                .then((response) => {
                    if (response.status === 403) {
                        response.text().then((text) => {
                            alert(text);
                        });
                        successful = false;
                    } else {
                        getUser().then((user) => {
                            setUser(user);
                        });
                    }
                })
                .catch((error) => {
                    alert(error);
                    successful = false;
                });

            if (successful) {
                console.log("Success!");
            }
        } catch (error) {
            alert(error);
        } finally {
            setLoading(false);
        }

        setLoading(false);
    }, []);

    return (
        <>
            <div className="flex flex-col h-screen">
                <Header user={user}></Header>

                {loading ? (
                    <h1>Loading...</h1>
                ) : (
                    <h1 className="text-3xl font-bold text-blue-950">
                        You have been successfully logged out!
                    </h1>
                )}
            </div>
        </>
    );
}
