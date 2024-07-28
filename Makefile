all:
	g++ -I include -L lib -o chippy src/main.cpp src/chip8.cpp src/interface.cpp -lmingw32 -lSDL2main -lSDL2