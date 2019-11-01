#include "Global.h"

class Command {
public:
    const char* name;
    uint8_t code;
    uint8_t argument_numbers;
    Command(const char* name, uint8_t code, uint8_t arg_n);
};

Command::Command(const char *name, uint8_t code, uint8_t arg_n) : name(name), code(code), argument_numbers(arg_n) {}

const std::vector<Command> ALL_COMMANDS = {{"pushr", Cmd::Push_reg, 1}, {"push", Cmd::Push_num, 1}, {"pop", Cmd::Pop, 1},
                                           {"mov", Cmd::Mov, 2}, {"add", Cmd::Add, 0}, {"mul", Cmd::Mul, 0},
                                           {"sub", Cmd::Sub, 0}, {"div", Cmd::Div, 0}, {"in", Cmd::In, 0},
                                           {"out", Cmd::Out, 0}, {"end", Cmd::End, 0}, {"jmp", Cmd::Jmp, 0},
                                           {"cmp", Cmd::Cmp, 2}, {"jz", Cmd::Jz, 0}, {"jl", Cmd::Jl, 0},
                                           {"ja", Cmd::Ja, 0}, {"ret", Cmd::Ret, 0}, {"call", Cmd::Call, 0},
                                           {"sqrt", Cmd::Sqrt, 0}, {"out_f", Cmd::Out_f, 0}};