#include "SystemFunctionsTest.h"


TEST_F(SystemFunctionsTest, NOPWillDoNothingButConsumeCycle)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    // start - inline a little program
    memory[startAddress] = CPU::INS_NOP;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.PC, startAddress + 1);
    EXPECT_EQ(cpu.X, copy.X);
    EXPECT_EQ(cpu.Y, copy.Y);
    EXPECT_EQ(cpu.A, copy.A);
    EXPECT_EQ(cpu.SP, copy.SP);
}


TEST_F(SystemFunctionsTest, BRKWillLoadTheProgramCounterFromInterruptVector)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x00 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    // start - inline a little program
    memory[startAddress] = CPU::INS_BRK;
    memory[0xFFFE] = address[1];
    memory[0xFFFF] = address[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, (address[0] << 8) + address[1]);
}


TEST_F(SystemFunctionsTest, BRKWillLoadTheProgramCounterFromInterruptVector2)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x90, 0x00 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    // start - inline a little program
    memory[startAddress] = CPU::INS_BRK;
    memory[0xFFFE] = address[1];
    memory[0xFFFF] = address[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PC, (address[0] << 8) + address[1]);
}


TEST_F(SystemFunctionsTest, BRKWillSetTheBreakFlag)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x90, 0x00 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.B = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_BRK;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Flags.B, true);
}


TEST_F(SystemFunctionsTest, BRKWillPushThreeBytesOntoTheStack)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x90, 0x00 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    // start - inline a little program
    memory[startAddress] = CPU::INS_BRK;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.SP, copy.SP - 3);
}


TEST_F(SystemFunctionsTest, BRKWillPushPCandPSOntoTheStack)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x90, 0x00 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    // start - inline a little program
    memory[startAddress] = CPU::INS_BRK;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(0x100 | copy.SP) - 2], copy.PS);
    EXPECT_EQ(memory[(0x100 | copy.SP) - 1], (startAddress + 1) & 0xFF);
    EXPECT_EQ(memory[(0x100 | copy.SP) - 0], (startAddress + 1) >> 8);
}


TEST_F(SystemFunctionsTest, RTICanReturnFromInterruptLeavingCPUSameWhenEntered)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x00 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    // start - inline a little program
    memory[startAddress] = CPU::INS_BRK;
    memory[0xFFFE] = address[1];
    memory[0xFFFF] = address[0];
    memory[(address[0] << 8) + address[1]] = CPU::INS_RTI;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES_BRK = 7;
    constexpr int32_t EXPECTED_CYCLES_RTI = 6;
    CPU copy = cpu;

    int32_t cyclesUsedBRK = cpu.Execute(EXPECTED_CYCLES_BRK, memory);
    int32_t cyclesUsedRTI = cpu.Execute(EXPECTED_CYCLES_RTI, memory);

    // then:
    EXPECT_EQ(cyclesUsedBRK, EXPECTED_CYCLES_BRK);
    EXPECT_EQ(cyclesUsedRTI, EXPECTED_CYCLES_RTI);
    EXPECT_EQ(cpu.SP, copy.SP);
    EXPECT_EQ(cpu.PS, copy.PS);
    EXPECT_EQ(cpu.PC, startAddress + 1);
}