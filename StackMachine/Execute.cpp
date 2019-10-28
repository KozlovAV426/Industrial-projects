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
        case Cmd::Push_reg :
            stack.Push(registers[instr->farg]);
            break;
        case Cmd::Push_num :
            stack.Push(instr->farg);
            break;
        case Cmd::Pop :
            registers[instr->farg] = stack.Top();
            stack.Pop();
            break;
        case Cmd::Mov :
            registers[instr->farg] = registers[instr->sarg];
            break;
        case Cmd::Add :
            tmp = stack.Top();
            stack.Pop();
            tmp += stack.Top();
            stack.Pop();
            stack.Push(tmp);
            break;
        case Cmd::Mul :
            tmp = stack.Top();
            stack.Pop();
            tmp *= stack.Top();
            stack.Pop();
            stack.Push(tmp);
            break;
        case Cmd::Sub :
            tmp = stack.Top();
            stack.Pop();
            tmp -= stack.Top();
            stack.Pop();
            stack.Push(tmp);
            break;
        case Cmd::Div :
            tmp = stack.Top();
            tmp *= 1000000;
            stack.Pop();
            s_tmp = stack.Top() * 1000;
            tmp /= s_tmp;
            stack.Pop();
            stack.Push(tmp);
            break;
        case Cmd::In :
            fscanf(in_ptr, "%d", registers);
            break;
        case Cmd::Out :
            fprintf(out_ptr, "%d\n", stack.Top());
            break;
        case Cmd::Jmp:
            j = instr->farg;
            break;
        case Cmd::Cmp :
            flag = registers[instr->farg] - instr->sarg;
            break;
        case Cmd::Jz :
            if (!flag) {
                j = instr->farg;
            }
            break;
        case Cmd::Jl:
            if (flag < 0) {
                j = instr->farg;
            }
            break;
        case Cmd::Ja:
            if (flag > 0) {
                j = instr->farg;
            }
            break;
        case Cmd::Ret:
            tmp = stack.Top();
            stack.Pop();
            j = tmp;
            break;
        case Cmd::Call :
            stack.Push(registers['e' - 'a']);
            j = instr->farg;
            break;
        case Cmd::Sqrt :
            tmp = sqrt(stack.Top());
            stack.Pop();
            stack.Push(tmp);
            break;
        case Cmd::Out_f :
            fprintf(out_ptr, "%f\n", stack.Top() / 1000.0);
            break;
        case Cmd::End :
            return 0;
    }
    return 1;
}