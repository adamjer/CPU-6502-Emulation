#pragma once

#include <cstdio>
#include "Memory.h"
#include "Flags.h"

class CPU
{
public:
    uint16_t PC; // program counter
    uint16_t SP; // stack pointer

    uint8_t A, X, Y; // registers (accumulator, register X, register Y)

    Flags Flags;
    
    // opcodes
    static constexpr uint8_t
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_LDA_ABS = 0xAD,
        INS_LDA_ABSX = 0xBD,
        INS_LDA_ABSY = 0xB9,
        INS_LDA_INDX = 0xA1,
        INS_LDA_INDY = 0xB1,
        INS_JSR = 0x20;


    void ResetRegisters();
    void Reset(Memory&);
    int32_t Execute(int32_t, Memory&);
    void LDASetStatus();

    uint8_t FetchByte(int32_t&, Memory&);   
    uint16_t FetchWord(int32_t&, Memory&);
    uint8_t ReadByte(int32_t&, const uint16_t&, const Memory&);
    uint16_t ReadWord(int32_t&, const uint16_t&, const Memory&);
};

