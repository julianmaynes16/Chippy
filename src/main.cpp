#include <SDL2/SDL.h>
#include <chip8.h>
#include <interface.h>
#include <stdio.h>
#include <iostream>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char *argv[])
{
    bool quit = false;
    Chip8 chip8;
    Interface interface;
    while(!quit){
        chip8.execute();
        interface.displayScreen(&chip8);
        //chip8.debug_printScreen();
    }
    
    return 0;
}