#include "CompareRegistersTest.h"


void CompareRegistersTest::ExpectUnaffectedRegisters(const CPU& copy)
{
	EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
	EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
	EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
	EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}


CompareRegistersTest::TestData CompareRegistersTest::CompareTwoIdenticalValues()
{
    TestData testData;
    testData.startAddress = 0xFF00;
    testData.offset = 0x42;
    testData.address[0] = 0x80;
    testData.address[1] = 0x02;
    testData.values[0] = 0x1A;
    testData.values[1] = 0x1A;

    testData.expectC = true;
    testData.expectZ = true;
    testData.expectN = false;

    return testData;
}


CompareRegistersTest::TestData CompareRegistersTest::CompareTwoDifferentPositiveValues()
{
    TestData testData;
    testData.startAddress = 0xFF00;
    testData.offset = 0x42;
    testData.address[0] = 0x80;
    testData.address[1] = 0x02;
    testData.values[0] = 0x1A;
    testData.values[1] = 0x30;

    testData.expectC = false;
    testData.expectZ = false;
    testData.expectN = true;

    return testData;
}


CompareRegistersTest::TestData CompareRegistersTest::CompareANegativeNumberToAPositive()
{
    TestData testData;
    testData.startAddress = 0xFF00;
    testData.offset = 0x42;
    testData.address[0] = 0x80;
    testData.address[1] = 0x02;
    testData.values[0] = 0x1A;
    testData.values[1] = 0x82; // 0x82 is negative number

    testData.expectC = false;
    testData.expectZ = false;
    testData.expectN = true;

    return testData;
}


CompareRegistersTest::TestData CompareRegistersTest::CompareTwoValuesThatResultInANegative()
{
    TestData testData;
    testData.startAddress = 0xFF00;
    testData.offset = 0x42;
    testData.address[0] = 0x80;
    testData.address[1] = 0x02;
    testData.values[0] = 0x1A;
    testData.values[1] = 0x08;

    testData.expectC = true;
    testData.expectZ = false;
    testData.expectN = false;

    return testData;
}


void CompareRegistersTest::CMPImmediate(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP;
    memory[test.startAddress + 1] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareTwoIdenticalValues)
{
    CMPImmediate(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareTwoDifferentPositiveValues)
{
    CMPImmediate(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareANegativeNumberToAPositive)
{
    CMPImmediate(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareTwoValuesThatResultInANegative)
{
    CMPImmediate(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPZeroPage(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_ZP;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPZeroPageCanCompareTwoIdenticalValues)
{
    CMPZeroPage(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPZeroPageCanCompareTwoDifferentPositiveValues)
{
    CMPZeroPage(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPZeroPageCanCompareANegativeNumberToAPositive)
{
    CMPZeroPage(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPZeroPageCanCompareTwoValuesThatResultInANegative)
{
    CMPZeroPage(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPZeroPageX(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.X = test.offset;
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_ZPX;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0] + cpu.X] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPZeroPageXCanCompareTwoIdenticalValues)
{
    CMPZeroPageX(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPZeroPageXCanCompareTwoDifferentPositiveValues)
{
    CMPZeroPageX(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPZeroPageXCanCompareANegativeNumberToAPositive)
{
    CMPZeroPageX(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPZeroPageXCanCompareTwoValuesThatResultInANegative)
{
    CMPZeroPageX(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPAbsolute(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_ABS;
    memory[test.startAddress + 1] = test.address[1];
    memory[test.startAddress + 2] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPAbsoluteCanCompareTwoIdenticalValues)
{
    CMPAbsolute(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPAbsoluteCanCompareTwoDifferentPositiveValues)
{
    CMPAbsolute(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPAbsoluteCanCompareANegativeNumberToAPositive)
{
    CMPAbsolute(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPAbsoluteCanCompareTwoValuesThatResultInANegative)
{
    CMPAbsolute(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPAbsoluteX(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.X = test.offset;
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_ABSX;
    memory[test.startAddress + 1] = test.address[1];
    memory[test.startAddress + 2] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1] + cpu.X] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPAbsoluteXCanCompareTwoIdenticalValues)
{
    CMPAbsoluteX(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPAbsoluteXCanCompareTwoDifferentPositiveValues)
{
    CMPAbsoluteX(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPAbsoluteXCanCompareANegativeNumberToAPositive)
{
    CMPAbsoluteX(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPAbsoluteXCanCompareTwoValuesThatResultInANegative)
{
    CMPAbsoluteX(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPAbsoluteY(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Y = test.offset;
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_ABSY;
    memory[test.startAddress + 1] = test.address[1];
    memory[test.startAddress + 2] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1] + cpu.Y] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPAbsoluteYCanCompareTwoIdenticalValues)
{
    CMPAbsoluteY(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPAbsoluteYCanCompareTwoDifferentPositiveValues)
{
    CMPAbsoluteY(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPAbsoluteYCanCompareANegativeNumberToAPositive)
{
    CMPAbsoluteY(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPAbsoluteYCanCompareTwoValuesThatResultInANegative)
{
    CMPAbsoluteY(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPIndirectX(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.X = 0x10;
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;

    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_INDX;
    memory[test.startAddress + 1] = test.offset;
    memory[test.offset + cpu.X] = test.address[1];
    memory[test.offset + 1 + cpu.X] = test.address[0];
    memory[(test.address[0] << 8) + test.address[1]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPIndirectXCanCompareTwoIdenticalValues)
{
    CMPIndirectX(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPIndirectXCanCompareTwoDifferentPositiveValues)
{
    CMPIndirectX(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPIndirectXCanCompareANegativeNumberToAPositive)
{
    CMPIndirectX(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPIndirectXCanCompareTwoValuesThatResultInANegative)
{
    CMPIndirectX(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPIndirectY(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.A = test.values[0];
    cpu.Y = 0x10;
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;

    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CMP_INDY;
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
    EXPECT_EQ(cpu.A, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPIndirectYCanCompareTwoIdenticalValues)
{
    CMPIndirectY(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPIndirectYCanCompareTwoDifferentPositiveValues)
{
    CMPIndirectY(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPIndirectYCanCompareANegativeNumberToAPositive)
{
    CMPIndirectY(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPIndirectYCanCompareTwoValuesThatResultInANegative)
{
    CMPIndirectY(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPXImmediate(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.X = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CPX;
    memory[test.startAddress + 1] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPXImmediateCanCompareTwoIdenticalValues)
{
    CMPXImmediate(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPXImmediateCanCompareTwoDifferentPositiveValues)
{
    CMPXImmediate(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPXImmediateCanCompareANegativeNumberToAPositive)
{
    CMPXImmediate(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPXImmediateCanCompareTwoValuesThatResultInANegative)
{
    CMPXImmediate(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPYImmediate(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.Y = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CPY;
    memory[test.startAddress + 1] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPYImmediateCanCompareTwoIdenticalValues)
{
    CMPYImmediate(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPYImmediateCanCompareTwoDifferentPositiveValues)
{
    CMPYImmediate(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPYImmediateCanCompareANegativeNumberToAPositive)
{
    CMPYImmediate(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPYImmediateCanCompareTwoValuesThatResultInANegative)
{
    CMPYImmediate(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPXZeroPage(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.X = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CPX_ZP;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPXZeroPageCanCompareTwoIdenticalValues)
{
    CMPXZeroPage(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPXZeroPageCanCompareTwoDifferentPositiveValues)
{
    CMPXZeroPage(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPXZeroPageCanCompareANegativeNumberToAPositive)
{
    CMPXZeroPage(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPXZeroPageCanCompareTwoValuesThatResultInANegative)
{
    CMPXZeroPage(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPYZeroPage(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.Y = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CPY_ZP;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPYZeroPageCanCompareTwoIdenticalValues)
{
    CMPYZeroPage(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPYZeroPageCanCompareTwoDifferentPositiveValues)
{
    CMPYZeroPage(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPYZeroPageCanCompareANegativeNumberToAPositive)
{
    CMPYZeroPage(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPYZeroPageCanCompareTwoValuesThatResultInANegative)
{
    CMPYZeroPage(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPXAbsolute(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.X = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CPX_ABS;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPXAbsoluteCanCompareTwoIdenticalValues)
{
    CMPXAbsolute(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPXAbsoluteCanCompareTwoDifferentPositiveValues)
{
    CMPXAbsolute(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPXAbsoluteCanCompareANegativeNumberToAPositive)
{
    CMPXAbsolute(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPXAbsoluteCanCompareTwoValuesThatResultInANegative)
{
    CMPXAbsolute(CompareTwoValuesThatResultInANegative());
}


void CompareRegistersTest::CMPYAbsolute(const TestData& test)
{
    // given:
    cpu.Reset(test.startAddress, memory);
    cpu.Y = test.values[0];
    cpu.Flags.C = !test.expectC;
    cpu.Flags.Z = !test.expectZ;
    cpu.Flags.N = !test.expectN;
    // start - inline a little program
    memory[test.startAddress] = CPU::INS_CPY_ABS;
    memory[test.startAddress + 1] = test.address[0];
    memory[test.address[0]] = test.values[1];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, test.values[0]);
    EXPECT_EQ(cpu.Flags.C, test.expectC);
    EXPECT_EQ(cpu.Flags.Z, test.expectZ);
    EXPECT_EQ(cpu.Flags.N, test.expectN);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPYAbsoluteCanCompareTwoIdenticalValues)
{
    CMPYAbsolute(CompareTwoIdenticalValues());
}


TEST_F(CompareRegistersTest, CMPYAbsoluteCanCompareTwoDifferentPositiveValues)
{
    CMPYAbsolute(CompareTwoDifferentPositiveValues());
}


TEST_F(CompareRegistersTest, CMPYAbsoluteCanCompareANegativeNumberToAPositive)
{
    CMPYAbsolute(CompareANegativeNumberToAPositive());
}


TEST_F(CompareRegistersTest, CMPYAbsoluteCanCompareTwoValuesThatResultInANegative)
{
    CMPYAbsolute(CompareTwoValuesThatResultInANegative());
}