#include "BaseTest.h"


void BaseTest::SetUp()
{
    cpu.Reset(memory);
}


void BaseTest::Teardown()
{
}


void BaseTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    
}
