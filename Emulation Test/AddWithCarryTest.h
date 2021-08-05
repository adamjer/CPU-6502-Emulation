#pragma once
#include "BaseTest.h"


class AddWithCarryTest : public BaseTest
{
public:

	struct TestData
	{
		uint8_t values[2];
		uint8_t address[2];
		uint16_t offset;
		uint16_t startAddress;

		bool carryBefore;
		bool expectZ, expectN, expectC, expectV;
		uint8_t answer;
	};

	void TestAbsolute(const TestData&);

	void ExpectUnaffectedRegisters(const CPU&) override;
};

