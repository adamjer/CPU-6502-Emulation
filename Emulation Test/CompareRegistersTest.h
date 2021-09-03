#pragma once
#include "BaseTest.h"

class CompareRegistersTest : public BaseTest
{
public:

	void ExpectUnaffectedRegisters(const CPU&) override;
};

