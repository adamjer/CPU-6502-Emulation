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

class LogicalOperationsTest : public BaseTest
{
public:
    uint8_t logicalOperation(uint8_t, uint8_t, LogicalOperation);

    virtual void TestLogicalOperationOnARegisterImmediate(LogicalOperation);
    virtual void TestLogicalOperationZeroPage(LogicalOperation);
    virtual void TestLogicalOperationZeroPageX(LogicalOperation);
    virtual void TestLogicalOperationZeroPageY(LogicalOperation);
    virtual void TestLogicalOperationAbsolute(LogicalOperation);
    virtual void TestLogicalOperationAbsoluteX(LogicalOperation);
    virtual void TestLogicalOperationAbsoluteXWhenCrossingPage(LogicalOperation);
    virtual void TestLogicalOperationAbsoluteY(LogicalOperation);
    virtual void TestLogicalOperationAbsoluteYWhenCrossingPage(LogicalOperation);
    virtual void TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation);
    virtual void TestLogicalOperationIndirectX(LogicalOperation);
    virtual void TestLogicalOperationIndirectY(LogicalOperation);
    virtual void TestLogicalOperationIndirectYWhenItCrossesPage(LogicalOperation);
};

