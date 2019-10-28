#include "Assembly.h"
#include "Global.h"
#include "Stack.h"
#include <unordered_map>


Instruction ParseCommandAndArgument(const char* command, std::unordered_map<std::string, int>& table);

void WriteCommand(FILE* out_ptr, const Instruction* instr);

void Disassembly(const char* asm_file, const char* out_file);

int ExecuteInstruction(UnkillableStack<int, IntViewer>& stack, int* registers,
                       const Instruction* instr, FILE* in_ptr, FILE* out_ptr, size_t& j, int& flag);



void FillCommandsList(char* text, const char* command, size_t& j, Instruction& instr,
        std::unordered_map<std::string, int>& table);

void FillJmps(FILE* in_ptr, char* text, std::unordered_map<std::string, int>& table);


void Assembly(const char* file_name, const char* out_file) {

    FILE *in_ptr = fopen(file_name, "r");
    FILE *out_ptr = fopen(out_file, "wb");


    fseek(in_ptr, 0, SEEK_END);
    int size = ftell(in_ptr);
    char *text = new char[size * sizeof(int)];
    size_t j = 0;

    std::unordered_map<std::string, int> table;

    char list[LINE_SIZE];
    for (size_t i = 0; i < LINE_SIZE; ++i) {
        list[i] = 0;
    }

    fseek(in_ptr, 0, SEEK_SET);

    while (fgets(list, MAX_SHIFT, in_ptr)) {
        for (size_t i = 0; i < LINE_SIZE; ++i) {
            if (list[i] == ' ' || list[i] == '\n' || list[i] == EOF) {
                list[i] = 0;
            }
        }
        if (list[0] != 0) {
            Instruction instr = ParseCommandAndArgument(list, table);
            FillCommandsList(text, list, j, instr, table);
        }
    }
    FillJmps(in_ptr, text, table);

    fwrite(text, sizeof(char), j, out_ptr);

    delete text;

    fclose(out_ptr);
    fclose(in_ptr);

}

void FillJmps(FILE* in_ptr, char* text, std::unordered_map<std::string, int>& table) {

    fseek(in_ptr, 0, SEEK_SET);
    size_t j = 0;

    char list[LINE_SIZE];
    for (size_t i = 0; i < LINE_SIZE; ++i) {
        list[i] = 0;
    }

    while (fgets(list, MAX_SHIFT, in_ptr)) {
        for (size_t i = 0; i < LINE_SIZE; ++i) {
            if (list[i] == ' ' || list[i] == '\n' || list[i] == EOF) {
                list[i] = 0;
            }
        }
        if (list[0] != 0) {
            Instruction instr = ParseCommandAndArgument(list, table);
            if (instr.instruction < Cmd::Jmp || instr.instruction == Cmd::Cmp || instr.instruction == Cmd::Ret
            || instr.instruction >= Cmd::Sqrt) {
                j += 1;
                if (instr.instruction > Cmd::End && instr.instruction <= Cmd::Pop) {
                    j += 4;
                }
                else if (instr.instruction == Cmd::Mov || instr.instruction == Cmd::Cmp) {
                    j += 8;
                }
            }
            else {
                if (instr.instruction == Cmd::Jmp || instr.instruction >= Cmd::Jz) {
                    j += 1;
                    size_t arg_index = strlen(list) + 1;
                    std::string label(list + arg_index);
                    label += ":";
                    int* ptr = (int*)(text + j);
                    *ptr = table[label];
                    j += 4;
                }

            }
        }
    }

}

void FillCommandsList(char* text, const char* command, size_t& j, Instruction& instr,
        std::unordered_map<std::string, int>& table) {
    uint8_t * u_ptr = nullptr;
    int* i_ptr = nullptr;

    if (instr.instruction == Label) {
        std::string label(command);
        table[label] = j;
        return;
    }

    u_ptr = (uint8_t *) (text + j);
    *u_ptr = instr.instruction;
    j += 1;

    if ((instr.instruction > Cmd::End && instr.instruction <= Cmd::Pop) || instr.instruction == Cmd::Jmp
    || (instr.instruction >= Cmd::Jz && instr.instruction <= Cmd::Ja) || instr.instruction == Cmd::Call) {
        i_ptr = (int *) (text + j);
        *i_ptr = instr.farg;
        j += 4;
    }
    if (instr.instruction == Cmd::Mov || instr.instruction == Cmd::Cmp) {
        i_ptr = (int *) (text + j);
        *i_ptr = instr.farg;
        j += 4;
        i_ptr = (int *) (text + j);
        *i_ptr = instr.sarg;
        j += 4;
    }


}


Instruction ParseCommandAndArgument(const char* command, std::unordered_map<std::string, int>& table) {
    char* end;
    size_t arg_index = strlen(command) + 1;
    Instruction code;

    if (!strcmp(command, "push")) {
        if (command[arg_index] >= 'a' && command[arg_index] <= 'e') {
            code.instruction = 1;
            code.farg = command[arg_index] - 'a';
        }
        else {
            int arg = strtol(command + arg_index, &end, 10);
            code.instruction = 2;
            code.farg = arg;
        }
    }
    else if (!strcmp(command, "pop")) {
        if (command[arg_index] >= 'a' && command[arg_index] <= 'd') {
            code.instruction = 3;
            code.farg = command[arg_index] - 'a';
        }
    }
    else if (!strcmp(command, "mov")) {
        if (command[arg_index] >= 'a' && command[arg_index] <= 'd') {
            if (command[arg_index + 4] >= 'a' && command[arg_index + 4] <= 'd') {
                code.instruction = 4;
                code.farg = command[arg_index] - 'a';
                code.sarg = command[arg_index + 4] - 'a';
            }
        }
    }
    else if (!strcmp(command, "add")) {
        code.instruction = 5;
    }
    else if (!strcmp(command, "mul")) {
        code.instruction = 6;
    }
    else if (!strcmp(command, "sub")) {
        code.instruction = 7;
    }
    else if (!strcmp(command, "div")) {
        code.instruction = 8;
    }
    else if (!strcmp(command, "in")) {
        code.instruction = 9;
    }
    else if (!strcmp(command, "out")) {
        code.instruction = 10;
    }
    else if (!strcmp(command, "end")) {
        code.instruction = 0;
    }
    else if (!strcmp(command, "jmp")) {
        code.instruction = 11;

    }
    else if (!strcmp(command, "cmp")) {
        code.instruction = 13;
        code.farg = command[arg_index] - 'a';
        int arg = strtol(command + arg_index + 4, &end, 10);
        code.sarg = arg;

    }
    else if (!strcmp(command, "jz")) {
        code.instruction = 14;
    }
    else if (!strcmp(command, "jl")) {
        code.instruction = 15;
    }
    else if (!strcmp(command, "ja")) {
        code.instruction = 16;
    }
    else if (!strcmp(command, "ret")) {
        code.instruction = 17;
    }
    else if (!strcmp(command, "call")) {
        code.instruction = 18;
    }
    else if (!strcmp(command, "sqrt")) {
        code.instruction = 19;
    }
    else if (!strcmp(command, "out_f")) {
        code.instruction = 20;
    }
    else {
        code.instruction = 12;
    }
    return code;
}

void WriteCommand(FILE* out_ptr, const Instruction* instr) {
    switch (instr->instruction) {
        case Cmd::Push_reg : fprintf(out_ptr, "push "); break;
        case Cmd::Push_num : fprintf(out_ptr, "push "); break;
        case Cmd::Pop      : fprintf(out_ptr, "pop ");  break;
        case Cmd::Mov      : fprintf(out_ptr, "mov ");  break;
        case Cmd::Add      : fprintf(out_ptr, "add ");  break;
        case Cmd::Mul      : fprintf(out_ptr, "mul ");  break;
        case Cmd::Sub      : fprintf(out_ptr, "sub ");  break;
        case Cmd::Div      : fprintf(out_ptr, "div ");  break;
        case Cmd::In       : fprintf(out_ptr, "in ");   break;
        case Cmd::Out      : fprintf(out_ptr, "out "); break;
        case Cmd::End      : fprintf(out_ptr, "end ");  break;
    }
    if (instr->instruction > Cmd::End && instr->instruction <= Cmd::Mov) {
        char c[3] = "ax";
        char d[3] = "ax";
        switch (instr->farg) {
            case 0 :
                break;
            case 1 :
                c[0] += 1;
                break;
            case 2 :
                c[0] += 2;
                break;
            case 3 :
                c[0] += 3;
                break;
        }
        switch (instr->sarg) {
            case 0 :
                break;
            case 1 :
                d[0] += 1;
                break;
            case 2 :
                d[0] += 2;
                break;
            case 3 :
                d[3] += 3;
                break;
        }
        if (instr->instruction != Cmd::Mov) {
            fprintf(out_ptr, "%s ", c);
        } else {
            fprintf(out_ptr, "%s, %s", c, d);
        }
    }
    fprintf(out_ptr, "\n");
}




