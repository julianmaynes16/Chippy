#include <cstdint>

class chip8{
    public: 
        uint8_t memory[4096];
        uint8_t register[16];
        char I_reg[2];
        uint8_t delayTimer;
        uint8_t soundTimer;
        uint16_t PC;
        uint8_t SP;


    void 00E0(){  //CLR, Clear display

    }

    void 00EE(){//RET, return from subroutine
    //set program counter to the top opf stack , subtracts one from stack pointer

    }

    void 
}