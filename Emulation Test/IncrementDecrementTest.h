#pragma once
#include "BaseTest.h"


class IncrementDecrementTest : public BaseTest
{
public:

	void ExpectUnaffectedRegisters(const CPU&) override;
};

