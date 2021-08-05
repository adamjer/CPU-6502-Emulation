#pragma once
#include "BaseTest.h"


class StoreRegisterTest : public BaseTest
{
public:

    virtual void TestStoreRegisterZeroPage(uint8_t, uint8_t CPU::*);
    virtual void TestStoreRegisterAbsolute(uint8_t, uint8_t CPU::*);
    virtual void TestStoreRegisterZeroPageX(uint8_t, uint8_t CPU::*);

    void ExpectUnaffectedRegisters(const CPU&) override;
};

