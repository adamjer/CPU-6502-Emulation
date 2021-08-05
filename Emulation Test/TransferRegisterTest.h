#pragma once
#include "BaseTest.h"

class TransferRegisterTest : public BaseTest
{
public:

	void ExpectUnaffectedRegisters(const CPU&) override;
};

