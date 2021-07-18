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

/**
; p2
* = $1000
lda #$00
sta $42
start
inc $42
ldx $42
inx
jmp start
*/

const std::vector<uint8_t> p2 = {
        0x00, 0x10, 0xA9, 0x00, 0x85, 0x42, 0xE6,
        0x42, 0xA6, 0x42, 0xE8, 0x4C, 0x04, 0x10 };

/**
; p3
* = $1000
loop
lda #0
beq loop
*/

const std::vector<uint8_t> p3 = {
        0x00, 0x10, 0xA9, 0x00, 0xF0, 0xFC };

const static std::vector<std::vector<uint8_t>> programs = { p1, p2, p3 };

class LoadProgramTest : public BaseTest
{
public:

};

