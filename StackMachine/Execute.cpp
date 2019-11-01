#include <Execute.h>
#include <Stack.h>


int ExecuteInstruction(UnkillableStack<int, IntViewer>& stack, int* registers,
                       const Instruction* instr, FILE* in_ptr, FILE* out_ptr, size_t& j, int& flag);




char* ReadAssembly(const char* asm_file, size_t* sz) {
    FILE* in_ptr = fopen(asm_file, "rb");

    fseek(in_ptr, 0, SEEK_END);
    *sz = ftell(in_ptr);
    char * commands = new char[*sz];
    fseek(in_ptr, 0, SEEK_SET);

    fread(commands, sizeof(char), *sz, in_ptr);

    fclose(in_ptr);
    return commands;

}

Instruction DecodeCommand(const char* buffer, size_t* counter) {
    Instruction result;

    uint8_t * u_ptr = nullptr;
    int* i_ptr = nullptr;

    u_ptr = (uint8_t*)(buffer);
    result.instruction = *u_ptr;
    *counter += 1;
    if ((*u_ptr > Cmd::End && *u_ptr <= Cmd::Mov) || (*u_ptr >= Cmd::Jmp && *u_ptr <= Cmd::Ja) || *u_ptr == Cmd::Call) {
        i_ptr = (int*)(buffer + 1);
        result.farg = *i_ptr;
        *counter += 4;

        if (*u_ptr == Cmd::Mov || *u_ptr == Cmd::Cmp ) {
            i_ptr = (int*)(buffer + 5);
            result.sarg = *i_ptr;
            *counter += 4;
        }

    }
    return result;
}



void Execute(const char* commands, size_t size, const char* in, const char* out) {
    FILE* in_ptr = fopen(in, "r");
    FILE* out_ptr = fopen(out, "w");

    UnkillableStack<int, IntViewer> stack(VARIABLE_NAME(stack));
    int registers[5] = {0, 0, 0, 0};
    int flag = 0;

    size_t j = 0;
    for (; j < size;) {
        Instruction instr = DecodeCommand(commands + j, &j);
        registers['e' - 'a'] = j;
        if (!ExecuteInstruction(stack, registers, &instr, in_ptr, out_ptr, j, flag)) {
            break;
        }
    }
}


int ExecuteInstruction(UnkillableStack<int, IntViewer>& stack, int* registers,
                       const Instruction* instr, FILE* in_ptr, FILE* out_ptr, size_t& j, int& flag) {
    int tmp = 0;
    int s_tmp = 0;
    switch (instr->instruction) {
    #include "CommandsDescription.h"
    }
    return 1;
}