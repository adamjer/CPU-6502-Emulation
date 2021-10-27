#pragma once
#include "BaseTest.h"


class AddWithCarryTest : public BaseTest
{
public:

	enum class Operation
	{
		Add, Subtract
	};

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

	void TestADCAbsolute(const TestData&, Operation);
	void TestADCAbsoluteX(const TestData&, Operation);
	void TestADCAbsoluteY(const TestData&, Operation);
	void TestADCImmediate(const TestData&, Operation);
	void TestADCZeroPage(const TestData&, Operation);
	void TestADCZeroPageX(const TestData&, Operation);
	void TestADCIndirectX(const TestData&, Operation);
	void TestADCIndirectY(const TestData&, Operation);
	void TestSBCAbsolute(const TestData&);
	void TestSBCAbsoluteX(const TestData&);
	void TestSBCAbsoluteY(const TestData&);
	void TestSBCImmediate(const TestData&);
	void TestSBCZeroPage(const TestData&);
	void TestSBCZeroPageX(const TestData&);
	void TestSBCIndirectX(const TestData&);
	void TestSBCIndirectY(const TestData&);

	void ExpectUnaffectedRegisters(const CPU&) override;
};

