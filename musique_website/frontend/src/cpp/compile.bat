@cd build
call emcmake cmake ..
call emmake make 
@cd ..
@python comp.py