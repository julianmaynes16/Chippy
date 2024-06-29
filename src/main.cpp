#include <SDL2/SDL.h>
#include <chip8.h>
#include <stdio.h>
#include <iostream>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char *argv[])
{

    Chip8 chip8;
    //chip8.sdlInit();
    chip8.execute();
    chip8.sdlInit();
    //chip8.debug_printScreen();
    return 0;
}