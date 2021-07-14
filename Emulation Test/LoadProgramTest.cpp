#include "LoadProgramTest.h"

TEST_F(BaseTest, TestLoadProgramAProgramIntoTheCorrectAreaOfMemory)
{
    // given:

    // start - inline a little program

    // end - inline a little program

    // when:   
    cpu.LoadProgram(programs[0], memory);

    uint16_t offset = (programs[0][1] << 8) + programs[0][0];
    // then:
    EXPECT_EQ(memory[offset - 1], 0x00);
    EXPECT_EQ(memory[offset], 0xA9);
    EXPECT_EQ(memory[offset + 1], 0xFF);
    for (int i = 4; i < programs[0].size(); i++)
    {
        EXPECT_EQ(memory[offset + i - 2], programs[0][i]);
    }
}

TEST_F(BaseTest, TestLoadProgramAndExecuteIt)
{
    // given:

    // start - inline a little program

    // end - inline a little program

    // when:   
    uint16_t startAddress = cpu.LoadProgram(programs[0], memory);
    cpu.Reset(startAddress);

    for (int32_t clock = 1000; clock > 0;)
    {
        clock -= cpu.Execute(1, memory);
        //cpu.PrintStatus();
    }
}
