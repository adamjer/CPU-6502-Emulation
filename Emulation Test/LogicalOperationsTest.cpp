#include "LogicalOperationsTest.h"


uint8_t LogicalOperationsTest::logicalOperation(uint8_t A, uint8_t B, LogicalOperation operation)
{
    switch (operation)
    {
    case LogicalOperation::And:
        return A & B;
        break;
    case LogicalOperation::Or:
        return A | B;
        break;
    case LogicalOperation::Eor:
        return A ^ B;
        break;
    default:
        break;
    }

    throw - 2; //invalid Logical Operation
}

void LogicalOperationsTest::TestLogicalOperationOnARegisterImmediate(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    cpu.A = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
        break;
    default:
        break;
    }
    memory[0xFFFD] = B;

    // when:
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationZeroPage(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint16_t address = 0x42;
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ZP;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ZP;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ZP;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address;
    memory[address] = B;

    // when:
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 3;

    int32_t cyclesUsed = cpu.Execute(3, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 3);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationZeroPageX(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint16_t address = 0x42;
    cpu.A = A;
    cpu.X = 0x05;
    cpu.Flags.Z = cpu.Flags.N = true;    
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ZPX;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ZPX;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ZPX;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address;
    memory[address + cpu.X] = B;

    // when:
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationZeroPageY(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint16_t address = 0x42;
    cpu.A = A;
    cpu.Y = 0x05;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address;
    memory[address + cpu.Y] = B;

    // when:
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationAbsolute(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x44, 0x80 };
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ABS;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ABS;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ABS;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1]] = B;

    // when:    
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationAbsoluteX(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x44, 0x80 };
    cpu.A = A;
    cpu.X = 0x05;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ABSX;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ABSX;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ABSX;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.X] = B;

    // when:    
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationAbsoluteXWhenCrossingPage(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x44, 0x02 };
    cpu.A = A;
    cpu.X = 0xFF;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ABSX;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ABSX;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ABSX;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.X] = B;

    // when:    
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 5;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationAbsoluteY(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x44, 0x80 };
    cpu.A = A;
    cpu.Y = 0x05;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ABSY;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ABSY;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ABSY;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.Y] = B;

    // when:    
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

void LogicalOperationsTest::TestLogicalOperationAbsoluteYWhenCrossingPage(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x44, 0x80 };
    cpu.A = A;
    cpu.Y = 0xFF;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ABSY;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ABSY;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ABSY;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.Y] = B;

    // when:   
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 5;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationOnARegisterImmediate)
{
    this->TestLogicalOperationOnARegisterImmediate(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationOnARegisterImmediate)
{
    this->TestLogicalOperationOnARegisterImmediate(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationOnARegisterImmediate)
{
    this->TestLogicalOperationOnARegisterImmediate(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationZeroPag)
{
    TestLogicalOperationZeroPage(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationZeroPag)
{
    TestLogicalOperationZeroPage(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationZeroPag)
{
    TestLogicalOperationZeroPage(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalOperationEORImmediateCanAffectZeroFlag)
{
    // given:
    uint8_t A = 0xCC;
    cpu.A = A;
    memory[0xFFFC] = CPU::INS_EOR_IM;
    memory[0xFFFD] = A;

    //when:
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(cpu.A, this->logicalOperation(A, A, LogicalOperation::Eor));
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationZeroPageX)
{
    TestLogicalOperationZeroPageX(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationZeroPageX)
{
    TestLogicalOperationZeroPageX(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationZeroPageX)
{
    TestLogicalOperationZeroPageX(LogicalOperation::Eor);
}

void LogicalOperationsTest::TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint16_t address = 0x80, wrapper = 0x100;
    cpu.A = A;
    cpu.X = 0xFF;
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_ZPX;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_ZPX;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_ZPX;
        break;
    default:
        break;
    }
    memory[0xFFFD] = address;
    memory[(address + cpu.X) % wrapper] = B;
    // end - inline a little program

    // when:
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 4;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationZeroPageXWhenItWraps)
{
    this->TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationZeroPageXWhenItWraps)
{
    this->TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationZeroPageXWhenItWraps)
{
    this->TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationAbsolute)
{
    TestLogicalOperationAbsolute(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationAbsolute)
{
    TestLogicalOperationAbsolute(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationAbsolute)
{
    TestLogicalOperationAbsolute(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationAbsoluteX)
{
    TestLogicalOperationAbsoluteX(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationAbsoluteX)
{
    TestLogicalOperationAbsoluteX(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationAbsoluteX)
{
    TestLogicalOperationAbsoluteX(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationAbsoluteXWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteXWhenCrossingPage(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationAbsoluteXWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteXWhenCrossingPage(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationAbsoluteXWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteXWhenCrossingPage(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationAbsoluteY)
{
    TestLogicalOperationAbsoluteY(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationAbsoluteY)
{
    TestLogicalOperationAbsoluteY(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationAbsoluteY)
{
    TestLogicalOperationAbsoluteY(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationAbsoluteYWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteYWhenCrossingPage(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationAbsoluteYWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteYWhenCrossingPage(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEorOperationAbsoluteYWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteYWhenCrossingPage(LogicalOperation::Eor);
}

void LogicalOperationsTest::TestLogicalOperationIndirectX(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x80, 0x00 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.X = 0x05;
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_INDX;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_INDX;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_INDX;
        break;
    default:
        break;
    }
    memory[0xFFFD] = offset;
    memory[offset + cpu.X] = address[1];
    memory[offset + cpu.X + 1] = address[0];
    memory[(address[0] << 8) + address[1]] = B;
    // end - inline a little program

    // when:   
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationIndirectX)
{
    this->TestLogicalOperationIndirectX(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationIndirectX)
{
    this->TestLogicalOperationIndirectX(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationIndirectX)
{
    this->TestLogicalOperationIndirectX(LogicalOperation::Eor);
}

void LogicalOperationsTest::TestLogicalOperationIndirectY(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x80, 0x00 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.Y = 0x05;
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_INDY;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_INDY;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_INDY;
        break;
    default:
        break;
    }
    memory[0xFFFD] = offset;
    memory[offset] = address[1];
    memory[offset + 1] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.Y] = B; //0x8000 + 0x5
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A, expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationIndirectY)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalOROperationIndirectY)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationIndirectY)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::Eor);
}

void LogicalOperationsTest::TestLogicalOperationIndirectYWhenItCrossesPage(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.Y = 0xFF;
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_INDY;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_ORA_INDY;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_INDY;
        break;
    default:
        break;
    }
    memory[0xFFFD] = offset;
    memory[offset] = address[1];
    memory[offset + 1] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.Y] = B; //0x8002 + 0xFF
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    const uint8_t expectedResult = this->logicalOperation(A, B, operation);
    const bool isNegative = (expectedResult & 0b10000000) > 0;
    EXPECT_EQ(cpu.A,expectedResult);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_EQ(cpu.Flags.N, isNegative);
    VerfifyUnmodifiedFlagsFromLogicalOpInstruction(cpu, copy);
}

TEST_F(LogicalOperationsTest, TestLogicalANDOperationIndirectYWhenItCrossesPage)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::And);
}

TEST_F(LogicalOperationsTest, TestLogicalORAOperationIndirectYWhenItCrossesPage)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::Or);
}

TEST_F(LogicalOperationsTest, TestLogicalEOROperationIndirectYWhenItCrossesPage)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::Eor);
}

TEST_F(LogicalOperationsTest, TestBitZeroPage)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = true;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ZP;
    memory[0xFFFD] = offset;
    memory[offset] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    EXPECT_TRUE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitZeroPageResultZero)
{
    // given:
    const uint8_t A = 0xCC, B = 0x33;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = false;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ZP;
    memory[0xFFFD] = offset;
    memory[offset] = B;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    EXPECT_FALSE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitZeroPageResultZeroBits6And7Zero)
{
    // given:
    const uint8_t A = 0xCC, B = 0x33;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = B;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = false;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ZP;
    memory[0xFFFD] = offset;
    memory[offset] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, B);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    EXPECT_TRUE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitZeroPageResultZeroBits6And7Mixed)
{
    // given:
    const uint8_t A = 0b01000000, B = 0x33;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = B;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = false;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ZP;
    memory[0xFFFD] = offset;
    memory[offset] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, B);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    EXPECT_TRUE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitAbsolute)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = true;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ABS;
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    EXPECT_TRUE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitAbsoluteResultZero)
{
    // given:
    const uint8_t A = 0xCC, B = 0x33;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = false;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ABS;
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1]] = B;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    EXPECT_FALSE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitAbsoluteResultZeroBits6And7Zero)
{
    // given:
    const uint8_t A = 0xCC, B = 0x33;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = B;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = false;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ABS;
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, B);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    EXPECT_TRUE(cpu.Flags.V);
}

TEST_F(LogicalOperationsTest, TestBitAbsoluteResultZeroBits6And7Mixed)
{
    // given:
    const uint8_t A = 0b10000000, B = 0x33;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    cpu.A = B;
    cpu.Flags.Z = cpu.Flags.N = cpu.Flags.V = false;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_BIT_ABS;
    memory[0xFFFD] = address[1];
    memory[0xFFFE] = address[0];
    memory[(address[0] << 8) + address[1]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, B);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    EXPECT_FALSE(cpu.Flags.V);
}

