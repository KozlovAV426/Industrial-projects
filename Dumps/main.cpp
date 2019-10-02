#include "Stack.h"

class IntPoison {
public:
    int operator() () {
        return -666;
    }
};

int main() {
    DefaultPoison<int> b;
    int a = f<int>();
    std::cout << a;

    UnkillableStack<int, IntPoison, std::hash<int>, 10> stack(VARIABLE_NAME(stack));
    for (size_t i = 0; i < 5; ++i) {
        stack.Push(i);
    }
    /*int* p = (int*)(&stack);
    *p = 0;
    stack.Push(0);*/
    stack.PrintData();
    return 0;
}