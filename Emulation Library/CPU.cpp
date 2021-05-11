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

uint8_t CPU::FetchByte(int32_t& cycles, Memory& memory)
{
    uint8_t data = memory[PC++];
    --cycles;

    return data;
}

uint16_t CPU::FetchWord(int32_t& cycles, Memory& memory)
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

uint8_t CPU::ReadByte(int32_t& cycles, const uint16_t& address, const Memory& memory)
{
    uint8_t data = memory[address];
    --cycles;

    return data;
}

uint16_t CPU::ReadWord(int32_t& cycles, const uint16_t& address, const Memory& memory)
{
    uint8_t lowByte = ReadByte(cycles, address, memory);
    uint8_t highByte = ReadByte(cycles, address + 1, memory);

    return lowByte | (highByte << 8);
}

//return the number of cycles that were used
int32_t CPU::Execute(int32_t cycles, Memory& memory)
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
                A = this->ReadByte(cycles, zeroPageAddress, memory);
                LDASetStatus();
            }break;
            case INS_LDA_ZPX:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                zeroPageAddress += X;
                --cycles;
                A = ReadByte(cycles, zeroPageAddress, memory);
                LDASetStatus();
            }break;
            case INS_LDA_ABS:
            {
                uint16_t absoluteAddress = this->FetchWord(cycles, memory);
                A = this->ReadByte(cycles, absoluteAddress, memory);
            }break;
            case INS_LDA_ABSX:
            {
                uint16_t absoluteAddress = this->FetchWord(cycles, memory);
                uint16_t absoluteAddressX = absoluteAddress + X;
                A = this->ReadByte(cycles, absoluteAddressX, memory);
                if (absoluteAddressX - absoluteAddress >= 0xFF)
                    --cycles;
            }break;
            case INS_LDA_ABSY:
            {
                uint16_t absoluteAddress = this->FetchWord(cycles, memory);
                uint16_t absoluteAddressY = absoluteAddress + Y;
                A = this->ReadByte(cycles, absoluteAddressY, memory);
                if (absoluteAddressY - absoluteAddress >= 0xFF)
                    --cycles;
            }break;
            case INS_LDA_INDX:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                zeroPageAddress += X;
                --cycles;
                uint16_t effectiveAddress = this->ReadWord(cycles, zeroPageAddress, memory);
                A = this->ReadByte(cycles, effectiveAddress, memory);
            }break;
            case INS_LDA_INDY:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                uint16_t effectiveAddress = this->ReadWord(cycles, zeroPageAddress, memory);
                uint16_t effectiveAddressY = effectiveAddress + Y;
                A = this->ReadByte(cycles, effectiveAddressY, memory);
                if (effectiveAddressY - effectiveAddress >= 0xFF)
                    --cycles;
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
                throw -1;
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

