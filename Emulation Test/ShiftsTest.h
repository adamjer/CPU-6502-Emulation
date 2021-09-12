#pragma once
#include "BaseTest.h"


class ShiftsTest : public BaseTest
{
public:
	void ExpectUnaffectedRegisters(const CPU&) override;

};

