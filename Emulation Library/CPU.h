#pragma once

#include <cstdio>
#include <vector>
#include "Memory.h"
#include "StatusFlags.h"

class CPU
{
public:
    uint16_t PC; // program counter
    uint8_t SP; // stack pointer

    uint8_t A, X, Y; // registers (accumulator, register X, register Y)

    union // processor state
    {
        StatusFlags Flags;
        uint8_t PS;
    };
    
    // process status bits
    static constexpr uint8_t
        NegativeFlagBit = 0b10000000,
        OverflowFlagBit = 0b01000000;

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
        INS_STA_ABSX = 0x9D,
        INS_STA_ABSY = 0x99,
        INS_STA_INDX = 0x81,
        INS_STA_INDY = 0x91,
        //STX
        INS_STX_ZP = 0x86,
        INS_STX_ABS = 0x8E,
        //STY
        INS_STY_ZP = 0x84,
        INS_STY_ABS = 0x8C,
        INS_STY_ZPX = 0x94,

        INS_TSX = 0xBA,
        INS_TXS = 0x9A,
        INS_PHA = 0x48,
        INS_PHP = 0x08,
        INS_PLA = 0x68,
        INS_PLP = 0x28,

        INS_JMP_ABS = 0x4C,
        INS_JMP_IND = 0x6C,
        INS_JSR = 0x20,
        INS_RTS = 0x60,

        //Logical Operations

        //AND
        INS_AND_IM = 0x29,
        INS_AND_ZP = 0x25,
        INS_AND_ZPX = 0x35,
        INS_AND_ABS = 0x2D,
        INS_AND_ABSX = 0x3D,
        INS_AND_ABSY = 0x39,
        INS_AND_INDX = 0x21,
        INS_AND_INDY = 0x31,

        //OR
        INS_ORA_IM = 0x09,
        INS_ORA_ZP = 0x05,
        INS_ORA_ZPX = 0x15,
        INS_ORA_ABS = 0x0D,
        INS_ORA_ABSX = 0x1D,
        INS_ORA_ABSY = 0x19,
        INS_ORA_INDX = 0x01,
        INS_ORA_INDY = 0x11,

        //EOR
        INS_EOR_IM = 0x49,
        INS_EOR_ZP = 0x45,
        INS_EOR_ZPX = 0x55,
        INS_EOR_ABS = 0x4D,
        INS_EOR_ABSX = 0x5D,
        INS_EOR_ABSY = 0x59,
        INS_EOR_INDX = 0x41,
        INS_EOR_INDY = 0x51,

        //BIT
        INS_BIT_ZP = 0x24,
        INS_BIT_ABS = 0x2C,

        //Transfer Registers
        INS_TAX = 0xAA,
        INS_TAY = 0xA8,
        INS_TXA = 0x8A,
        INS_TYA = 0x98,

        //Increment and decrement
        INS_INX = 0xE8,
        INS_INY = 0xC8,
        INS_DEX = 0xCA,
        INS_DEY = 0x88,
        INS_DEC_ZP = 0xC6,
        INS_DEC_ZPX = 0xD6,
        INS_DEC_ABS = 0xCE,
        INS_DEC_ABSX = 0xDE,
        INS_INC_ZP = 0xE6,
        INS_INC_ZPX = 0xF6,
        INS_INC_ABS = 0xEE,
        INS_INC_ABSX = 0xFE,

        INS_BEQ = 0xF0,

        INS_NOP = 0x70;


    void ResetRegisters();
    void Reset(Memory&);
    void Reset(const uint16_t&, Memory&);
    void Reset(const uint16_t&);
    int32_t Execute(int32_t, Memory&);
    void SetNegativeAndZeroFlags( uint8_t);
    uint16_t LoadProgram(const std::vector<uint8_t>&, Memory&);
    void PrintStatus() const;

    uint8_t FetchByte(int32_t&, const Memory&);   
    uint16_t FetchWord(int32_t&, const Memory&);
    uint8_t ReadByte(int32_t&, const uint16_t&, const Memory&);
    uint16_t ReadWord(int32_t&, const uint16_t&, const Memory&);
    void WriteByte(int32_t&, const uint16_t&, Memory&, const uint8_t);
    void WriteWord(int32_t&, const uint16_t&, Memory&, const uint16_t);
    uint16_t StackPointerToAddress() const;
    void PushProgramCounterToStack(int32_t&, Memory&);
    void PushByteOntoStack(int32_t&, Memory&, uint8_t);
    uint16_t PopWordFromStack(int32_t&, const Memory&);
    uint8_t PopByteFromStack(int32_t&, const Memory&);

    uint16_t AddressZeroPage(int32_t&, const Memory&);
    uint16_t AddressZeroPageX(int32_t&, const Memory&);
    uint16_t AddressZeroPageY(int32_t&, const Memory&);
    uint16_t AddressAbsolute(int32_t&, const Memory&);
    uint16_t AddressAbsoluteX(int32_t&, const Memory&, bool);
    uint16_t AddressAbsoluteY(int32_t&, const Memory&, bool);
    uint16_t AddressIndirectX(int32_t&, const Memory&);
    uint16_t AddressIndirectY(int32_t&, const Memory&, bool);
};

