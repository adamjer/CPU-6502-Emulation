#pragma once

#include <cstdint>

class StatusFlags
{
public:

    uint8_t C : 1; //0: CarryFlag C;
    uint8_t Z : 1; //1: ZeroFlag Z;
    uint8_t I : 1; //2: InterruptDisableFlag I;
    uint8_t D : 1; //3: DecimalModeFlag D;
    uint8_t B : 1; //4: BreakCommandFlag B;
    uint8_t Unused : 1; //5: Unused
    uint8_t V : 1; //6: OverflowFlag V;
    uint8_t N : 1; //7: NegativeFlag N;

    void Reset();
    void Set();

    bool operator== (const StatusFlags&) const;
};
