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
    uint8_t A = 0xcc, B = 0x84;
    cpu.A = 0xCC;
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
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(2, memory);

    // then:
    EXPECT_EQ(cpu.A, this->logicalOperation(A, B, operation));
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = address;
    memory[address] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(3, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, 3);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.X = 5;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = address;
    memory[address + cpu.X] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterZeroPageY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.Y = 5;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = address;
    memory[address + cpu.Y] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4480] = 0x37;
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterAbsoluteX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.X = 0x1;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4481] = 0x37; //0x4480 + 0x01
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterAbsoluteXWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.X = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x02;
    memory[0xFFFE] = 0x44;
    memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterAbsoluteY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.Y = 0x1;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4481] = 0x37; //0x4480 + 0x01
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void AndEorOraTest::TestLoadRegisterAbsoluteYWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.Y = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x02;
    memory[0xFFFE] = 0x44;
    memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
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

TEST_F(AndEorOraTest, LDAZeroPageCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDA_ZP, &CPU::A);
}

TEST_F(AndEorOraTest, LDAImmediateCanAffectZeroFlag)
{

    // given:
    cpu.A = 0x44;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0x0;
    // end -inline a little program
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(2, memory);

    // then:
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, LDAZeroPageXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPageX(CPU::INS_LDA_ZPX, &CPU::A);
}

TEST_F(AndEorOraTest, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps)
{

    // given:
    cpu.X = 0xFF;
    int address = 0x80, wrapper = 0x100;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ZPX;
    memory[0xFFFD] = address;
    memory[(address + cpu.X) % wrapper] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, LDAAbsoluteCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDA_ABS, &CPU::A);
}

TEST_F(AndEorOraTest, LDAAbsoluteXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(AndEorOraTest, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteXWhenCrossingBoundary(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(AndEorOraTest, LDAAbsoluteYCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(AndEorOraTest, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteYWhenCrossingBoundary(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(AndEorOraTest, LDAIndirectXCanLoadAValueIntoTheARegister)
{

    // given:
    cpu.X = 0x04;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_INDX;
    memory[0xFFFD] = 0x02;
    memory[0x0006] = 0x00; //0x2 + 0x4
    memory[0x0007] = 0x80;
    memory[0x8000] = 0x37;
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, LDAIndirectYCanLoadAValueIntoTheARegister)
{

    // given:
    cpu.Y = 0x04;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_INDY;
    memory[0xFFFD] = 0x02;
    memory[0x0002] = 0x00; //0x2 + 0x4
    memory[0x0003] = 0x80;
    memory[0x8004] = 0x37; //0x8000 + 0x4
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

TEST_F(AndEorOraTest, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{

    // given:
    cpu.Y = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_INDY;
    memory[0xFFFD] = 0x02;
    memory[0x0002] = 0x02;
    memory[0x0003] = 0x80;
    memory[0x8101] = 0x37; //0x8002 + 0xFF
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

