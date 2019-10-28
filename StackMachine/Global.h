#include <stdint.h>
#include <string.h>
#include <iostream>


size_t const LINE_SIZE = 15;
size_t const MAX_SHIFT = 50;

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