#include "Stack.h"
#include "gtest/gtest.h"



int main() {
    UnkillableStack<int, IntViewer> st(VARIABLE_NAME(st));
    for (size_t i = 0; i < 2 << 10; ++i) {
        st.Push(1);
    }
    st.~UnkillableStack<int, IntViewer>();

//    testing::InitGoogleTest();

//    return RUN_ALL_TESTS();
}