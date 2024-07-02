#include <interface.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>

Interface::Interface(){

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chippy", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, OUTPUT_WIDTH, OUTPUT_HEIGHT);

}

Interface::~Interface(){
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Interface::displayScreen(Chip8* chip8){
    
    SDL_UpdateTexture(texture, nullptr, chip8->getScreen(), PITCH);

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer,texture, nullptr, nullptr);

    SDL_RenderPresent(renderer);
}

