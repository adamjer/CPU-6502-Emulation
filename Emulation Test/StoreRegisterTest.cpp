#include "StoreRegisterTest.h"


void StoreRegisterTest::ExpectUnaffectedRegisters(const CPU& copy)
{
    EXPECT_EQ(cpu.Flags.C, copy.Flags.C);
    EXPECT_EQ(cpu.Flags.Z, copy.Flags.Z);
    EXPECT_EQ(cpu.Flags.I, copy.Flags.I);
    EXPECT_EQ(cpu.Flags.D, copy.Flags.D);
    EXPECT_EQ(cpu.Flags.B, copy.Flags.B);
    EXPECT_EQ(cpu.Flags.V, copy.Flags.V);
    EXPECT_EQ(cpu.Flags.N, copy.Flags.N);
}


void StoreRegisterTest::TestStoreRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    //given:
    cpu.*RegisterToTest = 0x2F;
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0x0080] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 3;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);
    
    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x0080], 0x2F);
    this->ExpectUnaffectedRegisters(copy);
}


void StoreRegisterTest::TestStoreRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    //given:
    cpu.*RegisterToTest = 0x2F;
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x00;
    memory[0xFFFE] = 0x80;
    memory[0x8000] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x8000], 0x2F);
    this->ExpectUnaffectedRegisters(copy);
}


void StoreRegisterTest::TestStoreRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    //given:
    cpu.*RegisterToTest = 0x42;
    cpu.X = 0x0F;
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0x008F] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x008F], 0x42);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(StoreRegisterTest, STAZeroPageCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterZeroPage(CPU::INS_STA_ZP, &CPU::A);
}


TEST_F(StoreRegisterTest, STXZeroPageCanStoreTheXRegisterIntoMemory)
{
    TestStoreRegisterZeroPage(CPU::INS_STX_ZP, &CPU::X);
}


TEST_F(StoreRegisterTest, STYZeroPageCanStoreTheYRegisterIntoMemory)
{
    TestStoreRegisterZeroPage(CPU::INS_STY_ZP, &CPU::Y);
}


TEST_F(StoreRegisterTest, STAAbsoluteCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterAbsolute(CPU::INS_STA_ABS, &CPU::A);
}


TEST_F(StoreRegisterTest, STXAbsoluteCanStoreTheXRegisterIntoMemory)
{
    TestStoreRegisterAbsolute(CPU::INS_STX_ABS, &CPU::X);
}


TEST_F(StoreRegisterTest, STYAbsoluteCanStoreTheYRegisterIntoMemory)
{
    TestStoreRegisterAbsolute(CPU::INS_STY_ABS, &CPU::Y);
}


TEST_F(StoreRegisterTest, STAZeroPageXCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterZeroPageX(CPU::INS_STA_ZPX, &CPU::A);
}


TEST_F(StoreRegisterTest, STYZeroPageXCanStoreTheARegisterIntoMemory)
{
    TestStoreRegisterZeroPageX(CPU::INS_STY_ZPX, &CPU::Y);
}


TEST_F(StoreRegisterTest, STAAbsoluteXCanStoreTheARegisterIntoMemory)
{
    //given:
    cpu.A = 0x42;
    cpu.X = 0x0F;
    memory[0xFFFC] = CPU::INS_STA_ABSX;
    memory[0xFFFD] = 0x00;
    memory[0xFFFE] = 0x80;
    memory[0x800F] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x800F], 0x42);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(StoreRegisterTest, STAAbsoluteYCanStoreTheARegisterIntoMemory)
{
    //given:
    cpu.A = 0x42;
    cpu.Y = 0x0F;
    memory[0xFFFC] = CPU::INS_STA_ABSY;
    memory[0xFFFD] = 0x00;
    memory[0xFFFE] = 0x80;
    memory[0x800F] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x800F], 0x42);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(StoreRegisterTest, STAIndirectXCanStoreTheARegisterIntoMemory)
{
    //given:
    cpu.A = 0x42;
    cpu.X = 0x0F;
    memory[0xFFFC] = CPU::INS_STA_INDX;
    memory[0xFFFD] = 0x20;
    memory[0x002F] = 0x00;
    memory[0x0030] = 0x80;
    memory[0x8000] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x8000], 0x42);
    this->ExpectUnaffectedRegisters(copy);
}


TEST_F(StoreRegisterTest, STAIndirectYCanStoreTheARegisterIntoMemory)
{
    //given:
    cpu.A = 0x42;
    cpu.Y = 0x0F;
    memory[0xFFFC] = CPU::INS_STA_INDY;
    memory[0xFFFD] = 0x20;
    memory[0x0020] = 0x00;
    memory[0x0021] = 0x80;
    memory[0x8000 + 0x0F] = 0x00;
    constexpr uint32_t EXPECTED_CYCLES = 6;
    CPU copy = cpu;

    //when:
    const uint32_t actualCycles = cpu.Execute(EXPECTED_CYCLES, memory);

    //then:
    EXPECT_EQ(actualCycles, EXPECTED_CYCLES);
    EXPECT_EQ(memory[0x8000 + 0x0F], 0x42);
    this->ExpectUnaffectedRegisters(copy);
}

