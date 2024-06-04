import { BrowserRouter, Routes, Route } from "react-router-dom";
import HomePage from "./pages/HomePage";
import EditorPage from "./pages/EditorPage";
import LoginPage from "./pages/Login";
import RegisterPage from "./pages/Register";
import NotFoundPage from "./pages/NotFound";
import LogoutPage from "./pages/Logout";

export default function App() {
    return (
        <>
            <BrowserRouter>
                <Routes>
                    <Route index element={<HomePage></HomePage>}></Route>
                    <Route path="/home" element={<HomePage></HomePage>}></Route>
                    <Route
                        path="/editor"
                        element={<EditorPage></EditorPage>}
                    ></Route>
                    <Route
                        path="/login"
                        element={<LoginPage></LoginPage>}
                    ></Route>
                    <Route
                        path="/logout"
                        element={<LogoutPage></LogoutPage>}
                    ></Route>
                    <Route
                        path="/register"
                        element={<RegisterPage></RegisterPage>}
                    ></Route>
                    <Route
                        path="*"
                        element={<NotFoundPage></NotFoundPage>}
                    ></Route>
                </Routes>
            </BrowserRouter>
        </>
    );
}
