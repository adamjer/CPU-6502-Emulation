#include "pch.h"


static void VerifyUnmodifiedFlagsFromLDA(const CPU& cpu, const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}

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

TEST_F(EmulationTest, ExecutingABadInstructionDoesNotPutUsInAnInfiniteLoop)
{
    // given:
    // start - inline a little program
    memory[0xFFFC] = 0x0; //invalid opcode
    memory[0xFFFD] = 0x0;
    // end -inline a little program
    constexpr int32_t NUM_CYCLES = 2;

    // when:
    int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, NUM_CYCLES);
}

TEST_F(EmulationTest, LDAImmediateCanLoadAValueIntoTheARegister) 
{

    // given:
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0x84;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(2, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
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

TEST_F(EmulationTest, LDAZeroPageCanLoadAValueIntoTheARegister) 
{

    // given:
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ZP;
    memory[0xFFFD] = address;
    memory[address] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(3, memory);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(cyclesUsed, 3);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

TEST_F(EmulationTest, LDAZeroPageXCanLoadAValueIntoTheARegister) 
{

    // given:
    cpu.X = 5;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ZPX;
    memory[0xFFFD] = address;
    memory[address + cpu.X] = 0x37;
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

    // given:
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ABS;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4480] = 0x37;
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
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

TEST_F(EmulationTest, LDAAbsoluteXCanLoadAValueIntoTheARegister)
{

    // given:
    cpu.X = 0x1;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ABSX;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4481] = 0x37; //0x4480 + 0x01
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
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

TEST_F(EmulationTest, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{

    // given:
    cpu.X = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ABSX;
    memory[0xFFFD] = 0x02;
    memory[0xFFFE] = 0x44;
    memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary
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

TEST_F(EmulationTest, LDAAbsoluteYCanLoadAValueIntoTheARegister)
{

    // given:
    cpu.Y = 0x1;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ABSY;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4481] = 0x37;
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
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

TEST_F(EmulationTest, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesPageBoundary)
{

    // given:
    cpu.Y = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = CPU::INS_LDA_ABSY;
    memory[0xFFFD] = 0x02;
    memory[0xFFFE] = 0x44;
    memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary
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