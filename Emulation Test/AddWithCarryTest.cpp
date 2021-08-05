#include "AddWithCarryTest.h"


void AddWithCarryTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
}


void AddWithCarryTest::TestAbsolute(const TestData& test)
{
    // given:

    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_ABS;
    memory[test.startAddress + 1] = test.address[1];
    memory[test.startAddress + 2] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1]] = test.values[1];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.answer);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    EXPECT_EQ(cpu.Flags.V, test.expectV);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(AddWithCarryTest, ADCCanAddZeroToZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCCanAddCarryAndZeroToZeroAndGetOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 0x01;

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCCanAddTwoUnsignedNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 38;

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCCanAddPositiveAndNegativeNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = static_cast<uint8_t>(-17);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 4;

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCCanAddOneToFFAndCauseCarry)
{
    TestData testData;

    testData.values[0] = 0xFF;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCWillSetNegativeFlagWhenResultIsNegative)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCWillSetOverflowFlagWhenSignedNegativeAdditionFail)
{
    // A: 10000000
    // O: 11111111
    // =: 01111111
    // C: 1 N: 0 V: 1 Z: 0

    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCWillSetOverflowFlagWhenSignedNegativeAdditionPassDueToInitialCarry)
{
    // C: 00000001
    // A: 10000000
    // O: 11111111
    // =: 10000000
    // C: 1 N: 1 V: 0 Z: 0

    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-128);

    TestAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCWillSetOverflowFlagWhenSignedPositiveAdditionFail)
{
    // A: 01111111
    // O: 00000001
    // =: 10000000
    // C: 0 N: 1 V: 1 Z: 0

    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = 1;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestAbsolute(testData);
}

