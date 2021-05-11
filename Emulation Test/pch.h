//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"
#include "..\Emulation Library\Source.h"

class EmulationTest : public testing::Test
{
public:
    Memory memory;
    CPU cpu;

    virtual void SetUp();
    virtual void Teardown();
};
