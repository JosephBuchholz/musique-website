import React from "react"
import { render } from "react-dom"
import App from "./App.tsx"
import "./index.css"

const container = document.getElementById("app");
render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
  container)