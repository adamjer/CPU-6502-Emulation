#pragma once
#include "gtest/gtest.h"
#include "..\Emulation Library\Source.h"

class StackOperationsTest : public testing::Test
{
public:
    Memory memory;
    CPU cpu;

    virtual void SetUp();
    virtual void Teardown();
};

