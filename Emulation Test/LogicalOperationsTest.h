#pragma once
#include "BaseTest.h"

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

    void TestLogicalOperationOnARegisterImmediate(LogicalOperation);
    void TestLogicalOperationZeroPage(LogicalOperation);
    void TestLogicalOperationZeroPageX(LogicalOperation);
    void TestLogicalOperationZeroPageY(LogicalOperation);
    void TestLogicalOperationAbsolute(LogicalOperation);
    void TestLogicalOperationAbsoluteX(LogicalOperation);
    void TestLogicalOperationAbsoluteXWhenCrossingPage(LogicalOperation);
    void TestLogicalOperationAbsoluteY(LogicalOperation);
    void TestLogicalOperationAbsoluteYWhenCrossingPage(LogicalOperation);
    void TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation);
    void TestLogicalOperationIndirectX(LogicalOperation);
    void TestLogicalOperationIndirectY(LogicalOperation);
    void TestLogicalOperationIndirectYWhenItCrossesPage(LogicalOperation);

    void ExpectUnaffectedRegisters(const CPU&) override;
};

