#pragma once

#include <cstdint>

class Memory
{
public:
    static constexpr uint32_t MAX_MEM = 1024 * 64;
    uint8_t Data[MAX_MEM];

    void Initialize();

    //read 1 byte
    uint8_t operator[] (uint32_t) const;

    //write 1 byte
    uint8_t& operator[] (uint32_t);
};

