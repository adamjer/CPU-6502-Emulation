#include "LoadRegisterTest.h"

void LoadRegisterTest::SetUp()
{
    cpu.Reset(memory);
}

void LoadRegisterTest::Teardown()
{
}

void LoadRegisterTest::TestLoadRegisterImmediate(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x84;

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(2, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x84);
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromLoadRegister(cpu, copy);
}

void LoadRegisterTest::TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterZeroPageY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterAbsoluteX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterAbsoluteXWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterAbsoluteY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

void LoadRegisterTest::TestLoadRegisterAbsoluteYWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
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

TEST_F(LoadRegisterTest, TheCPUDoesNothingWhenWeExecuteZeroCycles)
{
    // given:
    constexpr int32_t NUM_CYCLES = 0;

    // when:
    int32_t cyclesUsed = cpu.Execute(0, memory);

    // then:
    EXPECT_EQ(cyclesUsed, 0);
}

TEST_F(LoadRegisterTest, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction)
{
    // given:
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0x84;
    // end -inline a little program
    constexpr int32_t NUM_CYCLES = 1;

    // when:
    int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, 2);
}

TEST_F(LoadRegisterTest, LDAImmediateCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterImmediate(CPU::INS_LDA_IM, &CPU::A);
}

TEST_F(LoadRegisterTest, LDXImmediateCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterImmediate(CPU::INS_LDX_IM, &CPU::X);
}

TEST_F(LoadRegisterTest, LDYImmediateCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterImmediate(CPU::INS_LDY_IM, &CPU::Y);
}

TEST_F(LoadRegisterTest, LDAZeroPageCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDA_ZP, &CPU::A);
}

TEST_F(LoadRegisterTest, LDXZeroPageCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDX_ZP, &CPU::X);
}

TEST_F(LoadRegisterTest, LDYZeroPageCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDY_ZP, &CPU::Y);
}

TEST_F(LoadRegisterTest, LDAImmediateCanAffectZeroFlag)
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

TEST_F(LoadRegisterTest, LDAZeroPageXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPageX(CPU::INS_LDA_ZPX, &CPU::A);
}

TEST_F(LoadRegisterTest, LDXZeroPageYCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterZeroPageY(CPU::INS_LDX_ZPY, &CPU::X);
}

TEST_F(LoadRegisterTest, LDYZeroPageXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPageX(CPU::INS_LDY_ZPX, &CPU::Y);
}

TEST_F(LoadRegisterTest, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps)
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

TEST_F(LoadRegisterTest, LDAAbsoluteCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDA_ABS, &CPU::A);
}

TEST_F(LoadRegisterTest, LDXAbsoluteCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDX_ABS, &CPU::X);
}

TEST_F(LoadRegisterTest, LDYAbsoluteCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDY_ABS, &CPU::Y);
}

TEST_F(LoadRegisterTest, LDAAbsoluteXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(LoadRegisterTest, LDXAbsoluteYCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(LoadRegisterTest, LDYAbsoluteXCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDY_ABSX, &CPU::Y);
}

TEST_F(LoadRegisterTest, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteXWhenCrossingBoundary(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(LoadRegisterTest, LDYAbsoluteXCanLoadAValueIntoTheYRegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteXWhenCrossingBoundary(CPU::INS_LDY_ABSX, &CPU::Y);
}

TEST_F(LoadRegisterTest, LDAAbsoluteYCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(LoadRegisterTest, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteYWhenCrossingBoundary(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(LoadRegisterTest, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteYWhenCrossingBoundary(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(LoadRegisterTest, LDAIndirectXCanLoadAValueIntoTheARegister)
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

TEST_F(LoadRegisterTest, LDAIndirectYCanLoadAValueIntoTheARegister)
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

TEST_F(LoadRegisterTest, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
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

