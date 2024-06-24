#include <chip8.h>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <fstream>
#include <filesystem>



Chip8::Chip8(std::string file_name){
    //make all memory default 0
    for(int i = 0; i < 4096; i++){
        memory[i] = 0;
    }
    //Load default spritesheet
    for (int i = 0; i < 5; i++){
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

    //Load ROM
    for(auto const& directory_entry : std::filesystem::directory_iterator("../rom/")){
        if(directory_entry.path().extension() == ".ch8"){
           std::ifstream ich8(directory_entry.path());
           if(ich8.fail()){
            std::cout <<"Couldn't open file" << std::endl;
           }else{
                std::cout << "Successfully opened file" << std::endl;
                std::cout << "Loading ROM..." << std::endl;
                for(int i = 1536; i < 4096; i++){
                    uint8_t hex_byte;
                    ich8 >> std::hex >> hex_byte;
                    if(ich8.fail()){
                        std::cout << "Error occured while reading ROM" << std::endl;
                    }
                    memory[i] = hex_byte;
                }
                std::cout << "ROM read successful" << std::endl;
                ich8.close();
                break;
           } 
        }
    }
    std::cout << "Couldn't find ROM. Make sure the file is in the rom folder and has the .ch8 extension" << std::endl;
}



void Chip8::debug_printMemory()
{ // populates designated memory spaces
    for(int i = 0; i < 4096; i++){
        if((i % 16) == 0){
            std::cout << "" << std::endl;
        }
        std::cout << std::hex << memory[i] << " ";
    }
}



// assembly functions, xy = each 4bit hex val, nnn = addr/address, n = nibble/lowest 4 bit of instruction, kk = byte/8 bit
void Chip8::Op_00E0()
{ // CLR, Clear display
    std::cout << "Hi";
}

void Chip8::Op_00EE()
{ // RET, return from subroutine
    // set program counter to the top of stack , subtracts one from stack pointer
    PC = stack[0];
    SP--;
}

void Chip8::Op_1nnn(uint16_t nnn)
{ // [JP address] Jump, set program countert to nnn
    PC = nnn;
}

void Chip8::Op_2nnn(uint16_t nnn)
{ // [CALL address] Call, inmcremend stack pointer, puts current PC on top of stack, setting it to nnnn
    SP++;
    stack[0] = PC;
    PC = nnn;
}

void Chip8::Op_3xkk(uint8_t Vx, uint8_t kk)
{ // [SE Vx, byte], if reg Vx = kk, increment PC by 2
    if (registers[Vx] == kk)
    {
        PC += 2;
    }
}

void Chip8::Op_4xkk(uint8_t Vx, uint8_t kk)
{ // [SNE Vx, byte] if reg Vx != kk, increment PC by 2
    if (registers[Vx] != kk)
    {
        PC += 2;
    }
}

void Chip8::Op_5xy0(uint8_t Vx, uint8_t Vy)
{ // [SE Vx, Vy] if reg Vx = reg Vy, increment PC by 2
    if (registers[Vx] == registers[Vy])
    {
        PC += 2;
    }
}

void Chip8::Op_6xkk(uint8_t Vx, uint8_t kk)
{ // [LD Vx, byte] Puts value kk into register Vx
    registers[Vx] = kk;
}

void Chip8::Op_7xkk(uint8_t Vx, uint8_t kk)
{ // ADD [Vx, byte] Add kk to value in reg Vx, store result in Vx
    uint8_t result = registers[Vx] + kk;
    registers[Vx] = result;
}

void Chip8::Op_8xy0(uint8_t Vx, uint8_t Vy){ // [LD Vx Vy] Stores value of reg Vy into Vx
    registers[Vx] = registers[Vy];
}

void Chip8::Op_8xy1(uint8_t Vx, uint8_t Vy){ // [Vx OR Vy] Bitwise OR on Vx and Vy, stores result in Vx 
    registers[Vx] = (registers[Vx] | registers[Vy]);
}

void Chip8::Op_8xy2(uint8_t Vx, uint8_t Vy){ // [Vx AND Vy] Bitwise AND on Vx and Vy, stores result in Vx 
    registers[Vx] = (registers[Vx] & registers[Vy]);
}

void Chip8::Op_8xy3(uint8_t  Vx, uint8_t Vy){ // [Vx XOR Vy] Bitwise XOR on Vx and Vy, stores result in Vx 
    registers[Vx] = (registers[Vx] ^ registers[Vy]);
}

void Chip8::Op_8xy4(uint8_t Vx, uint8_t Vy){ // [ADD Vx, Vy] Vx = Vx+Vy, Vf = carry
    uint16_t result = (registers[Vx] + registers[Vy]);
    if (result > 255){
        VF = 1;
    }else{
        VF = 0;
    }
    registers[Vx] = uint8_t(result);
}
void Chip8::Op_8xy5(uint8_t Vx, uint8_t Vy){ // [SUB Vx, Vy] Vx = Vx-Vy, Vf = 1 if Vx>Vy, otheriwse 0
    if(registers[Vx] >= registers[Vy]){
        VF = 1;
    }else{
        VF=0;
    }
    registers[Vx] = (registers[Vx] - registers[Vy]);
}

void Chip8::Op_8xy6(uint8_t Vx, uint8_t Vy){ // [SHR Vx {, Vy}] Vx = Vx SHR 1y, Vf = least significant bit
    VF = registers[Vx] & 1;
    registers[Vx] >>= 1;
}

void Chip8::Op_8xy7(uint8_t Vx, uint8_t Vy){ // [SUBN Vx, Vy] Vx = Vy - Vx, 
    if(registers[Vy] >= registers[Vx]){
        VF = 1;
    }else{
        VF=0;
    }
    registers[Vx] = (registers[Vy] - registers[Vx]);
}

void Chip8::Op_8xyE(uint8_t Vx, uint8_t Vy){ // [SHL Vx {, Vy}] Vx = Vx SHL 1, 
    VF = registers[Vx] & 0x80;
    registers[Vx] <<= 1;
}

void Chip8::Op_9xy0(uint8_t Vx, uint8_t Vy){ // [SNE Vx, Vy], IF reg Vx != reg  Vy, increase PC by 2
    if(registers[Vx] != registers[Vy]){
        PC += 2;
    }
}

void Chip8::Op_Annn(uint16_t nnn){ // [Ld I, addr], set register I to nnnn
    I_reg = nnn;
}

void Chip8::Op_Bnnn(uint16_t nnn){ // [JP V0, addr], jump to location nnn + Vo
    PC = registers[0] + nnn;
}

void Chip8::Op_Cxkk(uint8_t Vx, uint8_t kk){ // [RND Vx, byte], generates random 8 bit number, AND'd with kk, set ot to Vx
    registers[Vx] = (rand() % 255) & kk;
}
//TODO
void Chip8::Op_Dxyn(uint8_t Vx, uint8_t Vy, uint8_t n){ 
    // [DRW Vx, Vy, nibble], Read n bytes from memory starting at I. 
    //Bytes are displayed as sprites at coordinates in reg Vx and Vy.
    //If pixels are erased, Vf is set to 1, otherwise Vf is 0
    //If displayed outside coordinates, wrap around.
}

void Chip8::Op_Ex9E(uint8_t Vx){ // [SKP Vx] If key coresponding to value of Vx is currently pressed, PC increases by 2
}

void Chip8::Op_ExA1(uint8_t Vx){ // [SKNP Vx] If key coresponding to value of Vx is NOT currently pressed, PC increases by 2
}

void Chip8::Op_Fx07(uint8_t Vx){ // [LD Vx, DT] Value of DT is placed into VX
    registers[Vx] = delayTimer;
}

void Chip8::Op_Fx0A(uint8_t Vx){ // [LD Vx, K] All execution stops until a key is pressed, then the value if stored in Vx
}

void Chip8::Op_Fx15(uint8_t Vx){ // [LD Dt, Vx] Set delay timer = Vx
    delayTimer = registers[Vx];
}

void Chip8::Op_Fx18(uint8_t Vx){ // [LD St, Vx] Set sound timer = Vx
    soundTimer = registers[Vx];
}

void Chip8::Op_Fx1E(uint8_t Vx){ // [Add I, Vx] Values of I and Vx, results stored in I
    I_reg += registers[Vx];
}

void Chip8::Op_Fx29(uint8_t Vx){ // [LD F, Vx] I = location of hex sprite for value of reg Vx 
}

void Chip8::Op_Fx33(uint8_t Vx){ // [LD B, Vx] I = decimal vlaue of Vx, places hundreds digit in memory at location in I, tens digit at I + 1, ones digit at I + 2 
    uint8_t hundreds_digit = registers[Vx] / 100;
    uint8_t tens_digit = (registers[Vx] - (hundreds_digit * 100)) / 10;
    uint8_t ones_digit = (registers[Vx] - (hundreds_digit * 100)) % 10;
    
    memory[I_reg] = hundreds_digit;
    memory[I_reg + 1] = tens_digit;
    memory[I_reg + 2] = ones_digit; 
}

void Chip8::Op_Fx55(uint8_t Vx){ // [LD [I], Vx] Copies values of registers V0 through Vx into memory starting at address I
    for(int i = 0; i <= Vx; i++){
        memory[I_reg + i] = registers[i];
    }
}

void Chip8::Op_Fx65(uint8_t Vx){ // [LD Vx, [I]] Puts values from memory starting at location In into registers V0 through Vx
    for(int i = 0; i <= Vx; i++){
        registers[i] = memory[I_reg + i];
    }
}