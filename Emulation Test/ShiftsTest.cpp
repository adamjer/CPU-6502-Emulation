#include "ShiftsTest.h"


void ShiftsTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}


TEST_F (ShiftsTest, ASLCanShiftTheValueOfOne)
{
    // given:
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x02);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLCanShiftNegativeValue)
{
    // given:
    // given:
    const uint8_t values[] = { 0b11000010, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0b10000100);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLZeroPageCanShiftTheValueOfOne)
{
    // given:
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], 0x02);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLZeroPageCanShiftNegativeValue)
{
    // given:
    // given:
    const uint8_t values[] = { 0b11000010, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], 0b10000100);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLZeroPageXCanShiftTheValueOfOne)
{
    // given:
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x05;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + cpu.X] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], 0x02);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLZeroPageXCanShiftNegativeValue)
{
    // given:
    // given:
    const uint8_t values[] = { 0b11000010, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x42;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + cpu.X] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], 0b10000100);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLAbsoluteCanShiftTheValueOfOne)
{
    // given:
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x05;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ABS;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], 0x02);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLAbsoluteCanShiftNegativeValue)
{
    // given:
    // given:
    const uint8_t values[] = { 0b11000010, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x42;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ABS;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], 0b10000100);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLAbsoluteXCanShiftTheValueOfOne)
{
    // given:
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x05;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.X] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], 0x02);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ASLAbsoluteXCanShiftNegativeValue)
{
    // given:
    // given:
    const uint8_t values[] = { 0b11000010, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x42;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ASL_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.X] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], 0b10000100);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}