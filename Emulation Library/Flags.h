#pragma once

#include <cstdint>

class Flags
{
public:

    uint8_t C : 1; // CarryFlag C;
    uint8_t Z : 1; // ZeroFlag Z;
    uint8_t I : 1; // InterruptDisableFlag I;
    uint8_t D : 1; // DecimalModeFlag D;
    uint8_t B : 1; // BreakCommandFlag B;
    uint8_t V : 1; // OverflowFlag V;
    uint8_t N : 1; // NegativeFlag N;

    void Reset();
    void Set();
};
