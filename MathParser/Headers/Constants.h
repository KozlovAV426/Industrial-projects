#pragma once
#include <cstdlib>

enum FUNC {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,
    POW = 4,
    SIN = 5,
    COS = 6,
    VARIABLE = 7
};

const size_t FUNCTION_SIZE = 8;

static const char* FUNCTION_NAME[FUNCTION_SIZE] = {"+", "-", "*", "/", "**", "sin", "cos", "x"};