

EXECUTE_COMMAND(Cmd::Push_reg, {stack.Push(registers[instr->farg]);})

EXECUTE_COMMAND(Cmd::Push_num, {stack.Push(instr->farg);})

EXECUTE_COMMAND(Cmd::Pop, {registers[instr->farg] = stack.Top(); stack.Pop();})

EXECUTE_COMMAND(Cmd::Mov, {registers[instr->farg] = registers[instr->sarg];})

EXECUTE_COMMAND(Cmd::Add,
        {tmp = stack.Top();
        stack.Pop();
        tmp += stack.Top();
        stack.Pop();
        stack.Push(tmp);}
        )

EXECUTE_COMMAND(Cmd::Mul,
        {tmp = stack.Top();
        stack.Pop();
        tmp *= stack.Top();
        stack.Pop();
        stack.Push(tmp);})

EXECUTE_COMMAND(Cmd::Sub,
        {tmp = stack.Top();
        stack.Pop();
        tmp -= stack.Top();
        stack.Pop();
        stack.Push(tmp);})

EXECUTE_COMMAND(Cmd::Div,
        {tmp = stack.Top();
        tmp *= (1000 * PRECISION);
        stack.Pop();
        s_tmp = stack.Top() * PRECISION;
        tmp /= s_tmp;
        stack.Pop();
        stack.Push(tmp);})

EXECUTE_COMMAND(Cmd::In, {fscanf(in_ptr, "%d", registers);})

EXECUTE_COMMAND(Cmd::Out, {fprintf(out_ptr, "%d\n", stack.Top());})

EXECUTE_COMMAND(Cmd::Jmp, {j = instr->farg;})

EXECUTE_COMMAND(Cmd::Cmp, {flag = registers[instr->farg] - instr->sarg;})

EXECUTE_COMMAND(Cmd::Jz, {if (!flag) { j = instr->farg; }})

EXECUTE_COMMAND(Cmd::Jl, {if (flag < 0) { j = instr->farg; }})

EXECUTE_COMMAND(Cmd::Ja, {if (flag > 0) { j = instr->farg; }})

EXECUTE_COMMAND(Cmd::Ret, {tmp = stack.Top(); stack.Pop(); j = tmp;})

EXECUTE_COMMAND(Cmd::Call, {stack.Push(registers['e' - 'a']); j = instr->farg;})

EXECUTE_COMMAND(Cmd::Sqrt, {tmp = sqrt(stack.Top()); stack.Pop(); stack.Push(tmp);})

EXECUTE_COMMAND(Cmd::Out_f, {fprintf(out_ptr, "%f\n", stack.Top() / DPRECISION);})

EXECUTE_COMMAND(Cmd::End, {return 0;})




