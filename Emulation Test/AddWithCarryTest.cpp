#include "AddWithCarryTest.h"


void AddWithCarryTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
}


void AddWithCarryTest::TestADCAbsolute(const TestData& test)
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


TEST_F(AddWithCarryTest, ADCAbsCanAddZeroToZeroAndGetZero)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsCanAddCarryAndZeroToZeroAndGetOne)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsCanAddTwoUnsignedNumbers)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 37;

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsCanAddTwoUnsignedNumbersWithCarry)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsCanAddPositiveAndNegativeNumbersWithCarry)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsCanAddOneToFFAndCauseCarry)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsWillSetNegativeFlagWhenResultIsNegative)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsWillSetOverflowFlagWhenSignedNegativeAdditionFail)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsWillSetOverflowFlagWhenSignedNegativeAdditionPassDueToInitialCarry)
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

    TestADCAbsolute(testData);
}


TEST_F(AddWithCarryTest, ADCAbsWillSetOverflowFlagWhenSignedPositiveAdditionFail)
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

    TestADCAbsolute(testData);
}


void AddWithCarryTest::TestADCImmediate(const TestData& test)
{
    // given:

    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC;
    memory[test.startAddress + 1] = test.values[1];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
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


TEST_F(AddWithCarryTest, ADCImmediateCanAddTwoUnsignedNumbersWithCarry)
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

    TestADCImmediate(testData);
}


TEST_F(AddWithCarryTest, ADCImmediateCanAddPositiveAndNegativeNumbersWithCarry)
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

    TestADCImmediate(testData);
}


void AddWithCarryTest::TestADCZeroPage(const TestData& test)
{
    // given:

    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_ZP;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0]] = test.values[1];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
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


TEST_F(AddWithCarryTest, ADCZeroPageCanAddTwoUnsignedNumbersWithCarry)
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

    TestADCZeroPage(testData);
}


TEST_F(AddWithCarryTest, ADCZeroPageCanAddPositiveAndNegativeNumbersWithCarry)
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

    TestADCZeroPage(testData);
}


void AddWithCarryTest::TestADCZeroPageX(const TestData& test)
{
    // given:

    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.X = test.offset;
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_ZPX;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0] + cpu.X] = test.values[1];
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


TEST_F(AddWithCarryTest, ADCZeroPageXCanAddTwoUnsignedNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x05;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 38;

    TestADCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, ADCZeroPageXCanAddPositiveAndNegativeNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = static_cast<uint8_t>(-17);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x15;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 4;

    TestADCZeroPageX(testData);
}


void AddWithCarryTest::TestADCAbsoluteX(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.X = test.offset;
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_ABSX;
    memory[test.startAddress + 1] = test.address[1];
    memory[test.startAddress + 2] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1] + cpu.X] = test.values[1];

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


TEST_F(AddWithCarryTest, ADCAbsoluteXCanAddTwoUnsignedNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x05;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 38;

    TestADCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, ADCAbsoluteXCanAddPositiveAndNegativeNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = static_cast<uint8_t>(-17);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x15;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 4;

    TestADCAbsoluteX(testData);
}


void AddWithCarryTest::TestADCAbsoluteY(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Y = test.offset;
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_ABSY;
    memory[test.startAddress + 1] = test.address[1];
    memory[test.startAddress + 2] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1] + cpu.Y] = test.values[1];

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


TEST_F(AddWithCarryTest, ADCAbsoluteYCanAddTwoUnsignedNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x05;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 38;

    TestADCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, ADCAbsoluteYCanAddPositiveAndNegativeNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = static_cast<uint8_t>(-17);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x15;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 4;

    TestADCAbsoluteY(testData);
}


void AddWithCarryTest::TestADCIndirectX(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.X = 0x10;
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_INDX;
    memory[test.startAddress + 1] = test.offset;
    memory[cpu.X + test.offset] = test.address[1];
    memory[cpu.X + test.offset + 1] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
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


TEST_F(AddWithCarryTest, ADCIndirectXCanAddTwoUnsignedNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x05;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 38;

    TestADCIndirectX(testData);
}


TEST_F(AddWithCarryTest, ADCIndirectXCanAddPositiveAndNegativeNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = static_cast<uint8_t>(-17);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x15;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 4;

    TestADCIndirectX(testData);
}


void AddWithCarryTest::TestADCIndirectY(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Y = 0x10;
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_ADC_INDY;
    memory[test.startAddress + 1] = test.offset;
    memory[test.offset] = test.address[1];
    memory[test.offset + 1] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1] + cpu.Y] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
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


TEST_F(AddWithCarryTest, ADCIndirectYCanAddTwoUnsignedNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = 17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x05;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 38;

    TestADCIndirectY(testData);
}


TEST_F(AddWithCarryTest, ADCIndirectYCanAddPositiveAndNegativeNumbersWithCarry)
{
    TestData testData;

    testData.values[0] = 20;
    testData.values[1] = static_cast<uint8_t>(-17);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x15;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 4;

    TestADCIndirectY(testData);
}