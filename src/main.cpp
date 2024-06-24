#include <SDL2/SDL.h>
#include <chip8.h>
#include <stdio.h>
#include <iostream>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char *argv[])
{
    /*
   SDL_Window * window = nullptr;

    SDL_Surface * screenSurface = nullptr;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    screenSurface = SDL_GetWindowSurface(window);

    //Fill surface wqite
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));


    SDL_UpdateWindowSurface(window);

    //Hack to get window to stay up
    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

    SDL_DestroyWindow(window);
    SDL_Quit();
    */

    Chip8 chip8;
    chip8.debug_printRegisters();
    chip8.execute();
    chip8.debug_printRegisters();
    return 0;
}