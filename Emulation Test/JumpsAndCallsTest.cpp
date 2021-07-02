#include "JumpsAndCallsTest.h"

TEST_F(JumpsAndCallsTest, CanJumpToASubroutineAndJumpBackAgain)
{
    // given:
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;
    cpu.Reset(startOffset, memory);
    // start - inline a little program
    memory[startOffset] = CPU::INS_JSR;
    memory[startOffset + 1] = 0x00;
    memory[startOffset + 2] = 0x80;
    memory[0x8000] = CPU::INS_RTS;
    memory[startOffset + 3] = CPU::INS_LDA_IM;
    memory[startOffset + 4] = result;
    // end - inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 6 + 6 + 2;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, result);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.SP, copy.SP);
}

TEST_F(JumpsAndCallsTest, JSRDoesNotAffectTheProcessorStatus)
{
    // given:
    uint16_t startOffset = 0xFF00;
    cpu.Reset(startOffset, memory);
    // start - inline a little program
    memory[startOffset] = CPU::INS_JSR;
    memory[startOffset + 1] = 0x00;
    memory[startOffset + 2] = 0x80;
    // end - inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 6;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_NE(cpu.PS, copy.SP);
    EXPECT_EQ(cpu.PC, 0x8000);
}

TEST_F(JumpsAndCallsTest, RTSDoesNotAffectTheProcessorStatus)
{
    // given:
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;
    cpu.Reset(startOffset, memory);
    // start - inline a little program
    memory[startOffset] = CPU::INS_JSR;
    memory[startOffset + 1] = 0x00;
    memory[startOffset + 2] = 0x80;
    memory[0x8000] = CPU::INS_RTS;
    // end - inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 6 + 6;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.PC, startOffset + 2 + 1);
}

TEST_F(JumpsAndCallsTest, JumpAbsoluteCanJumpToAnNewLocationInTheProgram)
{
    // given:
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;
    cpu.Reset(startOffset, memory);
    // start - inline a little program
    memory[startOffset] = CPU::INS_JMP_ABS;
    memory[startOffset + 1] = 0x00;
    memory[startOffset + 2] = 0x80;
    memory[0x8000] = CPU::INS_RTS;
    // end - inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 3;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.SP, copy.SP);
    EXPECT_EQ(cpu.PC, 0x8000);
}

TEST_F(JumpsAndCallsTest, JumpIndirectCanJumpToAnNewLocationInTheProgram)
{
    // given:
    uint16_t startOffset = 0xFF00;
    uint8_t result = 0x42;
    cpu.Reset(startOffset, memory);
    // start - inline a little program
    memory[startOffset] = CPU::INS_JMP_IND;
    memory[startOffset + 1] = 0x00;
    memory[startOffset + 2] = 0x80;
    memory[0x8000] = 0x00;
    memory[0x8001] = 0x90;
    // end - inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 5;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.SP, copy.SP);
    EXPECT_EQ(cpu.PC, 0x9000);
}
