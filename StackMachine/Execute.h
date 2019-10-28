#include "Assembly.h"
#include "Global.h"

char* ReadAssembly(const char* asm_file, size_t* sz);

Instruction DecodeCommand(const char* buffer, size_t* counter);

void Execute(const char* commands, size_t size, const char* in, const char* out);