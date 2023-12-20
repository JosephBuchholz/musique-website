
/*var exports: WebAssembly.Exports;
export var addNums: CallableFunction;

function logMsg() {
    console.log("Hello");
}

function initWebAssembly() {

    const importObject = {
        env: {
            logMsg: logMsg,
        }
    }

    WebAssembly.instantiateStreaming(
        fetch("/static/wasm/cpp.wasm"), importObject
    ).then((results) => {
        exports = results.instance.exports;
        addNums = exports.addNums as any;
    });
}

initWebAssembly();*/

/*import Module from "./cpp";

Module.onRuntimeInitialized = _ => {
    console.log(Module.addNums(1, 2))
};*/