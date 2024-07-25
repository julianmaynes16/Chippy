#pragma once
#include <SDL2/SDL.h>
#include "chip8.h"
#include <cstdint>
#include <iostream>
class Interface{
    public:
        Interface();
        ~Interface();
        SDL_AudioDeviceID device;
        void displayScreen(Chip8* chip8);
        void audioCallback(void *userdata, uint8_t *stream, int len);
        void callback(void *userdata, Uint8 *stream, int len);
        void audioPlay();
        void audioStop();
        void squarewave(short *buffer);
        void debug_displayKeyboard();
        void debug_printBuffer();
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
        const int SAMPLE_RATE = 44100;
        const int FREQUENCY = 440;
        //value between 0(mute) and 32767 (loud)
        const short VOLUME = 32000;
        int sample_index = 0;
        const int PERIOD = SAMPLE_RATE / FREQUENCY; // 100
        const int HALF_PERIOD = PERIOD / 2; // 50
        // multiply by two for stereo
        const int BYTES_PER_SAMPLE = sizeof(short) * 2;
        const int BUFFER_SIZE = 735;
        
        
};