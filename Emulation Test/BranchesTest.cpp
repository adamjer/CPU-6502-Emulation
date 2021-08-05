#include "BranchesTest.h"


TEST_F(BranchesTest, BEQCanBranchForwardWhenZeroFlagIsSet)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.Z = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BEQ;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BEQDoesNotBranchForwardWhenZeroFlagIsClear)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.Z = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BEQ;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BEQCanBranchForwardIntoNewPageWhenZeroFlagIsSet)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x1;
    const uint16_t startAddress = 0xFAFD;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.Z = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BEQ;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1 + 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BEQCanBranchBackwardWhenZeroFlagIsSet)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    const uint16_t startAddress = 0xFFCC;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.Z = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BEQ;
    memory[startAddress + 1] = static_cast<uint8_t>(-offset);
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 - offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BEQDoesNotBranchBackwardWhenZeroFlagIsClear)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;
    const uint16_t startAddress = 0xFFCC;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.Z = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BEQ;
    memory[startAddress + 1] = static_cast<uint8_t>(-offset);
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BNECanBranchForwardWhenZeroFlagIsClear)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.Z = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BNE;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BCSCanBranchForwardWhenCarryFlagIsSet)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.C = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BCS;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BCCCanBranchForwardWhenCarryFlagIsClear)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.C = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BCC;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BMICanBranchForwardWhenNegativeFlagIsSet)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BMI;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BPLCanBranchForwardWhenNegativeFlagIsClear)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BPL;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BVSCanBranchForwardWhenNegativeFlagIsSet)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.V = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BVS;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}


TEST_F(BranchesTest, BVCCanBranchForwardWhenNegativeFlagIsClear)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.V = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BVC;
    memory[startAddress + 1] = offset;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2 + 1;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, startAddress + 2 + offset);
    EXPECT_EQ(cpu.PS, copy.PS);
}

