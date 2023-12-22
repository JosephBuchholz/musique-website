import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App.jsx'
import Module from './cpp.js';
import "./index.css"

/*addToLibrary({
  my_js: function() {
    alert('hi');
  },
});*/

function jsFunction(someNumber) {
  console.log("this is the js funciton!!: " + someNumber);
  alert('this is the js function');
}


console.log("Hello outside");

Module()
  .then(module => {
    console.log('Module created!');
    let a = module.addNums(-1, 2);
    console.log(a);

    console.log("adding js function (from js)");
    var newFunctPtr = module.addFunction(jsFunction, "vi");
    console.log("calling add js function (from js)");
    module.addJsFunction(newFunctPtr);

    console.log("calling js function (from js)");
    module.callJsFunction();
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
