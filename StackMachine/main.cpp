#include "Assembly.h"
#include "Execute.h"
#include "Stack.h"


int main() {
    char* file_name = "../Commands/equation";
    Assembly(file_name, "../Commands/assembly");
    size_t sz = 0;
    char* commands;
    commands = ReadAssembly("../Commands/assembly", &sz);
    Execute(commands, sz, "../Commands/input", "../Commands/output");
}