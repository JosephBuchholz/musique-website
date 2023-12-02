import React from "react"
import { render } from "react-dom"
import App from "./components/App.tsx"

const container = document.getElementById("app");
render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
  container)