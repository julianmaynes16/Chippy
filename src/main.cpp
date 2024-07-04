#include <SDL2/SDL.h>
#include <chip8.h>
#include <interface.h>
#include <stdio.h>
#include <iostream>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char *argv[])
{
    
    Chip8 chip8;
    Interface interface;
    while(!interface.quit){
        interface.updateKeyboard();
        chip8.execute(&interface);
        interface.clearKeyboard();
        interface.displayScreen(&chip8);
        
    }
    
    return 0;
}