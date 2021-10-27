#include "AddWithCarryTest.h"


void AddWithCarryTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
}


void AddWithCarryTest::TestSBCAbsolute(const TestData& test)
{
    TestADCAbsolute(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCAbsolute(const TestData& test, Operation operation = Operation::Add)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_ABS : CPU::INS_SBC_ABS;
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


TEST_F(AddWithCarryTest, ADCAbsoluteCanAddZeroToZeroAndGetZero)
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


TEST_F(AddWithCarryTest, ADCAbsoluteCanAddCarryAndZeroToZeroAndGetOne)
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


TEST_F(AddWithCarryTest, ADCAbsoluteCanAddTwoUnsignedNumbers)
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


TEST_F(AddWithCarryTest, ADCAbsoluteCanAddTwoUnsignedNumbersWithCarry)
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


TEST_F(AddWithCarryTest, ADCAbsoluteCanAddPositiveAndNegativeNumbersWithCarry)
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


TEST_F(AddWithCarryTest, ADCAbsoluteCanAddOneToFFAndCauseCarry)
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


TEST_F(AddWithCarryTest, ADCAbsoluteWillSetNegativeFlagWhenResultIsNegative)
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


TEST_F(AddWithCarryTest, ADCAbsoluteWillSetOverflowFlagWhenSignedNegativeAdditionFail)
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


TEST_F(AddWithCarryTest, ADCAbsoluteWillSetOverflowFlagWhenSignedNegativeAdditionPassDueToInitialCarry)
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


TEST_F(AddWithCarryTest, ADCAbsoluteWillSetOverflowFlagWhenSignedPositiveAdditionFail)
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


void AddWithCarryTest::TestSBCImmediate(const TestData& test)
{
    TestADCImmediate(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCImmediate(const TestData& test, Operation operation = Operation::Add)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC : CPU::INS_SBC;
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


void AddWithCarryTest::TestSBCZeroPage(const TestData& test)
{
    TestADCZeroPage(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCZeroPage(const TestData& test, Operation operation = Operation::Add)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = test.carryBefore;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    cpu.Flags.V = !test.expectV;
    // start - inline a little program
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_ZP : CPU::INS_SBC_ZP;
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


void AddWithCarryTest::TestSBCZeroPageX(const TestData& test)
{
    TestADCZeroPageX(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCZeroPageX(const TestData& test, Operation operation = Operation::Add)
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
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_ZPX : CPU::INS_SBC_ZPX;
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


void AddWithCarryTest::TestSBCAbsoluteX(const TestData& test)
{
    TestADCAbsoluteX(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCAbsoluteX(const TestData& test, Operation operation = Operation::Add)
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
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_ABSX : CPU::INS_SBC_ABSX;
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


void AddWithCarryTest::TestSBCAbsoluteY(const TestData& test)
{
    TestADCAbsoluteY(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCAbsoluteY(const TestData& test, Operation operation = Operation::Add)
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
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_ABSY : CPU::INS_SBC_ABSY;
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


void AddWithCarryTest::TestSBCIndirectX(const TestData& test)
{
    TestADCIndirectX(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCIndirectX(const TestData& test, Operation operation = Operation::Add)
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
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_INDX : CPU::INS_SBC_INDX;
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


void AddWithCarryTest::TestSBCIndirectY(const TestData& test)
{
    TestADCIndirectY(test, Operation::Subtract);
}


void AddWithCarryTest::TestADCIndirectY(const TestData& test, Operation operation = Operation::Add)
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
    memory[test.startAddress] = (operation == Operation::Add) ? CPU::INS_ADC_INDY : CPU::INS_SBC_INDY;
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


//Substractions tests
TEST_F(AddWithCarryTest, SBCImmediateCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCImmediateCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCImmediate(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCZeroPage(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCZeroPageXCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCZeroPageX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCAbsolute(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteXCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCAbsoluteX(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCAbsoluteYCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCAbsoluteY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectXCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCIndirectX(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractZeroFromZeroAndGetZero)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x00;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = true;
    testData.answer = 0x00;

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractZeroFromZeroAndCarryAndGetMinusOne)
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
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractOneFromZeroAndGetMinusOne)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-1);

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractOneFromZeroWithCarryAndGetMinusTwo)
{
    TestData testData;

    testData.values[0] = 0x00;
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = false;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = static_cast<uint8_t>(-2);

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractTwoNegativeNumbersAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = static_cast<uint8_t>(-128);
    testData.values[1] = 0x01;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 127;

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractAPositiveAndNegativeNumberAndGetSignedOverflow)
{
    TestData testData;

    testData.values[0] = 127;
    testData.values[1] = static_cast<uint8_t>(-1);
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = true;
    testData.expectZ = false;
    testData.answer = 128;

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractTwoPositiveNumbers)
{
    TestData testData;

    testData.values[0] = 65;
    testData.values[1] = 13;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = true;
    testData.expectN = false;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = 52;

    TestSBCIndirectY(testData);
}


TEST_F(AddWithCarryTest, SBCIndirectYCanSubtractTwoNegativeNumbers)
{
    TestData testData;

    testData.values[0] = -20;
    testData.values[1] = -17;
    testData.address[0] = 0x80;
    testData.address[1] = 0x00;
    testData.offset = 0x01;
    testData.startAddress = 0xFF00;
    testData.carryBefore = true;
    testData.expectC = false;
    testData.expectN = true;
    testData.expectV = false;
    testData.expectZ = false;
    testData.answer = -3;

    TestSBCIndirectY(testData);
}