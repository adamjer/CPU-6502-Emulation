#pragma once
#include "gtest/gtest.h"
#include "BaseTest.h"
#include "..\Emulation Library\Source.h"

enum class LogicalOperation
{
    And,
    Or,
    Eor
};

class AndEorOraTest : public BaseTest
{
public:
    uint8_t logicalOperation(uint8_t, uint8_t, LogicalOperation);

    virtual void TestLogicalOperationOnARegisterImmediate(LogicalOperation);
    virtual void TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPageY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteXWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteYWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
};

