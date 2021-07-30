#include "StatusFlagChangeTest.h"

TEST_F(StatusFlagChangeTest, CLCWillClearCarryFlag)
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
    memory[startAddress] = CPU::INS_CLC;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);  
    EXPECT_EQ(cpu.PS | 0b00000001, copy.PS | 0b00000001);
    EXPECT_FALSE(cpu.Flags.C);
}

TEST_F(StatusFlagChangeTest, SECWillSetsCarryFlag)
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
    memory[startAddress] = CPU::INS_SEC;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS | 0b00000001, copy.PS | 0b00000001);
    EXPECT_TRUE(cpu.Flags.C);
}

TEST_F(StatusFlagChangeTest, CLDWillClearCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.D = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_CLD;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS | 0b00001000, copy.PS | 0b00001000);
    EXPECT_FALSE(cpu.Flags.D);
}

TEST_F(StatusFlagChangeTest, SEDWillSetsCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.D = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_SED;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS | 0b00001000, copy.PS | 0b00001000);
    EXPECT_TRUE(cpu.Flags.D);
}

TEST_F(StatusFlagChangeTest, CLIWillClearCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.I = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_CLI;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS | 0b00000100, copy.PS | 0b00000100);
    EXPECT_FALSE(cpu.Flags.I);
}

TEST_F(StatusFlagChangeTest, SEIWillSetsCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x42, 0x32 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Y = values[0];
    cpu.X = values[1];
    cpu.Flags.I = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_SEI;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS | 0b00000100, copy.PS | 0b00000100);
    EXPECT_TRUE(cpu.Flags.I);
}

TEST_F(StatusFlagChangeTest, CLVWillClearCarryFlag)
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
    memory[startAddress] = CPU::INS_CLV;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.PS | 0b01000000, copy.PS | 0b01000000);
    EXPECT_FALSE(cpu.Flags.V);
}

TEST_F(StatusFlagChangeTest, NOPWillDoNothingButConsumeCycle)
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
