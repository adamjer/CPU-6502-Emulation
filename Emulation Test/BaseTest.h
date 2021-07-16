#pragma once
#include "gtest/gtest.h"
#include "..\Emulation Library\Source.h"

static void VerifyUnmodifiedStatusFlagsFromLoadRegister(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}

static void VerifyUnmodifiedStatusFlagsFromStorageRegister(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.Z, copy.Flags.Z);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
    EXPECT_EQ(cpu.Flags.N, copy.Flags.N);   
}

static void VerfifyUnmodifiedFlagsFromLogicalOperaionInstruction(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}

static void VerifyUnmodifiedStatusFlagsFromTransferRegister(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}


class BaseTest : public testing::Test
{
protected:
    Memory memory;
    CPU cpu;

    virtual void SetUp();
    virtual void Teardown();
};

