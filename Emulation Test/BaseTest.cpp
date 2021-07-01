#include "BaseTest.h"

void BaseTest::SetUp()
{
    cpu.Reset(memory);
}

void BaseTest::Teardown()
{
}
