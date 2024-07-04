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
        void debug_displayKeyboard();
        void clearKeyboard();
        void updateKeyboard();
        bool anyKey();
        bool keyboard[16];
        bool quit;
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
};