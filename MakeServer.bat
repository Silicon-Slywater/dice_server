g++ -static-libgcc -static-libstdc++ server.cpp -o bin\server.exe server.res -lws2_32 -lwininet -Os
pause