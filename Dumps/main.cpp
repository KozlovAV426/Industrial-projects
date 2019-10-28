#include "Stack.h"
#include "gtest/gtest.h"



int main() {
    UnkillableStack<int, IntViewer> st(VARIABLE_NAME(st));
    /**(reinterpret_cast<char*>(&st) + sizeof(int) * 4 +
      sizeof(std::hash<int>) + sizeof(IntViewer) + sizeof(int*)) = 100500;*/
    st.Push(1);

//    testing::InitGoogleTest();

//    return RUN_ALL_TESTS();
}