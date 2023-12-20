import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App.jsx'
import Module from './cpp.js';
import "./index.css"

console.log("Hello outside");

Module()
  .then(module => {
    console.log('Module created!');
    let a = module.addNums(1, 2);
    console.log(a);
});

/*Module.onRuntimeInitialized = _ => {
  console.log("result: " + Module.addNums(1, 2));
};*/


/*var Module = {
    onRuntimeInitialized: function() {
        console.log("result: " + Module.addNums(1, 2));
    }
};*/

ReactDOM.createRoot(document.getElementById('app')).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
)
