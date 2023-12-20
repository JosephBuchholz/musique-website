
export function initWasm() { 
    console.log("Hello from init");

    var Module = {
            onRuntimeInitialized: function() {
                console.log("result: " + Module.addNums(1, 2));
            }
        };
}