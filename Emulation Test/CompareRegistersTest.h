#pragma once
#include "BaseTest.h"

class CompareRegistersTest : public BaseTest
{
public:

	struct TestData
	{
		uint8_t values[2];
		uint8_t address[2];
		uint16_t offset;
		uint16_t startAddress;

		bool expectZ, expectN, expectC;
	};

	TestData CompareTwoIdenticalValues();
	TestData CompareTwoDifferentPositiveValues();
	TestData CompareANegativeNumberToAPositive();
	TestData CompareTwoValuesThatResultInANegative();

	void ExpectUnaffectedRegisters(const CPU&) override;
	void CMPImmediate(const TestData&);
	void CMPZeroPage(const TestData&);
	void CMPZeroPageX(const TestData&);
	void CMPAbsolute(const TestData&);
	void CMPAbsoluteX(const TestData&);
	void CMPAbsoluteY(const TestData&);
	void CMPIndirectX(const TestData&);
	void CMPIndirectY(const TestData&);
};

