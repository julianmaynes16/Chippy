#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

class chip8
{
public:
    // memory
    uint8_t memory[4096]; // 4KB memory
    // registers
    uint8_t registers[16];
    uint16_t I_reg;
    uint8_t delayTimer;
    uint8_t soundTimer;
    bool VF;

    uint16_t PC; // Program Counter, points to currently executing address
    uint8_t SP;  // Stack pointer, points to topmost level of stack
    // stack
    uint16_t stack[16];

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

    void initializeMemory()
    { // populates designated memory spaces
        for (int i = 0; i < 5; i++)
        {
            memory[i + 80] = sprite_0[i];
            memory[i + 85] = sprite_1[i];
            memory[i + 90] = sprite_2[i];
            memory[i + 95] = sprite_3[i];
            memory[i + 100] = sprite_4[i];
            memory[i + 105] = sprite_5[i];
            memory[i + 110] = sprite_6[i];
            memory[i + 115] = sprite_7[i];
            memory[i + 120] = sprite_8[i];
            memory[i + 125] = sprite_9[i];
            memory[i + 130] = sprite_A[i];
            memory[i + 135] = sprite_B[i];
            memory[i + 140] = sprite_C[i];
            memory[i + 145] = sprite_D[i];
            memory[i + 150] = sprite_E[i];
            memory[i + 155] = sprite_F[i];
        }
    }

    // assembly functions, xy = each 4bit hex val, nnn = addr/address, n = nibble/lowest 4 bit of instruction, kk = byte/8 bit
    void Op_00E0()
    { // CLR, Clear display
        std::cout << "Hi";
    }

    void Op_00EE()
    { // RET, return from subroutine
        // set program counter to the top of stack , subtracts one from stack pointer
        PC = stack[0];
        SP--;
    }

    void Op_1nnn(uint16_t nnn)
    { // [JP address] Jump, set program countert to nnn
        PC = nnn;
    }

    void Op_2nnn(uint16_t nnn)
    { // [CALL address] Call, inmcremend stack pointer, puts current PC on top of stack, setting it to nnnn
        SP++;
        stack[0] = PC;
        PC = nnn;
    }

    void Op_3xkk(uint8_t Vx, uint8_t kk)
    { // [SE Vx, byte], if reg Vx = kk, increment PC by 2
        if (registers[Vx] == kk)
        {
            PC += 2;
        }
    }

    void Op_4xkk(uint8_t Vx, uint8_t kk)
    { // [SNE Vx, byte] if reg Vx != kk, increment PC by 2
        if (registers[Vx] != kk)
        {
            PC += 2;
        }
    }

    void Op_5xy0(uint8_t Vx, uint8_t Vy)
    { // [SE Vx, Vy] if reg Vx = reg Vy, increment PC by 2
        if (registers[Vx] == registers[Vy])
        {
            PC += 2;
        }
    }

    void Op_6xkk(uint8_t Vx, uint8_t kk)
    { // [LD Vx, byte] Puts value kk into register Vx
        registers[Vx] = kk;
    }

    void Op_7xkk(uint8_t Vx, uint8_t kk)
    { // ADD [Vx, byte] Add kk to value in reg Vx, store result in Vx
        uint8_t result = registers[Vx] + kk;
        registers[Vx] = result;
    }

    void Op_8xy0(uint8_t Vx, uint8_t Vy){ // [LD Vx Vy] Stores value of reg Vy into Vx
        registers[Vx] = registers[Vy];
    }

    void Op_8xy1(uint8_t Vx, uint8_t Vy){ // [Vx OR Vy] Bitwise OR on Vx and Vy, stores result in Vx 
        registers[Vx] = (registers[Vx] | registers[Vy]);
    }

    void Op_8xy2(uint8_t Vx, uint8_t Vy){ // [Vx AND Vy] Bitwise AND on Vx and Vy, stores result in Vx 
        registers[Vx] = (registers[Vx] & registers[Vy]);
    }

    void Op_8xy3(uint8_t  Vx, uint8_t Vy){ // [Vx XOR Vy] Bitwise XOR on Vx and Vy, stores result in Vx 
        registers[Vx] = (registers[Vx] ^ registers[Vy]);
    }

    void Op_8xy4(uint8_t Vx, uint8_t Vy){ // [ADD Vx, Vy] Vx = Vx+Vy, Vf = carry
        uint16_t result = (registers[Vx] + registers[Vy]);
        if (result > 255){
            VF = 1;
        }
        registers[Vx] = uint8_t(result);
    }
    void Op_8xy5(uint8_t Vx, uint8_t Vy){ // [SUB Vx, Vy] Vx = Vx-Vy, Vf = 1 if Vx>Vy, otheriwse 0
        if(registers[Vx] >= registers[Vy]){
            VF = 1;
        }else{
            VF=0;
        }
        registers[Vx] = (registers[Vx] - registers[Vy]);
    }

    void Op_8xy6(uint8_t Vx, uint8_t Vy){ // [SHR Vx {, Vy}] Vx = Vx SHR 1y, Vf = least significant bit
        VF = registers[Vx] & 1;
        registers[Vx] >>= 1;
    }

    void Op_8xy7(uint8_t Vx, uint8_t Vy){ // [SUBN Vx, Vy] Vx = Vy - Vx, 
      if(registers[Vy] >= registers[Vx]){
            VF = 1;
        }else{
            VF=0;
        }
        registers[Vx] = (registers[Vy] - registers[Vx]);
    }
    

};