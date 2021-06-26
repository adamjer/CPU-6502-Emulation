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
