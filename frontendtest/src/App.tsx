import { BrowserRouter, Routes, Route } from "react-router-dom"
import HomePage from "./pages/HomePage"
import EditorPage from "./pages/EditorPage"

export default function App() {

  return (<>
  <BrowserRouter>
    <Routes>
      <Route index element={<HomePage></HomePage>}></Route>
      <Route path="/home" element={<HomePage></HomePage>}></Route>
      <Route path="/editor" element={<EditorPage></EditorPage>}></Route>
    </Routes>
  </BrowserRouter>
  </>)
}