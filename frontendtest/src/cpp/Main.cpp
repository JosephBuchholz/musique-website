#include <iostream>
//#include <emscripten.h>
#include <emscripten/bind.h>

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

int addNums(int a, int b)
{
    return a + b;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("addNums", &addNums);
}