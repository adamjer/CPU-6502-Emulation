#pragma once
#include "BaseTest.h"

/**
; p1
* = $1000
lda #$FF
start
sta $90
sta $8000
eor #$CC
jmp start
*/

const std::vector<uint8_t> p1 = {
        0x00, 0x10, 0xA9, 0xFF, 0x85, 0x90, 0x8D,
        0x00, 0x80, 0x49, 0xCC, 0x4C, 0x02, 0x10 };

const static std::vector<std::vector<uint8_t>> programs = { p1 };

class LoadProgramTest : public BaseTest
{
public:

};

