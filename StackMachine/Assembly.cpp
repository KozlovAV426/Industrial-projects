#include "Assembly.h"
#include "Stack.h"
#include "Global.h"
#include "CommandClass.h"
#include <unordered_map>


Instruction ParseCommandAndArgument(const char* command, std::unordered_map<std::string, int>& table);


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
    bool no_label = false;
    for (auto cmd : ALL_COMMANDS) {
        if (!strcmp(command, cmd.name)) {
            no_label = true;
            code.instruction = cmd.code;

            uint8_t args_n = 0;
            int arg = 0;

            while (args_n < cmd.argument_numbers) {
                if (command[arg_index] >= 'a' && command[arg_index] <= 'e') {
                    arg = command[arg_index] - 'a';
                }
                else {
                    arg = strtol(command + arg_index, &end, 10);
                }
                (args_n) ? code.sarg = arg : code.farg = arg;
                ++args_n;
                arg_index += 4;
            }
        }
    }
    if (!no_label) code.instruction = Cmd::Label;
    return code;
}




