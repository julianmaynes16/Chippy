#include <chip8.h>
#include <cstdint>
#include <iostream>
class Interface{
    public:
        Interface();
        ~Interface();
        void displayScreen(Chip8* chip8);

    private:

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;

        const int OUTPUT_WIDTH = 64;
        const int OUTPUT_HEIGHT = 32; 
        const int SCALE = 10;
        const int PITCH = OUTPUT_WIDTH; //Width * bytes per pixel
        const int SCREEN_WIDTH = OUTPUT_WIDTH * SCALE;
        const int SCREEN_HEIGHT = OUTPUT_HEIGHT * SCALE;
};