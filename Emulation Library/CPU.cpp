#include "CPU.h"

void CPU::ResetRegisters()
{
    A = X = Y = 0;
}

void CPU::Reset(Memory& memory)
{
    this->PC = 0xFFFC;
    this->SP = 0x0100;
    this->Flags.Reset();
    ResetRegisters();

    memory.Initialize();
}

uint8_t CPU::FetchByte(uint32_t& cycles, Memory& memory)
{
    uint8_t data = memory[PC++];
    --cycles;

    return data;
}

uint16_t CPU::FetchWord(uint32_t& cycles, Memory& memory)
{
    // CPU 6502 is little endian
    // first bute is more signifant one (first in WORD)
    uint16_t data = memory[PC++];
    data |= (memory[PC++] << 8);
    cycles -= 2;

    // if you wanted to handle endianess
    // you would have to swap bytes here
    //     if (PLATFORM_BIG_ENDIAN)
    //         SwapBytesInWord(data);

    return data;
}

uint8_t CPU::Read(uint32_t& cycles, uint8_t& address, Memory& memory)
{
    uint8_t data = memory[address];
    --cycles;

    return data;
}

//return the number of cycles that were used
int32_t CPU::Execute(uint32_t cycles, Memory& memory)
{
    const uint32_t cyclesRequested = cycles;
    while (cycles > 0)
    {
        uint8_t instruction = this->FetchByte(cycles, memory);

        switch (instruction)
        {
            case INS_LDA_IM:
            {
                uint8_t value = this->FetchByte(cycles, memory);
                A = value;
                LDASetStatus();
            } break;
            case INS_LDA_ZP:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                A = Read(cycles, zeroPageAddress, memory);
                LDASetStatus();
            }break;
            case INS_LDA_ZPX:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                zeroPageAddress += X;
                --cycles;
                A = Read(cycles, zeroPageAddress, memory);
                LDASetStatus();
            }break;
            case INS_JSR:
            {
                uint16_t subroutineAddress = this->FetchWord(cycles, memory);
                memory.WriteWord(cycles, PC - 1, SP++);
                SP += 2;
                PC = subroutineAddress;
                --cycles;

                // flags not affected
            }break;
            default:
            {
                printf("Instruction not handled!\n");
            } break;
        }
    }

    const int32_t cyclesUsed = cyclesRequested - cycles;
    return cyclesUsed;
}

void CPU::LDASetStatus()
{
    Flags.Z = (A == 0);
    Flags.N = (A & 0b10000000) > 0;
}

