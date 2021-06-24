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
        //LDA
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_LDA_ZPY = 0xB6,
        INS_LDA_ABS = 0xAD,
        INS_LDA_ABSX = 0xBD,
        INS_LDA_ABSY = 0xB9,
        INS_LDA_INDX = 0xA1,
        INS_LDA_INDY = 0xB1,
        //LDX
        INS_LDX_IM = 0xA2,
        INS_LDX_ZP = 0xA6,
        INS_LDX_ZPY = 0xB6,
        INS_LDX_ABS = 0xAE,
        INS_LDX_ABSY = 0xBE,
        //LDY
        INS_LDY_IM = 0xA0,
        INS_LDY_ZP = 0xA4,
        INS_LDY_ZPX = 0xB4,
        INS_LDY_ABS = 0xAC,
        INS_LDY_ABSX = 0xBC,
        //STA
        INS_STA_ZP = 0x85,
        INS_STA_ABS = 0x8D,
        INS_STA_ZPX = 0x95,
        //STX
        INS_STX_ZP = 0x86,
        INS_STX_ABS = 0x8E,
        INS_STX_ABSX = 0x9D,
        INS_STX_ABSY = 0x99,
        INS_STX_INDX = 0x81,
        INS_STX_INDY = 0x91,
        //STY
        INS_STY_ZP = 0x84,
        INS_STY_ABS = 0x8C,
        INS_STY_ZPX = 0x94,


        INS_JSR = 0x20;


    void ResetRegisters();
    void Reset(Memory&);
    int32_t Execute(int32_t, Memory&);
    void LoadRegisterSetStatus( uint8_t);

    uint8_t FetchByte(int32_t&, Memory&);   
    uint16_t FetchWord(int32_t&, Memory&);
    uint8_t ReadByte(int32_t&, const uint16_t&, const Memory&);
    uint16_t ReadWord(int32_t&, const uint16_t&, const Memory&);
    uint16_t AddressZeroPage(int32_t&, Memory&);
    uint16_t AddressZeroPageX(int32_t&, Memory&);
    uint16_t AddressZeroPageY(int32_t&, Memory&);
    uint16_t AddressAbsolute(int32_t&, Memory&);
    uint16_t AddressAbsoluteX(int32_t&, Memory&);
    uint16_t AddressAbsoluteY(int32_t&, Memory&);
};

