#pragma once

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <vector>


const double  DPRECISION = 1000.0;
const size_t  PRECISION = 1000;
const size_t  LINE_SIZE = 15;
const size_t  MAX_SHIFT = 50;

struct Instruction {
    uint8_t instruction = 0;
    int farg = 0;
    int sarg = 0;
};

enum Cmd {
    End = 0, Push_reg, Push_num, Pop,
    Mov, Add, Mul, Sub, Div,
    In, Out, Jmp, Label, Cmp,
    Jz, Jl, Ja, Ret, Call,
    Sqrt, Out_f
};

