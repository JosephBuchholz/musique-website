import React from "react"
import { render } from "react-dom"
import App from "./App.tsx"
import "./index.css"
import { initWasm } from "./wasm.js";

console.log("Hello");

initWasm();

const container = document.getElementById("app");
render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
  container);