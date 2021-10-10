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


TEST_F(ShiftsTest, LSRCanShiftTheValueOfOne)
{
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRCanShiftAZeroIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x08, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x04);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRZeroPageCanShiftTheValueOfOne)
{
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRZeroPageCanShiftAZeroIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x08, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x01;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], 0x04);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRZeroPageXCanShiftTheValueOfOne)
{
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x05;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRZeroPageXCanShiftAZeroIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x08, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], 0x04);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRAbsoluteCanShiftTheValueOfOne)
{
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x05;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ABS;
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
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRAbsoluteCanShiftAZeroIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x08, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ABS;
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
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], 0x04);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRAbsoluteXCanShiftTheValueOfOne)
{
    // given:
    const uint8_t values[] = { 0x01, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x05;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, LSRAbsoluteXCanShiftAZeroIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x08, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_LSR_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], 0x04);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


// ---------------------------- ROL ----------------------------


TEST_F(ShiftsTest, ROLCanShiftABitOutOfTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, values[0] * 2 + 1);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLCanShiftABitIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0b10000000, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLCanShiftZeroWithNoCarry)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, values[0] * 2);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLCanShiftAValueThatREsultInANegativeValue)
{
    // given:
    const uint8_t values[] = { 0b01110011, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.A = values[0];
    cpu.Flags.C = true;
    cpu.Flags.Z = false;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, 0b11100111);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageCanShiftABitOutOfTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x42, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], values[0] * 2 + 1);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageCanShiftABitIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0b10000000, 0x00 };
    const uint8_t address[] = { 0x42, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageCanShiftZeroWithNoCarry)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], values[0] * 2);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageCanShiftAValueThatREsultInANegativeValue)
{
    // given:
    const uint8_t values[] = { 0b01110011, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZP;
    memory[startAddress + 1] = address[0];
    memory[address[0]] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], 0b11100111);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageXCanShiftABitOutOfTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x42, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], values[0] * 2 + 1);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageXCanShiftABitIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0b10000000, 0x00 };
    const uint8_t address[] = { 0x42, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageXCanShiftZeroWithNoCarry)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], values[0] * 2);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLZeroPageXCanShiftAValueThatREsultInANegativeValue)
{
    // given:
    const uint8_t values[] = { 0b01110011, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ZPX;
    memory[startAddress + 1] = address[0];
    memory[address[0] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + offset], 0b11100111);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteCanShiftABitOutOfTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABS;
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
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], values[0] * 2 + 1);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteCanShiftABitIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0b10000000, 0x00 };
    const uint8_t address[] = { 0x42, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABS;
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
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteCanShiftZeroWithNoCarry)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABS;
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
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], values[0] * 2);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteCanShiftAValueThatREsultInANegativeValue)
{
    // given:
    const uint8_t values[] = { 0b01110011, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABS;
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
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], 0b11100111);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteXCanShiftABitOutOfTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], values[0] * 2 + 1);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteXCanShiftABitIntoTheCarryFlag)
{
    // given:
    const uint8_t values[] = { 0b10000000, 0x00 };
    const uint8_t address[] = { 0x42, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], 0x00);
    EXPECT_EQ(cpu.Flags.C, true);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteXCanShiftZeroWithNoCarry)
{
    // given:
    const uint8_t values[] = { 0x00, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = false;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], values[0] * 2);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, true);
    EXPECT_EQ(cpu.Flags.N, false);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(ShiftsTest, ROLAbsoluteXCanShiftAValueThatREsultInANegativeValue)
{
    // given:
    const uint8_t values[] = { 0b01110011, 0x00 };
    const uint8_t address[] = { 0x80, 0x02 };
    const uint16_t offset = 0x27;
    const uint16_t startAddress = 0xFF00;

    cpu.Reset(startAddress, memory);
    cpu.X = offset;
    cpu.Flags.C = true;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[startAddress] = CPU::INS_ROL_ABSX;
    memory[startAddress + 1] = address[1];
    memory[startAddress + 2] = address[0];
    memory[(address[0] << 8) + address[1] + offset] = values[0];
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + offset], 0b11100111);
    EXPECT_EQ(cpu.Flags.C, false);
    EXPECT_EQ(cpu.Flags.Z, false);
    EXPECT_EQ(cpu.Flags.N, true);
    this->ExpectUnaffectedRegisters(copy);
}