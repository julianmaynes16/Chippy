#pragma once
#include <audio.h>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>
#include <string>

class Interface;
class Chip8
{
public:

    Chip8();
    
    void debug_printMemory();

    void debug_printRegisters();

    void debug_printScreen();

    void debug_printInstruction(uint16_t instr);
    
    void sdlInit();

    void incrementPC();

    uint32_t* getScreen();

    void interpret(Interface* interface);

    void delayDecrement();

    void soundDecrement(Audio* audio);

    uint8_t getSoundTimer();

    void execute(Interface* interface);

    // assembly functions, xy = each 4bit hex val, nnn = addr/address, n = nibble/lowest 4 bit of instruction, kk = byte/8 bit
    void Op_00E0();
    

    void Op_00EE();
    

    void Op_1nnn(uint16_t nnn);
    

    void Op_2nnn(uint16_t nnn);
    

    void Op_3xkk(uint8_t Vx, uint8_t kk);
    

    void Op_4xkk(uint8_t Vx, uint8_t kk);
    

    void Op_5xy0(uint8_t Vx, uint8_t Vy);
    

    void Op_6xkk(uint8_t Vx, uint8_t kk);
    

    void Op_7xkk(uint8_t Vx, uint8_t kk);
    

    void Op_8xy0(uint8_t Vx, uint8_t Vy); // [LD Vx Vy] Stores value of reg Vy into Vx
       
    

    void Op_8xy1(uint8_t Vx, uint8_t Vy); // [Vx OR Vy] Bitwise OR on Vx and Vy, stores result in Vx 
        
    

    void Op_8xy2(uint8_t Vx, uint8_t Vy); // [Vx AND Vy] Bitwise AND on Vx and Vy, stores result in Vx 
        
    

    void Op_8xy3(uint8_t  Vx, uint8_t Vy); // [Vx XOR Vy] Bitwise XOR on Vx and Vy, stores result in Vx 
       
    

    void Op_8xy4(uint8_t Vx, uint8_t Vy); // [ADD Vx, Vy] Vx = Vx+Vy, Vf = carry

    
    void Op_8xy5(uint8_t Vx, uint8_t Vy); // [SUB Vx, Vy] Vx = Vx-Vy, Vf = 1 if Vx>Vy, otheriwse 0
    

    void Op_8xy6(uint8_t Vx, uint8_t Vy); // [SHR Vx {, Vy}] Vx = Vx SHR 1y, Vf = least significant bit
       
    

    void Op_8xy7(uint8_t Vx, uint8_t Vy); // [SUBN Vx, Vy] Vx = Vy - Vx, 
     
    

    void Op_8xyE(uint8_t Vx, uint8_t Vy); // [SHL Vx {, Vy}] Vx = Vx SHL 1, 
      
    

    void Op_9xy0(uint8_t Vx, uint8_t Vy); // [SNE Vx, Vy], IF reg Vx != reg  Vy, increase PC by 2
       
    
    
    void Op_Annn(uint16_t nnn); // [Ld I, addr], set register I to nnnn
      
    

    void Op_Bnnn(uint16_t nnn); // [JP V0, addr], jump to location nnn + Vo
       
    

    void Op_Cxkk(uint8_t Vx, uint8_t kk); // [RND Vx, byte], generates random 8 bit number, AND'd with kk, set ot to Vx
      
    
    void Op_Dxyn(uint8_t Vx, uint8_t Vy, uint8_t n);
    
    

    void Op_Ex9E(uint8_t Vx, Interface* interface); // [SKP Vx] If key coresponding to value of Vx is currently pressed, PC increases by 2
    

    void Op_ExA1(uint8_t Vx, Interface* interface); // [SKNP Vx] If key coresponding to value of Vx is NOT currently pressed, PC increases by 2
    

    void Op_Fx07(uint8_t Vx); // [LD Vx, DT] Value of DT is placed into VX
    

    void Op_Fx0A(uint8_t Vx, Interface* interface); // [LD Vx, K] All execution stops until a key is pressed, then the value if stored in Vx
    

    void Op_Fx15(uint8_t Vx); // [LD Dt, Vx] Set delay timer = Vx
       
    

    void Op_Fx18(uint8_t Vx); // [LD St, Vx] Set sound timer = Vx
       
    
    
    void Op_Fx1E(uint8_t Vx);// [Add I, Vx] Values of I and Vx, results stored in I
        
    

    void Op_Fx29(uint8_t Vx); // [LD F, Vx] I = location of hex sprite for value of reg Vx 
    

    void Op_Fx33(uint8_t Vx); // [LD B, Vx] I = decimal vlaue of Vx, places hundreds digit in memory at location in I, tens digit at I + 1, ones digit at I + 2 
       
    

    void Op_Fx55(uint8_t Vx); // [LD [I], Vx] Copies values of registers V0 through Vx into memory starting at address I
       
    

    void Op_Fx65(uint8_t Vx); // [LD Vx, [I]] Puts values from memory starting at location In into registers V0 through Vx
     

    private:

        // memory
        uint8_t memory[4096]; // 4KB memory
        // registers
        uint8_t registers[16];
        uint16_t I_reg;
        uint8_t delayTimer;
        uint8_t soundTimer;

        uint16_t PC; // Program Counter, points to currently executing address
        uint8_t SP;  // Stack pointer, points to topmost level of stack
        // stack
        uint16_t stack[16];
        //screen, 64 x 32 pixels, 8 bytes x 4 bytes 
        uint32_t screen[64*32]; //each pixel is either 0xFF (on)or 0x00(off)  
        
        const uint8_t sprite_0[5] = {0xF0, 0x90, 0x90, 0x90, 0xF0};
        const uint8_t sprite_1[5] = {0x20, 0x60, 0x20, 0x20, 0x70};
        const uint8_t sprite_2[5] = {0xF0, 0x10, 0xF0, 0x80, 0xF0};
        const uint8_t sprite_3[5] = {0xF0, 0x10, 0xF0, 0x10, 0xF0};
        const uint8_t sprite_4[5] = {0x90, 0x90, 0xF0, 0x10, 0x10};
        const uint8_t sprite_5[5] = {0xF0, 0x80, 0xF0, 0x10, 0xF0};
        const uint8_t sprite_6[5] = {0xF0, 0x80, 0xF0, 0x90, 0xF0};
        const uint8_t sprite_7[5] = {0xF0, 0x10, 0x20, 0x40, 0x40};
        const uint8_t sprite_8[5] = {0xF0, 0x90, 0xF0, 0x90, 0xF0};
        const uint8_t sprite_9[5] = {0xF0, 0x90, 0xF0, 0x10, 0xF0};
        const uint8_t sprite_A[5] = {0xF0, 0x90, 0xF0, 0x90, 0x90};
        const uint8_t sprite_B[5] = {0xE0, 0x90, 0xE0, 0x90, 0xE0};
        const uint8_t sprite_C[5] = {0xF0, 0x80, 0x80, 0x80, 0xF0};
        const uint8_t sprite_D[5] = {0xE0, 0x90, 0x90, 0x90, 0xE0};
        const uint8_t sprite_E[5] = {0xF0, 0x80, 0xF0, 0x80, 0xF0};
        const uint8_t sprite_F[5] = {0xF0, 0x80, 0xF0, 0x80, 0x80};

};