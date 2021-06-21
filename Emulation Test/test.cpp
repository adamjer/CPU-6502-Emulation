#include "pch.h"


TEST_F(EmulationTest, TheCPUDoesNothingWhenWeExecuteZeroCycles) 
{
    // given:
    constexpr int32_t NUM_CYCLES = 0;

    // when:
    int32_t cyclesUsed = cpu.Execute(0, memory);

    // then:
    EXPECT_EQ(cyclesUsed, 0);
}

TEST_F(EmulationTest, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction)
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

TEST_F(EmulationTest, LDAImmediateCanLoadAValueIntoTheARegister) 
{
    TestLoadRegisterImmediate(CPU::INS_LDA_IM, &CPU::A);
}

TEST_F(EmulationTest, LDXImmediateCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterImmediate(CPU::INS_LDX_IM, &CPU::X);
}

TEST_F(EmulationTest, LDYImmediateCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterImmediate(CPU::INS_LDY_IM, &CPU::Y);
}

TEST_F(EmulationTest, LDAZeroPageCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDA_ZP, &CPU::A);
}

TEST_F(EmulationTest, LDXZeroPageCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDX_ZP, &CPU::X);
}

TEST_F(EmulationTest, LDYZeroPageCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterZeroPage(CPU::INS_LDY_ZP, &CPU::Y);
}

TEST_F(EmulationTest, LDAImmediateCanAffectZeroFlag)
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
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

TEST_F(EmulationTest, LDAZeroPageXCanLoadAValueIntoTheARegister) 
{
    TestLoadRegisterZeroPageX(CPU::INS_LDA_ZPX, &CPU::A);
}

TEST_F(EmulationTest, LDXZeroPageYCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterZeroPageY(CPU::INS_LDX_ZPY, &CPU::X);
}

TEST_F(EmulationTest, LDYZeroPageXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterZeroPageX(CPU::INS_LDY_ZPX, &CPU::Y);
}

TEST_F(EmulationTest, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps) 
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
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

TEST_F(EmulationTest, LDAAbsoluteCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDA_ABS, &CPU::A);
}

TEST_F(EmulationTest, LDXAbsoluteCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDX_ABS, &CPU::X);
}

TEST_F(EmulationTest, LDYAbsoluteCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterAbsolute(CPU::INS_LDY_ABS, &CPU::Y);
}

TEST_F(EmulationTest, LDAAbsoluteXCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(EmulationTest, LDXAbsoluteYCanLoadAValueIntoTheXRegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(EmulationTest, LDYAbsoluteXCanLoadAValueIntoTheYRegister)
{
    TestLoadRegisterAbsoluteX(CPU::INS_LDY_ABSX, &CPU::Y);
}

TEST_F(EmulationTest, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteXWhenCrossingBoundary(CPU::INS_LDA_ABSX, &CPU::A);
}

TEST_F(EmulationTest, LDYAbsoluteXCanLoadAValueIntoTheYRegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteXWhenCrossingBoundary(CPU::INS_LDY_ABSX, &CPU::Y);
}

TEST_F(EmulationTest, LDAAbsoluteYCanLoadAValueIntoTheARegister)
{
    TestLoadRegisterAbsoluteY(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(EmulationTest, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteYWhenCrossingBoundary(CPU::INS_LDA_ABSY, &CPU::A);
}

TEST_F(EmulationTest, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{
    TestLoadRegisterAbsoluteYWhenCrossingBoundary(CPU::INS_LDX_ABSY, &CPU::X);
}

TEST_F(EmulationTest, LDAIndirectXCanLoadAValueIntoTheARegister)
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
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

TEST_F(EmulationTest, LDAIndirectYCanLoadAValueIntoTheARegister)
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
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

TEST_F(EmulationTest, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
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
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}