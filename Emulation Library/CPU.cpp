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

/*  Addresing mode - Zero page*/
uint16_t CPU::AddressZeroPage(int32_t& cycles, Memory& memory)
{
    uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
    return zeroPageAddress;
}

/*  Addresing mode - Zero page with X offset*/
uint16_t CPU::AddressZeroPageX(int32_t& cycles, Memory& memory)
{
    uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
    zeroPageAddress += X;
    --cycles;
    return zeroPageAddress;
}

/*  Addresing mode - Zero page with Y offset*/
uint16_t CPU::AddressZeroPageY(int32_t& cycles, Memory& memory)
{
    uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
    zeroPageAddress += Y;
    --cycles;
    return zeroPageAddress;
}

/*  Addresing mode - Absolute*/
uint16_t CPU::AddressAbsolute(int32_t& cycles, Memory& memory)
{
    uint16_t absoluteAddress = this->FetchWord(cycles, memory);
    return absoluteAddress;
}

/*  Addresing mode - Absolute with X offset*/
uint16_t CPU::AddressAbsoluteX(int32_t& cycles, Memory& memory)
{
    uint16_t absoluteAddress = this->FetchWord(cycles, memory);
    uint16_t absoluteAddressX = absoluteAddress + X;
    if (absoluteAddressX - absoluteAddress >= 0xFF)
        --cycles;

    return absoluteAddressX;
}

/*  Addresing mode - Absolute with Y offset*/
uint16_t CPU::AddressAbsoluteY(int32_t& cycles, Memory& memory)
{
    uint16_t absoluteAddress = this->FetchWord(cycles, memory);
    uint16_t absoluteAddressY = absoluteAddress + Y;
    if (absoluteAddressY - absoluteAddress >= 0xFF)
        --cycles;

    return absoluteAddressY;
}


//return the number of cycles that were used
int32_t CPU::Execute(int32_t cycles, Memory& memory)
{
    //Load a register with the value from memory address
    auto LoadRegister = [&cycles, &memory, this](uint16_t address, uint8_t& registry)
    {
        registry = this->ReadByte(cycles, address, memory);
        LoadRegisterSetStatus(registry);
    };

    const uint32_t cyclesRequested = cycles;
    while (cycles > 0)
    {
        uint8_t instruction = this->FetchByte(cycles, memory);

        switch (instruction)
        {
            case INS_LDA_IM:
            {
                A = this->FetchByte(cycles, memory);
                LoadRegisterSetStatus(A);
            } break;
            case INS_LDX_IM:
            {
                X = this->FetchByte(cycles, memory);
                LoadRegisterSetStatus(X);
            } break;
            case INS_LDY_IM:
            {
                Y = this->FetchByte(cycles, memory);
                LoadRegisterSetStatus(Y);
            } break;
            case INS_LDA_ZP:
            {
                uint16_t address = this->AddressZeroPage(cycles, memory);
                LoadRegister(address, A);
            }break;
            case INS_LDX_ZP:
            {
                uint16_t address = this->AddressZeroPage(cycles, memory);
                LoadRegister(address, X);
            }break;
            case INS_LDY_ZP:
            {
                uint16_t address = this->AddressZeroPage(cycles, memory);
                LoadRegister(address, Y);
            }break;
            case INS_LDA_ZPX:
            {
                uint16_t address = this->AddressZeroPageX(cycles, memory);
                LoadRegister(address, A);
            }break;
            case INS_LDX_ZPY:
            {
                uint16_t address = this->AddressZeroPageY(cycles, memory);
                LoadRegister(address, X);
            }break;
            case INS_LDY_ZPX:
            {
                uint16_t address = this->AddressZeroPageX(cycles, memory);
                LoadRegister(address, Y);
            }break;
            case INS_LDA_ABS:
            {
                uint16_t address = this->AddressAbsolute(cycles, memory);
                LoadRegister(address, A);
            }break;
            case INS_LDX_ABS:
            {
                uint16_t address = this->AddressAbsolute(cycles, memory);
                LoadRegister(address, X);
            }break;
            case INS_LDY_ABS:
            {
                uint16_t address = this->AddressAbsolute(cycles, memory);
                LoadRegister(address, Y);
            }break;
            case INS_LDA_ABSX:
            {
                uint16_t address = this->AddressAbsoluteX(cycles, memory);
                LoadRegister(address, A);
            }break;
            case INS_LDY_ABSX:
            {
                uint16_t address = this->AddressAbsoluteX(cycles, memory);
                LoadRegister(address, Y);
            }break;
            case INS_LDA_ABSY:
            {
                uint16_t address = this->AddressAbsoluteY(cycles, memory);
                LoadRegister(address, A);
            }break;
            case INS_LDX_ABSY:
            {
                uint16_t address = this->AddressAbsoluteY(cycles, memory);
                LoadRegister(address, X);
            }break;
            case INS_LDA_INDX:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                zeroPageAddress += X;
                --cycles;
                uint16_t effectiveAddress = this->ReadWord(cycles, zeroPageAddress, memory);
                LoadRegister(effectiveAddress, A);
            }break;
            case INS_LDA_INDY:
            {
                uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
                uint16_t effectiveAddress = this->ReadWord(cycles, zeroPageAddress, memory);
                uint16_t effectiveAddressY = effectiveAddress + Y;
                if (effectiveAddressY - effectiveAddress >= 0xFF)
                    --cycles;
                LoadRegister(effectiveAddressY, A);
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

/**  Sets the correct Process status after a load register instruction
*    LDA, LDX, LDY
*    @reg A,X or Y Register*/
void CPU::LoadRegisterSetStatus( uint8_t reg)
{
    Flags.Z = (reg == 0);
    Flags.N = (reg & 0b10000000) > 0;
}

