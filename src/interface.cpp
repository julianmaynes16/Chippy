#include <interface.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <iostream>
#include <cstdint>
#include <stdio.h>

Interface::Interface(){

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, OUTPUT_WIDTH, OUTPUT_HEIGHT);
    
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = 0;
    }
    
    SDL_AudioSpec spec;
    SDL_zero(spec);
    spec.freq = FREQUENCY;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 735;
    spec.callback = nullptr;

    device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);

    squarewave(buffer);
    
    SDL_QueueAudio(device, buffer, 735 * 2);
    //SDL_PauseAudio(0);
    //debug_printBuffer();

    //SDL_Delay(3000);
    //SDL_CloseAudioDevice(device);
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

void Interface::squarewave(short* buffer){
    //fill up 100 sample buffer
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = ((i % PERIOD) < HALF_PERIOD) ? VOLUME : -VOLUME;
        //std::cout << buffer[i] << std::endl;
    }
}

void Interface::clearKeyboard(){
    for(int i = 0; i < 16; i++){
        keyboard[i]=0;
    }
}

void Interface::debug_displayKeyboard(){
    for(int i = 0; i < 16;i++){
        std::cout << keyboard[i] << " ";
    }
    std::cout << " " << std::endl;
}

void Interface::debug_printBuffer(){
    for(int i = 0; i < BUFFER_SIZE; i++){
        std::cout << buffer[i] << " ";
    }
    std::cout << " " << std::endl;
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
                   //top row      
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
                    //second row
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
                    //third row
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
                    //bottom row
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

bool Interface::anyKey(){
    bool keyPressed=false;
    for(int i=0; i < 16; i++){
        if(keyboard[i] == 1){
            keyPressed=true;
        }
    }
    if(keyPressed){
        return true;
    }else{
        return false;
    }
}
