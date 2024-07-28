#include <interface.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <iostream>
#include <cstdint>
#include <stdio.h>

Interface::Interface(){
    //Initialize video
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, OUTPUT_WIDTH, OUTPUT_HEIGHT);
    
    //Initialize default keyboard inputs
    for (int i = 0; i < 16; i++){
        keyboard[i] = 0;
    }
    quit = false;

    //Initialize audio
    SDL_AudioSpec desired;
	desired.freq = SAMPLE_RATE;
	desired.format = AUDIO_S16SYS;
	desired.channels = 1;
	desired.samples = 512;
	desired.callback = Interface::callback;
	desired.userdata = this;

	SDL_AudioSpec obtained;
	device = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desired, &obtained, 0);
}
    
Interface::~Interface(){
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_CloseAudioDevice(device);
    SDL_Quit();
}
/**
 * @brief Display 
 */
void Interface::displayScreen(Chip8* chip8){
    
    SDL_UpdateTexture(texture, nullptr, chip8->getScreen(), PITCH);

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer,texture, nullptr, nullptr);

    SDL_RenderPresent(renderer);
}
/**
 * @brief clears keyboard of all inputs, opens for another input
 */
void Interface::clearKeyboard(){
    for(int i = 0; i < 16; i++){
        keyboard[i]=0;
    }
}
/**
 * @brief Displays currently pressed keys
 */
void Interface::debug_displayKeyboard(){
    for(int i = 0; i < 16;i++){
        std::cout << keyboard[i] << " ";
    }
    std::cout << " " << std::endl;
}
/**
 * @brief Updates array depending on what key is pressed
 */
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
/**
 * @brief Handles audio playback by putting squarewave into buffer 
 */
void Interface::callback(void* context, uint8_t* stream, int bytes) {
	Interface* interface = (Interface*)context;
	int16_t* buffer = (int16_t*)stream;
	int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    //int SAMPLE_RATE = 44100;
    //const short VOLUME = 15000;
    
	for (int i = 0; i < length; i++) {
        buffer[i] = ((i % interface->PERIOD) < interface->HALF_PERIOD) ? interface->VOLUME : (interface->VOLUME) * -1;
	}
}
/**
 * @brief Sets volume, 0 - 32767, higher num = higher volume
 */
void Interface::setVolume(short volume){
    VOLUME = volume;
}
/**
 * @brief Plays audio
 */
void Interface::audioPlay() {
	SDL_PauseAudioDevice(device, 0);
}
/**
 * @brief Stops audio 
 */
void Interface::audioStop() {
	SDL_PauseAudioDevice(device, 1);
}
/**
 * @brief Returns true if any key is currently pressed 
 */
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
