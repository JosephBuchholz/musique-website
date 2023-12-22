#include <iostream>

#include <emscripten.h>
#include <emscripten/bind.h>

#include "Debugging/Debug.h"


/*
extern "C" {
    //extern void logMsg();

    EMSCRIPTEN_KEEPALIVE
    int addNums(int a, int b)
    {
        //logMsg();
        return a + b;
    }
}
*/

/*extern "C" {
    extern void my_js();
}

EM_JS(void, call_alert, (), {
    alert('hello from js inside c++');
    throw 'all done';
});*/

/*EM_JS(void, callJs, (), {
    console.log("calling js funciton from callJs c++");
    jsFunction();
});*/

int addNums(int a, int b)
{
    LOGI("calling call js");
    //my_js();
    //callJs();
    return a * b;
}

void (*jsFunction)(int);

void addJsFunction(int functionPointer)
{
    LOGI("adding js function (from c++)");
    jsFunction = reinterpret_cast<void (*)(int)>(functionPointer);
}

void callJsFunction()
{
    LOGI("calling js function from c++");
    jsFunction(555);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("addNums", &addNums);
    emscripten::function("addJsFunction", &addJsFunction);
    emscripten::function("callJsFunction", &callJsFunction);
}