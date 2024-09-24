# Description

A website that can display sheet music and chord sheets for various songs. It also contains an editor for creating and designing chord sheets.
The backend is made with Django and the frontend is made with React, TailwindCSS, and C++ (compiled to WebAssembly with Emscripten).

# Build and Run

Needed tools: npm, Python, and Emscripten

Emscripten can be installed here: https://emscripten.org/docs/getting_started/downloads.html

Run these commands to build and run for the first time:

1. `cd musique_website/frontend`
2. `npm install`
3. `mkdir build`
4. `comp_all.bat`
5. `cd ..`
6. `python manage.py runserver`
7. Local server should be running now: go to localhost at port 8000 (http://localhost:8000 or http://127.0.0.1:8000)

If changes are made to the C++ code, then run `comp_all.bat` again.

If changes are made to the JavaScript code and not the C++ code, then run `npm run build` instead of `comp_all.bat` for faster compile time.

Just run `python manage.py runserver` again to run the server (any changes to the Python code should be automatically updated).

# Valid Values For Database

## Valid Tags

Christmas
Easter
Rock
Blues
Metal
Jazz
Pop
Classical
Country
Folk

Solo
Duet
Trio
Quartet
Quintet
Sextet

Instrumental
Fingerstyle Guitar
SATB

## Valid Difficulty Levels

Really Easy = 0
Average = 5
Really Hard = 10
