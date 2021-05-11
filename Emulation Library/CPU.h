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
        INS_JSR = 0x20;


    void ResetRegisters();
    void Reset(Memory&);
    int32_t Execute(uint32_t, Memory&);
    void LDASetStatus();

    uint8_t FetchByte(uint32_t&, Memory&);   
    uint16_t FetchWord(uint32_t&, Memory&);
    uint8_t Read(uint32_t&, uint8_t&, Memory&);
};

