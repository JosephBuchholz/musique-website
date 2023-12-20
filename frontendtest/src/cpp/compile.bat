REM emcc Main.cpp -o ../../static/wasm/cpp.js
REM emcc Main.cpp -o ../cpp.js -s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap
REM emcc -lembind Main.cpp -o ../cpp.js --embind-emit-tsd cpp.d.ts
emcc -lembind Main.cpp -o ../cpp.js