#include "CompareRegistersTest.h"


void CompareRegistersTest::ExpectUnaffectedRegisters(const CPU& copy)
{
	EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
	EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
	EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
	EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareTwoIdenticalValues)
{
    // given:
    const uint8_t values[] = { 0x1A, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_CMP;
    memory[startAddress + 1] = values[0];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, values[0]);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareTwoDifferentPositiveValues)
{
    // given:
    const uint8_t values[] = { 0x1A, 0x30 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[1];
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_CMP;
    memory[startAddress + 1] = values[0];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, values[1]);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareANegativeNumberToAPositive)
{
    // given:
    const uint8_t values[] = { 0x1A, 0x82 }; // 0x82 is negative number
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[1];
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_CMP;
    memory[startAddress + 1] = values[0];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, values[1]);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(CompareRegistersTest, CMPImmediateCanCompareTwoValuesThatResultInANegative)
{
    // given:
    const uint8_t values[] = { 0x1A, 0x08 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[1];
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_CMP;
    memory[startAddress + 1] = values[0];

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, values[1]);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}