g++ -static -std=c++11 -O3 -o "./bin/jsonserver" "./example/jsonserver.cpp" ^
-I"./src" ^
-I"./src/thirdparty" ^
-L"C:\msys64\mingw64\x86_64-w64-mingw32\lib" ^
-l"ws2_32" -l"wsock32"