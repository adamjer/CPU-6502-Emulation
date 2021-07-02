#include "AndEorOraTest.h"


uint8_t AndEorOraTest::logicalOperation(uint8_t A, uint8_t B, LogicalOperation operation)
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

void AndEorOraTest::TestLogicalOperationOnARegisterImmediate(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_OR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationZeroPage(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint16_t address = 0x42;
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 3);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationZeroPageX(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationZeroPageY(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_OR_IM;
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
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationAbsolute(LogicalOperation operation)
{
    // given:
    const uint8_t A = 0xCC, B = 0x84;
    const uint8_t address[2] = { 0x44, 0x80 };
    cpu.A = A;
    cpu.Flags.Z = cpu.Flags.N = true;
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationAbsoluteX(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationAbsoluteXWhenCrossingBoundary(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationAbsoluteY(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLogicalOperationAbsoluteYWhenCrossingBoundary(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationOnARegisterImmediate)
{
    this->TestLogicalOperationOnARegisterImmediate(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationOnARegisterImmediate)
{
    this->TestLogicalOperationOnARegisterImmediate(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationOnARegisterImmediate)
{
    this->TestLogicalOperationOnARegisterImmediate(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationZeroPag)
{
    TestLogicalOperationZeroPage(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationZeroPag)
{
    TestLogicalOperationZeroPage(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationZeroPag)
{
    TestLogicalOperationZeroPage(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalOperationEORImmediateCanAffectZeroFlag)
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
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationZeroPageX)
{
    TestLogicalOperationZeroPageX(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationZeroPageX)
{
    TestLogicalOperationZeroPageX(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationZeroPageX)
{
    TestLogicalOperationZeroPageX(LogicalOperation::Eor);
}

void AndEorOraTest::TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationZeroPageXWhenItWraps)
{
    this->TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationZeroPageXWhenItWraps)
{
    this->TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationZeroPageXWhenItWraps)
{
    this->TestLogicalOperationZeroPageXWhenItWraps(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationAbsolute)
{
    TestLogicalOperationAbsolute(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationAbsolute)
{
    TestLogicalOperationAbsolute(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationAbsolute)
{
    TestLogicalOperationAbsolute(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationAbsoluteX)
{
    TestLogicalOperationAbsoluteX(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationAbsoluteX)
{
    TestLogicalOperationAbsoluteX(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationAbsoluteX)
{
    TestLogicalOperationAbsoluteX(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationAbsoluteXWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteXWhenCrossingBoundary(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationAbsoluteXWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteXWhenCrossingBoundary(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationAbsoluteXWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteXWhenCrossingBoundary(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationAbsoluteY)
{
    TestLogicalOperationAbsoluteY(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationAbsoluteY)
{
    TestLogicalOperationAbsoluteY(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationAbsoluteY)
{
    TestLogicalOperationAbsoluteY(LogicalOperation::Eor);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationAbsoluteYWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteYWhenCrossingBoundary(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationAbsoluteYWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteYWhenCrossingBoundary(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEorOperationAbsoluteYWhenCrossingBoundary)
{
    TestLogicalOperationAbsoluteYWhenCrossingBoundary(LogicalOperation::Eor);
}

void AndEorOraTest::TestLogicalOperationIndirectX(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
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
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationIndirectX)
{
    this->TestLogicalOperationIndirectX(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationIndirectX)
{
    this->TestLogicalOperationIndirectX(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationIndirectX)
{
    this->TestLogicalOperationIndirectX(LogicalOperation::Eor);
}

void AndEorOraTest::TestLogicalOperationIndirectY(LogicalOperation operation)
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
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
        break;
    default:
        break;
    }
    memory[0xFFFD] = offset;
    memory[offset] = address[1]; //0x2 + 0x4
    memory[offset + 1] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.Y] = 0x37; //0x8000 + 0x4
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, TestLogicalANDOperationIndirectY)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::And);
}

TEST_F(AndEorOraTest, TestLogicalOROperationIndirectY)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::Or);
}

TEST_F(AndEorOraTest, TestLogicalEOROperationIndirectY)
{
    this->TestLogicalOperationIndirectY(LogicalOperation::Eor);
}

void AndEorOraTest::TestLogicalOperationIndirectYWhenItCrossesPageBoundary(LogicalOperation operation)
{
    // given:
    cpu.Y = 0xFF;
    // start - inline a little program
    switch (operation)
    {
    case LogicalOperation::And:
        memory[0xFFFC] = CPU::INS_AND_IM;
        break;
    case LogicalOperation::Or:
        memory[0xFFFC] = CPU::INS_OR_IM;
        break;
    case LogicalOperation::Eor:
        memory[0xFFFC] = CPU::INS_EOR_IM;
        break;
    default:
        break;
    }
    memory[0xFFFD] = 0x02;
    memory[0x0002] = 0x02;
    memory[0x0003] = 0x80;
    memory[0x8101] = 0x37; //0x8002 + 0xFF
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, TestLogicalOperationIndirectYWhenItCrossesPageBoundary)
{

}

