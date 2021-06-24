#pragma once
#include "gtest/gtest.h"
#include "..\Emulation Library\Source.h"

static void VerifyUnmodifiedFlagsFromStorageRegister(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
    EXPECT_EQ(cpu.Flags.N, copy.Flags.N);
    EXPECT_EQ(cpu.Flags.Z, copy.Flags.Z);
}

class StoreRegisterTest : public testing::Test
{
public:
    Memory memory;
    CPU cpu;

    virtual void SetUp();
    virtual void Teardown();

    virtual void TestStoreRegisterZeroPage(uint8_t, uint8_t CPU::*);
    virtual void TestStoreRegisterAbsolute(uint8_t, uint8_t CPU::*);
    virtual void TestStoreRegisterZeroPageX(uint8_t, uint8_t CPU::*);
};
