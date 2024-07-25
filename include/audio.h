#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#pragma once

class Audio{
    public:
        Audio();
        //~Audio();
        void audio_callback();
        void generateSquarewave();
        void audio_play();
        void audio_stop();

    private:
        int SAMPLE_RATE = 44100;
        int VOLUME = 3000;
        
        SDL_AudioDeviceID device;
        static void callback(void* beeper, uint8_t* stream, int length);

        
};