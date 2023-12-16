#include <iostream>
#include <emscripten.h>

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    int addNums(int a, int b)
    {
        return a + b;
    }
}
