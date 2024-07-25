#include <SDL2/SDL.h>
#include <chip8.h>
#include <interface.h>
#include <audio.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cout << "Please add a secondary argument for delay (1-3 works well)." << std::endl;
        return 0;
    }
    Chip8 chip8;
    Interface interface;
    Audio audio;
    //how frequent program will run, lower = faster  
    //int delay_value = 500000;
    //int delay_value = 15000;
    //origin time
    int delay_value = std::stoi(argv[1]);
    std::chrono::high_resolution_clock::time_point execution_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point start_60_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point clear_time = std::chrono::high_resolution_clock::now();

    while(!interface.quit){
        
        //new time
        std::chrono::high_resolution_clock::time_point timestamp = std::chrono::high_resolution_clock::now();
        //total runtime
        std::chrono::duration<double> elapsed_execution_runtime = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp - execution_time);
        std::chrono::duration<double> elapsed_60_runtime = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp - start_60_time);
        std::chrono::duration<double> elapsed_clear_runtime = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp - clear_time);

        //instructions read at own pace, slowed by delay value
        if(elapsed_execution_runtime.count() > (0.001 * delay_value)){
            chip8.execute(&interface);
            execution_time = std::chrono::high_resolution_clock::now();
        }
        //60fps screen and keyboard read
        if((elapsed_60_runtime.count()) >  0.01666667){
            interface.updateKeyboard();
            interface.displayScreen(&chip8);
            //interface.debug_displayKeyboard();
            
            //SDL_CloseAudioDevice(interface.device);
            chip8.delayDecrement();
            chip8.soundDecrement(&audio);
            if(chip8.getSoundTimer() > 0){
                audio.audio_play();
            }else{
                audio.audio_stop();
            }
            start_60_time = std::chrono::high_resolution_clock::now();
            
        }
        if((elapsed_clear_runtime.count()) >  0.25){
            interface.clearKeyboard();
            clear_time = std::chrono::high_resolution_clock::now();
        }
    }
    chip8.debug_printMemory();
    return 0;
}