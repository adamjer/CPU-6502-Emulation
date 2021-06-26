#include "JumpsAndCallsTests.h"

void JumpsAndCallsTests::SetUp()
{
    cpu.Reset(memory);
}

void JumpsAndCallsTests::Teardown()
{
}

//void JumpsAndCallsTests::TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
//{
//    // given:
//    cpu.Flags.Z = cpu.Flags.N = true;
//    int address = 0x42;
//    // start - inline a little program
//    memory[0xFFFC] = opcodeToTest;
//    memory[0xFFFD] = address;
//    memory[address] = 0x37;
//    // end -inline a little program
//
//    // when:
//    CPU copy = cpu;
//    int32_t cyclesUsed = cpu.Execute(3, memory);
//
//    // then:
//    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
//    EXPECT_EQ(cyclesUsed, 3);
//    EXPECT_FALSE(cpu.Flags.Z);
//    EXPECT_FALSE(cpu.Flags.N);
//    VerifyUnmodifiedFlagsFromLoadRegister(cpu, copy);
//}

TEST_F(JumpsAndCallsTests, CanJumpToASubroutineAndJumpBackAgain)
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
    // end -inline a little program
    CPU copy = cpu;
    constexpr uint32_t EXPECTED_CYCLES = 6 + 6 + 2;

    // when:   
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.A, result);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}
