#include "TransferRegisterTest.h"


void TransferRegisterTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
}


TEST_F(TransferRegisterTest, TAXCanTransferANonNegativeNonZeroValue)
{
    // given:
    const uint8_t A = 0x42, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = A;
    cpu.X = B;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TAX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.X, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TAXCanTransferANonNegativeZeroValue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = A;
    cpu.X = B;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TAX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.X, A);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TAXCanTransferANegativeValue)
{
    // given:
    const uint8_t A = 0b10010010, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = A;
    cpu.X = B;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_TAX;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.X, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TAYCanTransferANonNegativeNonZeroValue)
{
    // given:
    const uint8_t A = 0x42, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = A;
    cpu.Y = B;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TAY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TAYCanTransferANonNegativeZeroValue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = A;
    cpu.Y = B;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TAY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TAYCanTransferANegativeValue)
{
    // given:
    const uint8_t A = 0b10010010, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = A;
    cpu.Y = B;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_TAY;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TXACanTransferANonNegativeNonZeroValue)
{
    // given:
    const uint8_t A = 0x42, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = B;
    cpu.X = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TXA;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.X, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TXACanTransferANonNegativeZeroValue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = B;
    cpu.X = A;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TXA;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TXACanTransferANegativeValue)
{
    // given:
    const uint8_t A = 0b10010010, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = B;
    cpu.X = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_TXA;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.X, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TYACanTransferANonNegativeNonZeroValue)
{
    // given:
    const uint8_t A = 0x42, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = B;
    cpu.Y = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TYA;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TYACanTransferANonNegativeZeroValue)
{
    // given:
    const uint8_t A = 0x00, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = B;
    cpu.Y = A;
    cpu.Flags.Z = false;
    cpu.Flags.N = true;
    // start - inline a little program
    memory[offset] = CPU::INS_TYA;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_TRUE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}


TEST_F(TransferRegisterTest, TYACanTransferANegativeValue)
{
    // given:
    const uint8_t A = 0b10010010, B = 0x32;
    const uint8_t address[2] = { 0x80, 0x02 };
    const uint16_t offset = 0x02;

    cpu.Reset(offset, memory);
    cpu.A = B;
    cpu.Y = A;
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    // start - inline a little program
    memory[offset] = CPU::INS_TYA;
    // end - inline a little program

    // when:    
    constexpr int32_t EXPECTED_CYCLES = 2;
    CPU copy = cpu;

    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_EQ(cpu.A, A);
    EXPECT_EQ(cpu.Y, A);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    this->ExpectUnaffectedRegisters(copy);;
}
