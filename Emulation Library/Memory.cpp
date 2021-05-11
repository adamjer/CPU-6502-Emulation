#include "Memory.h"

void Memory::Initialize()
{
    for (uint32_t i = 0; i < MAX_MEM; ++i)
    {
        this->Data[i] = 0;
    }
}

uint8_t Memory::operator[] (uint32_t address) const
{
    // assert here address is < MAX_MEM
    return this->Data[address];
}

uint8_t& Memory::operator[](uint32_t address)
{
    // assert here address is < MAX_MEM
    return this->Data[address];
}

void Memory::WriteWord(int32_t& cycles, const uint16_t& value, const uint32_t& address)
{
    this->Data[address] = value & 0xFF;
    this->Data[address + 1] = (value >> 8);
    cycles -= 2;
}
