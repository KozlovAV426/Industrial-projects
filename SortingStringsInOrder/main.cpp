#include "Header.h"
#include "gtest/gtest.h"

#define asserted || (({assert(false);}), false)

int main() {

    testing::InitGoogleTest();

    FILE* input  = fopen("../input", "r+"); assert(input  != nullptr);
    FILE* output = fopen("../output", "w"); assert(output != nullptr);

    SortStringsInOrder(input, output);

    (fclose(input) != EOF) asserted;
    (fclose(output) != EOF) asserted;
    return RUN_ALL_TESTS();
}

