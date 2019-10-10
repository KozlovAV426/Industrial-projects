#include "../Stack.h"
#include "gtest/gtest.h"

/*TEST(TestingCanaryDeletion, CanaryDeletion) {
    UnkillableStack<int, IntViewer> my_stack(VARIABLE_NAME(my_stack));
    int* ptr = (int*)&my_stack;
    *ptr = 5;
    my_stack.Top();
}*/

/*TEST(TestingHash, HashChecker) {
    UnkillableStack<int, IntViewer> my_stack(VARIABLE_NAME(my_stack));
    int* poison = (int*)&my_stack;
    char* ptr = (char*)&my_stack;
    for (size_t i = 1; i < sizeof(UnkillableStack<int, IntViewer>); ++i) {
        int* t = (int*)(ptr + i);
        if ((*t) == *poison) {
            poison = t;
            break;
        }
    }
    ptr = ((char*)(poison) - sizeof(std::string) - sizeof(size_t*));
    size_t** ptr_st = (size_t**)(ptr);
    **ptr_st = 0;
    my_stack.Top();
}*/

TEST(TestingDestructor, DestructorChecker) {
    UnkillableStack<int, IntViewer> my_stack(VARIABLE_NAME(my_stack));
    my_stack.Push(3);
}