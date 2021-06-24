#pragma once
#include "gtest/gtest.h"
#include "..\Emulation Library\Source.h"

static void VerifyUnmodifiedFlagsFromLoadRegister(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}

class LoadRegisterTest : public testing::Test
{
public:
    Memory memory;
    CPU cpu;

    virtual void SetUp();
    virtual void Teardown();

    virtual void TestLoadRegisterImmediate(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterZeroPageY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteXWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
    virtual void TestLoadRegisterAbsoluteYWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest);
};