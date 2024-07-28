#pragma once
#include <SDL2/SDL.h>
#include "chip8.h"
#include <cstdint>
#include <iostream>
class Interface{
    public:
        Interface();
        ~Interface();

        void displayScreen(Chip8* chip8);
        void setVolume(short volume);
        void audioPlay();
        void audioStop();
        void squarewave(short *buffer);
        void debug_displayKeyboard();
        void clearKeyboard();
        void updateKeyboard();
        bool anyKey();
        bool keyboard[16];
        bool quit;
        short buffer[735];
    private:

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        
        const int OUTPUT_WIDTH = 64;
        const int OUTPUT_HEIGHT = 32; 
        const int SCALE = 10;
        const int PITCH = OUTPUT_WIDTH * 4; //Width * bytes per pixel
        const int SCREEN_WIDTH = OUTPUT_WIDTH * SCALE;
        const int SCREEN_HEIGHT = OUTPUT_HEIGHT * SCALE;
        
        SDL_AudioDeviceID device;
        
        const int SAMPLE_RATE = 44100;
        //Default value
        short VOLUME = 15000;
        const int PERIOD = SAMPLE_RATE / 440; // 100
        const int HALF_PERIOD = PERIOD / 2; // 50
        static void callback(void* beeper, uint8_t* stream, int length);
};