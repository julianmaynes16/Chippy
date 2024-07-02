#include <chip8.h>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <fstream>
#include <filesystem>



Chip8::Chip8(){
    //Set default PC
    PC = 0x200;
    //Set registers to default 0
    for(int i = 0; i < 16; i++){
        registers[i] = 0;
    }
    //stack initialization
    SP = 0;
    for(int i = 0; i < 16;i++){
        stack[i] = 0;
    }

    //Other inits
    I_reg = 0;
    delayTimer = 0;
    soundTimer = 0;

    //set screen to blank
    for(int i = 0; i < 2048; i++){
        screen[i] = 0; 
    }


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
    for(auto const& directory_entry : std::filesystem::directory_iterator("rom/")){
        if(directory_entry.path().extension() == ".ch8"){
           std::ifstream ich8(directory_entry.path(), std::ios::binary);
           if(!ich8.fail()){
                std::cout << "Successfully opened file" << std::endl;
                std::cout << "Loading ROM..." << std::endl;
                int i = 512;
                ich8 >> std::noskipws;
                uint8_t hex_byte;
                while(ich8 >> hex_byte){
                    if(ich8.fail()){
                        std::cout << "Error occured while reading ROM" << std::endl;
                        //break;
                    }
                    memory[i++] = hex_byte;
                }
                std::cout << "ROM read successful" << std::endl;
                ich8.close();
                break;
           }else{
                std::cout <<"Couldn't open file" << std::endl;
                break;
           } 
        }
    }
    //std::cout << "Couldn't find ROM. Make sure the file is in the rom folder and has the .ch8 extension" << std::endl;
}


/**
 * @brief prints our chip8's entire memory
 */
void Chip8::debug_printMemory()
{ // populates designated memory spaces
    for(int i = 0; i < 4096; i++){
        if((i % 16) == 0){
            std::cout << "" << std::endl;
            std::cout << i << ": ";
        }
        std::cout << std::setw(2) << std::setfill('0') << std::hex << int(memory[i]) << " ";
    }
}
/**
 * @brief prints values currently stored in registers
 */
void Chip8::debug_printRegisters()
{ // populates designated memory spaces
    for(int i = 0; i < 16; i++){
        std::cout << std::setw(2) << std::setfill('0') << std::hex << int(registers[i]) << " ";
    }
    std::cout << std::endl;
}
/**
 * @brief prints entire screen in terminal, 0x00 = black, 0xff = white
 */
void Chip8::debug_printScreen(){
    for(int i = 0; i < 2048 ; i++){
        if((i % 64) == 0){
            std::cout << "" << std::endl;
        }
        std::cout <<  std::setw(2) << std::setfill('0') << std::hex << int(screen[i]) << " ";
    }
}
/**
 * @brief prints surrently executing assembly instruction in hex
 */
void Chip8::debug_printInstruction(uint16_t instr){
    std::cout <<  std::setw(4) << std::setfill('0') << std::hex << int(instr) << " ";
}
/**
 * @brief Increases program counter by 2, allowing next instruction to be read
 */
void Chip8::incrementPC(){
    //Moves to next 4 hex digits
    PC+=2;
}
/**
 * @brief Returns screen list
 */
uint8_t* Chip8::getScreen(){
    return screen;
}

/**
 * @brief reads next assembly instruction and calls associated function
*/
void Chip8::interpret(){
    uint16_t half_instr = memory[PC];
    half_instr <<=8;
    uint16_t instr = half_instr | memory[PC+1];
    uint8_t x = (instr & 0x0F00) >> 8;
    uint8_t y = (instr & 0x00F0) >> 4;
    uint8_t kk = instr & 0x00FF;
    uint8_t lsb =  instr & 0x000F;  
    uint8_t bottom_2 = instr & 0x00FF;
    uint8_t n = instr & 0x000F;
    uint16_t addr = instr & 0x0FFF;
    switch(memory[PC] >> 4){ // MSB
        case 0x00:
            if(instr == 0x00E0){
                Op_00E0();
            }else{
                Op_00EE();
            }
            break;
        case 0x01:
            Op_1nnn(addr);
            break;
        case 0x02:
            Op_2nnn(addr);
            break;
        case 0x03:
            Op_3xkk(x, kk);
            break;
        case 0x04:
            Op_4xkk(x, kk);
            break;
        case 0x05:
            Op_5xy0(x,y);
            break;
        case 0x06:
            Op_6xkk(x, kk);
            break;
        case 0x07:
            Op_7xkk(x, kk);
            break;
        case 0x08: // 8 possibilities
            switch(lsb){
                case 0:
                    Op_8xy0(x,y);
                    break;
                case 1:
                    Op_8xy1(x,y);
                    break;
                case 2:
                    Op_8xy2(x,y);
                    break;
                case 3:
                    Op_8xy3(x,y);
                    break;
                case 4:
                    Op_8xy4(x,y);
                    break;
                case 5:
                    Op_8xy5(x,y);
                    break;
                case 6:
                    Op_8xy6(x,y);
                    break;
                case 7:
                    Op_8xy7(x,y);
                    break;
                case 0x0E:
                    Op_8xyE(x,y);
                    break;
            }
            break;
        case 0x09:
            Op_9xy0(x,y);
            break;
        case 0x0A:
            Op_Annn(addr);
            break;
        case 0x0B:
            Op_Bnnn(addr);
            break;
        case 0x0C:
            Op_Cxkk(x, kk);
            break;
        case 0x0D:
            Op_Dxyn(x,y,n);
            break;
        case 0x0E:
            if((instr & 0x00FF) == 0x9E){
                Op_Ex9E(x);
            }else{
                Op_ExA1(x);
            }
            break;
        case 0x0F:
            switch(bottom_2){
                case 0x07:
                    Op_Fx07(x);
                    break;
                case 0x0A:
                    Op_Fx0A(x);
                    break;
                case 0x15:
                    Op_Fx15(x);
                    break;
                case 0x18:
                    Op_Fx18(x);
                    break;
                case 0x1E:
                    Op_Fx1E(x);
                    break;
                case 0x29:
                    Op_Fx29(x);
                    break;
                case 0x33:
                    Op_Fx33(x);
                    break;
                case 0x55:
                    Op_Fx55(x);
                    break;
                case 0x65:
                    Op_Fx65(x);
                    break;
            }
            break;
    }
}
/**
 * @brief decrements delay by 1. Should work at 60Hz but doesnt. Shouldnt be an issue
 */
void Chip8::delayDecrement(){
    if(delayTimer > 0){
        delayTimer--;
    }
}
/**
 * @brief decrements sound by 1. Should operate at 60Hz but doesnt
 */
void Chip8::soundDecrement(){
    if(soundTimer > 0){
        soundTimer--;
    }
}

/**
 * @brief Executes Code from address 0x200 (512) to 0xFFF (4095)
 */
void Chip8::execute(){
    //returns full 4 hex digit instruction
    interpret();
    delayDecrement();
    soundDecrement();
}
// assembly functions, xy = each 4bit hex val, nnn = addr/address, n = nibble/lowest 4 bit of instruction, kk = byte/8 bit
void Chip8::Op_00E0()
{ // CLR, Clear display
    for(int i = 0; i < 2048; i++){
        screen[i] = 0; 
    }
    incrementPC();
}

void Chip8::Op_00EE()
{ // RET, return from subroutine
    // set program counter to the top of stack , subtracts one from stack pointer
    PC = stack[0];
    SP--;
    incrementPC();
}

void Chip8::Op_1nnn(uint16_t nnn)
{ // [JP address] Jump, set program countert to nnn
    PC = nnn;
}

void Chip8::Op_2nnn(uint16_t nnn)
{ // [CALL address] Call, inmcremend stack pointer, puts current PC on top of stack, setting it to nnn
    SP++;
    stack[0] = PC;
    PC = nnn;
}

void Chip8::Op_3xkk(uint8_t Vx, uint8_t kk)
{ // [SE Vx, byte], if reg Vx = kk, increment PC by 2
    if (registers[Vx] == kk)
    {
        incrementPC();
    }
    incrementPC();
}

void Chip8::Op_4xkk(uint8_t Vx, uint8_t kk)
{ // [SNE Vx, byte] if reg Vx != kk, increment PC by 2
    if (registers[Vx] != kk)
    {
        incrementPC();
    }
    incrementPC();
}

void Chip8::Op_5xy0(uint8_t Vx, uint8_t Vy)
{ // [SE Vx, Vy] if reg Vx = reg Vy, increment PC by 2
    if (registers[Vx] == registers[Vy])
    {
        incrementPC();
    }
    incrementPC();
}

void Chip8::Op_6xkk(uint8_t Vx, uint8_t kk)
{ // [LD Vx, byte] Puts value kk into register Vx
    registers[Vx] = kk;
    incrementPC();
}

void Chip8::Op_7xkk(uint8_t Vx, uint8_t kk)
{ // ADD [Vx, byte] Add kk to value in reg Vx, store result in Vx
    uint8_t result = registers[Vx] + kk;
    registers[Vx] = result;
    incrementPC();
}

void Chip8::Op_8xy0(uint8_t Vx, uint8_t Vy){ // [LD Vx Vy] Stores value of reg Vy into Vx
    registers[Vx] = registers[Vy];
    incrementPC();
}

void Chip8::Op_8xy1(uint8_t Vx, uint8_t Vy){ // [Vx OR Vy] Bitwise OR on Vx and Vy, stores result in Vx 
    registers[Vx] = (registers[Vx] | registers[Vy]);
    incrementPC();
}

void Chip8::Op_8xy2(uint8_t Vx, uint8_t Vy){ // [Vx AND Vy] Bitwise AND on Vx and Vy, stores result in Vx 
    registers[Vx] = (registers[Vx] & registers[Vy]);
    incrementPC();
}

void Chip8::Op_8xy3(uint8_t  Vx, uint8_t Vy){ // [Vx XOR Vy] Bitwise XOR on Vx and Vy, stores result in Vx 
    registers[Vx] = (registers[Vx] ^ registers[Vy]);
    incrementPC();
}

void Chip8::Op_8xy4(uint8_t Vx, uint8_t Vy){ // [ADD Vx, Vy] Vx = Vx+Vy, Vf = carry
    uint16_t result = (registers[Vx] + registers[Vy]);
    if (result > 255){
        registers[0x0F] = 1;
    }else{
        registers[0x0F] = 0;
    }
    registers[Vx] = uint8_t(result);
    incrementPC();
}
void Chip8::Op_8xy5(uint8_t Vx, uint8_t Vy){ // [SUB Vx, Vy] Vx = Vx-Vy, Vf = 1 if Vx>Vy, otheriwse 0
    if(registers[Vx] >= registers[Vy]){
        registers[0x0F] = 1;
    }else{
        registers[0x0F] = 0;
    }
    registers[Vx] = (registers[Vx] - registers[Vy]);
    incrementPC();
}

void Chip8::Op_8xy6(uint8_t Vx, uint8_t Vy){ // [SHR Vx {, Vy}] Vx = Vx SHR 1y, Vf = least significant bit
    registers[0x0F] = registers[Vx] & 1;
    registers[Vx] >>= 1;
    incrementPC();
}

void Chip8::Op_8xy7(uint8_t Vx, uint8_t Vy){ // [SUBN Vx, Vy] Vx = Vy - Vx, 
    if(registers[Vy] >= registers[Vx]){
        registers[0x0F] = 1;
    }else{
        registers[0x0F] = 0;
    }
    registers[Vx] = (registers[Vy] - registers[Vx]);
    incrementPC();
}

void Chip8::Op_8xyE(uint8_t Vx, uint8_t Vy){ // [SHL Vx {, Vy}] Vx = Vx SHL 1, 
    registers[0x0F] = registers[Vx] & 0x80;
    registers[Vx] <<= 1;
    incrementPC();
}

void Chip8::Op_9xy0(uint8_t Vx, uint8_t Vy){ // [SNE Vx, Vy], IF reg Vx != reg  Vy, increase PC by 2
    if(registers[Vx] != registers[Vy]){
        incrementPC();
    }
    incrementPC();
}

void Chip8::Op_Annn(uint16_t nnn){ // [Ld I, addr], set register I to nnnn
    I_reg = nnn;
    incrementPC();
}

void Chip8::Op_Bnnn(uint16_t nnn){ // [JP V0, addr], jump to location nnn + Vo
    PC = registers[0] + nnn;
}

void Chip8::Op_Cxkk(uint8_t Vx, uint8_t kk){ // [RND Vx, byte], generates random 8 bit number, AND'd with kk, set ot to Vx
    registers[Vx] = (rand() % 256) & kk;
    incrementPC();
}
//TODO
void Chip8::Op_Dxyn(uint8_t Vx, uint8_t Vy, uint8_t n){ 
    // [DRW Vx, Vy, nibble], Read n bytes from memory starting at I. 
    //Bytes are displayed as sprites at coordinates in reg Vx and Vy.
    //If pixels are erased, Vf is set to 1, otherwise Vf is 0
    //If displayed outside coordinates, wrap around.
    uint8_t x = registers[Vx] % 64;
    uint8_t y = registers[Vy] % 32;
    registers[0x0F] = 0;
    //starts at location I, reads n bytes
    for(int r = 0; r < n; r++){ // rows, 0xFF 0x23 0x82 0x48 0x34 0x67 0x55 0x65 
        uint8_t sprite_hex = memory[I_reg + r];  //0x65, 8 wide, 0110_0101
        for(int b = 7; b > 0; b--){ // bit
            if(((x + b) < 64) && ((y + r) < 32)){ // bounds check
                int list_pos = ((y + r) * 64) + (x+(7-b));
                uint8_t input_bit;
                //screen[list_pos] = sprite_hex ^ // will either be 0x00 or 0xFF
                //cases where xor will erase pixel, ie turn from 1 to 0
                if((sprite_hex & (1 << b)) != 0){
                    input_bit = 0xFF;
                }else{
                    input_bit = 0;
                }
                if((input_bit == 0xFF) && (screen[list_pos] == 0xFF)){
                    registers[0x0F] = 1;
                }
                screen[list_pos] ^= input_bit;
                //ex input  = 0110 0101 & 1000_0000
                //ex screen = 1010_1111
                //0 0 | 0
                //0 1 | 1
                //1 0 | 1
                //1 1 | 0
            }
        }
    }
    incrementPC();
}

void Chip8::Op_Ex9E(uint8_t Vx){ // [SKP Vx] If key coresponding to value of Vx is currently pressed, PC increases by 2
    incrementPC();
}

void Chip8::Op_ExA1(uint8_t Vx){ // [SKNP Vx] If key coresponding to value of Vx is NOT currently pressed, PC increases by 2
    incrementPC();
}

void Chip8::Op_Fx07(uint8_t Vx){ // [LD Vx, DT] Value of DT is placed into VX
    registers[Vx] = delayTimer;
    incrementPC();
}

void Chip8::Op_Fx0A(uint8_t Vx){ // [LD Vx, K] All execution stops until a key is pressed, then the value if stored in Vx
    incrementPC();
}

void Chip8::Op_Fx15(uint8_t Vx){ // [LD Dt, Vx] Set delay timer = Vx
    delayTimer = registers[Vx];
    incrementPC();
}

void Chip8::Op_Fx18(uint8_t Vx){ // [LD St, Vx] Set sound timer = Vx
    soundTimer = registers[Vx];
    incrementPC();
}

void Chip8::Op_Fx1E(uint8_t Vx){ // [Add I, Vx] Values of I and Vx, results stored in I
    I_reg += registers[Vx];
    incrementPC();
}

void Chip8::Op_Fx29(uint8_t Vx){ // [LD F, Vx] I = location of hex sprite for value of reg Vx 
    incrementPC();
}

void Chip8::Op_Fx33(uint8_t Vx){ // [LD B, Vx] I = decimal vlaue of Vx, places hundreds digit in memory at location in I, tens digit at I + 1, ones digit at I + 2 
    uint8_t hundreds_digit = registers[Vx] / 100;
    uint8_t tens_digit = (registers[Vx] - (hundreds_digit * 100)) / 10;
    uint8_t ones_digit = (registers[Vx] - (hundreds_digit * 100)) % 10;
    
    memory[I_reg] = hundreds_digit;
    memory[I_reg + 1] = tens_digit;
    memory[I_reg + 2] = ones_digit; 
    incrementPC();
}

void Chip8::Op_Fx55(uint8_t Vx){ // [LD [I], Vx] Copies values of registers V0 through Vx into memory starting at address I
    for(int i = 0; i <= Vx; i++){
        memory[I_reg + i] = registers[i];
    }
    incrementPC();
}

void Chip8::Op_Fx65(uint8_t Vx){ // [LD Vx, [I]] Puts values from memory starting at location In into registers V0 through Vx
    for(int i = 0; i <= Vx; i++){
        registers[i] = memory[I_reg + i];
    }
    incrementPC();
}