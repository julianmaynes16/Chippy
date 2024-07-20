#include <SDL2/SDL.h>
#include <chip8.h>
#include <interface.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char *argv[])
{
    Chip8 chip8;
    Interface interface;

    uint32_t execution_cycle = 0;
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
            chip8.delayDecrement();
            chip8.soundDecrement();
            start_60_time = std::chrono::high_resolution_clock::now();
            
        }
        if((elapsed_clear_runtime.count()) >  0.25){
            interface.clearKeyboard();
            clear_time = std::chrono::high_resolution_clock::now();
        }
        execution_cycle++;
    }
    chip8.debug_printMemory();
    return 0;
}