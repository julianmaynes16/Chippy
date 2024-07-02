#include <interface.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>

Interface::Interface(){

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, OUTPUT_WIDTH, OUTPUT_HEIGHT);
    
    for (int i = 0; i < 16; i++){
        keyboard[i] = 0;
    }
    quit = false;
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

void Interface::updateKeyboard(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_x:
                        keyboard[0x0] = 1;
                        break;
                    case SDLK_1:
                        keyboard[0x1] = 1;
                        break;
                    case SDLK_2:
                        keyboard[0x2] = 1;
                        break;
                    case SDLK_3:
                        keyboard[0x3] = 1;
                        break;
                    case SDLK_4:
                        keyboard[0xC] = 1;
                        break;
                    case SDLK_q:
                        keyboard[4] = 1;
                        break;
                    case SDLK_w:
                        keyboard[5] = 1;
                        break;
                    case SDLK_e:
                        keyboard[6] = 1;
                        break;
                    case SDLK_r:
                        keyboard[0xD] = 1;
                        break;
                    case SDLK_a:
                        keyboard[7] = 1;
                        break;
                    case SDLK_s:
                        keyboard[8] = 1;
                        break;
                    case SDLK_d:
                        keyboard[9] = 1;
                        break;
                    case SDLK_f:
                        keyboard[0xE] = 1;
                        break;
                    case SDLK_z:
                        keyboard[0xA] = 1;
                        break;
                    case SDLK_c:
                        keyboard[0xB] = 1;
                        break;
                    case SDLK_v:
                        keyboard[0xF] = 1;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}
