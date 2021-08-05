#pragma once
#include "BaseTest.h"

class LoadRegisterTest : public BaseTest
{
public:  

    virtual void TestLoadRegisterImmediate(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPageY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteXWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteYWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);

    void ExpectUnaffectedRegisters(const CPU&) override;
};