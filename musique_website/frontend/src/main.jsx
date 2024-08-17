import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App.jsx";
import "./index.css";

/*addToLibrary({
  my_js: function() {
    alert('hi');
  },
});*/

/*Module.onRuntimeInitialized = _ => {
  console.log("result: " + Module.addNums(1, 2));
};*/

/*var Module = {
    onRuntimeInitialized: function() {
        console.log("result: " + Module.addNums(1, 2));
    }
};*/

ReactDOM.createRoot(document.getElementById("app")).render(
    <React.StrictMode>
        <App />
    </React.StrictMode>
);
