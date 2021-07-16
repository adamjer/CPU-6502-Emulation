#include "IncrementDecrementTest.h"


TEST_F(IncrementDecrementTest, INXCanIncrementAZerovalue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.X = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_INX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INXCanIncrement255)
{
    // given:
    const uint8_t A = 0xFF, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.X = A;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_INX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, (uint8_t)(A + 1));
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INXCanIncrementANegativeValue)
{
    // given:
    const uint8_t A = 0b10001000, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.X = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_INX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INYCanIncrementAZerovalue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Y = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_INY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, A + 1);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INYCanIncrement255)
{
    // given:
    const uint8_t A = 0xFF, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Y = A;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_INY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, (uint8_t)(A + 1));
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INYCanIncrementANegativeValue)
{
    // given:
    const uint8_t A = 0b10001000, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Y = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_INY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DEXCanDecrementAZerovalue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.X = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_DEX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DEXCanDecrement255)
{
    // given:
    const uint8_t A = 0xFF, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.X = A;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_DEX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DEXCanDecrementANegativeValue)
{
    // given:
    const uint8_t A = 0b10001000, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.X = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_DEX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.X, (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DEYCanDecrementAZerovalue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Y = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_DEY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DEYCanDecrement255)
{
    // given:
    const uint8_t A = 0xFF, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Y = A;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_DEY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DEYCanDecrementANegativeValue)
{
    // given:
    const uint8_t A = 0b10001000, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Y = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_DEY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.Y, (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DECCanDecrementAValueZeroPage)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_DEC_ZP;
    memory[offset + 1] = address[0];
    memory[address[0]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DECCanDecrementAValueZeroPageX)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    cpu.X = 0x10;
    // start - inline a little program
    memory[offset] = CPU::INS_DEC_ZPX;
    memory[offset + 1] = address[0];
    memory[address[0] + cpu.X] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + cpu.X], (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DECCanDecrementAValueAbsolute)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x00 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_DEC_ABS;
    memory[offset + 1] = address[1];
    memory[offset + 2] = address[0];
    memory[(address[0] << 8) + address[1]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, DECCanDecrementAValueAbsoluteX)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x00 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    cpu.X = 0x10;
    // start - inline a little program
    memory[offset] = CPU::INS_DEC_ABSX;
    memory[offset + 1] = address[1];
    memory[offset + 2] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.X] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + cpu.X], (uint8_t)(A - 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INCCanIncrementAValueZeroPage)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_INC_ZP;
    memory[offset + 1] = address[0];
    memory[address[0]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0]], (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INCCanIncrementAValueZeroPageX)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    cpu.X = 0x10;
    // start - inline a little program
    memory[offset] = CPU::INS_INC_ZPX;
    memory[offset + 1] = address[0];
    memory[address[0] + cpu.X] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[address[0] + cpu.X], (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INCCanIncrementAValueAbsolute)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x00 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_INC_ABS;
    memory[offset + 1] = address[1];
    memory[offset + 2] = address[0];
    memory[(address[0] << 8) + address[1]] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1]], (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}

TEST_F(IncrementDecrementTest, INCCanIncrementAValueAbsoluteX)
{
    // given:
    const uint8_t A = 0x57, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x00 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    cpu.X = 0x10;
    // start - inline a little program
    memory[offset] = CPU::INS_INC_ABSX;
    memory[offset + 1] = address[1];
    memory[offset + 2] = address[0];
    memory[(address[0] << 8) + address[1] + cpu.X] = A;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 7;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(memory[(address[0] << 8) + address[1] + cpu.X], (uint8_t)(A + 1));
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedStatusFlagsFromIncrementDecrementRegister(cpu, copy);
}