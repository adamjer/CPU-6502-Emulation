#include "StackOperationsTest.h"

void StackOperationsTest::SetUp()
{
    cpu.Reset(memory);
}

void StackOperationsTest::Teardown()
{
}

TEST_F(StackOperationsTest, TSXCanTransferTheStackPointerToXRegister)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.X = 0x00;
    cpu.SP = 0x01;
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    memory[startOffset] = CPU::INS_TSX;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 2;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, 0x01);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
}

TEST_F(StackOperationsTest, TSXCanTransferAZeroStackPointerToXRegister)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.X = 0x00;
    cpu.SP = 0x00;
    cpu.PS = 0xFF;
    cpu.Flags.Z = cpu.Flags.N = true;    
    // start - inline a little program
    memory[startOffset] = CPU::INS_TSX;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 2;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, 0x00);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
}

TEST_F(StackOperationsTest, TSXCanTransferANegativeStackPointerToXRegister)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.X = 0x00;
    cpu.SP = 0b10000000;
    cpu.PS = 0xFF;
    cpu.Flags.Z = cpu.Flags.N = false;  
    // start - inline a little program
    memory[startOffset] = CPU::INS_TSX;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 2;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, 0b10000000);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
}

TEST_F(StackOperationsTest, TXSCanTransferXRegisterToTheStackPointer)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.X = 0xFF;
    cpu.SP = 0x00;
    // start - inline a little program
    memory[startOffset] = CPU::INS_TXS;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 2;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.SP, 0xFF);
    EXPECT_EQ(cpu.PS, copy.PS);
}

TEST_F(StackOperationsTest, PHACanPushARegisterOntoTheStack)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.A = result;
    // start - inline a little program
    memory[startOffset] = CPU::INS_PHA;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 3;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[cpu.StackPointerToAddress() + 1], cpu.A);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(StackOperationsTest, PLACanPullAValueFromStackIntoTheARegister)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0xCC;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.A = 0x00;
    cpu.SP = 0xFE;
    memory[0x01FF] = result;
    // start - inline a little program
    memory[startOffset] = CPU::INS_PLA;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, result);
    EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(StackOperationsTest, PLACanPullAZeroValueFromStackIntoTheARegister)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x00;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    cpu.A = 0xCC;
    cpu.SP = 0xFE;
    memory[0x01FF] = result;
    // start - inline a little program
    memory[startOffset] = CPU::INS_PLA;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, result);
    EXPECT_FALSE(cpu.Flags.N);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(StackOperationsTest, PLACanPullANegativeValueFromStackIntoTheARegister)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0b10000001;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.Flags.N = false;
    cpu.Flags.Z = true;
    cpu.A = 0xCC;
    cpu.SP = 0xFE;
    memory[0x01FF] = result;
    // start - inline a little program
    memory[startOffset] = CPU::INS_PLA;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, result);
    EXPECT_TRUE(cpu.Flags.N);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(StackOperationsTest, PHPCanPushProcessorStatusOntoStack)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0xCC;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.PS = result;
    // start - inline a little program
    memory[startOffset] = CPU::INS_PHP;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 3;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[cpu.StackPointerToAddress() + 1], result);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(StackOperationsTest, PLPCanPullAValueFromStackIntoTheProcessorStatus)
{
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0xCC;

    // given:
    cpu.Reset(startOffset, memory);
    cpu.SP = 0xFE;
    cpu.PS = 0x00;
    memory[0x01FF] = result;
    // start - inline a little program
    memory[startOffset] = CPU::INS_PLP;
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, result);
}