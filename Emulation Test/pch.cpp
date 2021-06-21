//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

void EmulationTest::SetUp()
{
    cpu.Reset(memory);
}

void EmulationTest::Teardown()
{
}

void EmulationTest::TestLoadRegisterImmediate(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = true;
    cpu.Flags.N = false;
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x84;

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(2, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x84);
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_TRUE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterZeroPage(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = address;
    memory[address] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(3, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, 3);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterZeroPageX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.X = 5;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = address;
    memory[address + cpu.X] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterZeroPageY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.Y = 5;
    int address = 0x42;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = address;
    memory[address + cpu.Y] = 0x37;
    // end -inline a little program

    // when:
    CPU copy = cpu;
    int32_t cyclesUsed = cpu.Execute(4, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterAbsolute(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4480] = 0x37;
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterAbsoluteX(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.X = 0x1;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4481] = 0x37; //0x4480 + 0x01
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterAbsoluteXWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.X = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x02;
    memory[0xFFFE] = 0x44;
    memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterAbsoluteY(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.Y = 0x1;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0x44;
    memory[0x4481] = 0x37; //0x4480 + 0x01
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 4;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}

void EmulationTest::TestLoadRegisterAbsoluteYWhenCrossingBoundary(uint8_t opcodeToTest, uint8_t CPU::* RegisterToTest)
{
    // given:
    cpu.Flags.Z = cpu.Flags.N = true;
    cpu.Y = 0xFF;
    // start - inline a little program
    memory[0xFFFC] = opcodeToTest;
    memory[0xFFFD] = 0x02;
    memory[0xFFFE] = 0x44;
    memory[0x4501] = 0x37; //0x4402+0xFF crosses page boundary
    // end -inline a little program
    constexpr int32_t EXPECTED_CYCLES = 5;
    CPU copy = cpu;

    // when:    
    int32_t cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

    // then:
    EXPECT_EQ(cpu.*RegisterToTest, 0x37);
    EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Flags.Z);
    EXPECT_FALSE(cpu.Flags.N);
    VerifyUnmodifiedFlagsFromLDA(cpu, copy);
}
