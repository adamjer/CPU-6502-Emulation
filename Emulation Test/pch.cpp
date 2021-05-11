//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

void EmulationTest::SetUp()
{
    cpu.Reset(memory);
}

void EmulationTest::Teardown()
{
}
