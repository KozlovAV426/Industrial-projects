#include "gtest/gtest.h"
#include "../Header.h"


TEST(TestingCountFileSize, FileSize) {
    FILE* file = fopen("../tests/count_file_size", "w");
    char sym = 'a';
    size_t i = 0;
    for ( ; i < 10; ++i) {
        fputc(sym, file);
        sym += 1;
    }
    EXPECT_EQ(i * sizeof(char), CountFileSize(file));
    for (size_t j = 0; j < 40; ++j) {
        fputc(sym, file);
        sym += 1;
        i += 1;
    }
    EXPECT_EQ(i * sizeof(char), CountFileSize(file));
    fclose(file);
}


TEST(TestingStringsInFile, CountStrings_Test) {
    char* first_str = "\n\n\n\n";
    size_t f_len = 4;
    char* second_str = "\n\na\n\na\n\n";
    size_t s_len = 8;
    EXPECT_EQ(0, CountStrings(first_str, &f_len));
    EXPECT_EQ(2, CountStrings(second_str, &s_len));

}

TEST(TestingReadBuffer, ReadBuffer_Test) {
    FILE* file = fopen("../tests/count_file_size", "w+");
    const char* text = "testing\nreading\nfunction";
    size_t text_size = 26;
    fprintf(file, text);
    char* buffer = ReadBuffer(file, &text_size);
    for (size_t i = 0; i < text_size - 1; ++i) {
        EXPECT_EQ(text[i], buffer[i]);
    }
    fclose(file);
    delete buffer;
}

TEST(TestingReadingStrings, ReadStrings_Test) {
    FILE* file = fopen("../tests/count_file_size", "r");
    size_t text_size = 0;
    const char* text = "testingreadingfunction";
    char* buffer = ReadBuffer(file, &text_size);
    size_t str_n = CountStrings(buffer, &text_size);
    OpenedString* strings = ReadStrings(buffer, &text_size);
    for (size_t i = 0; i < strings[0].len_; ++i) {
            EXPECT_EQ(strings[0].ptr_[i], text[i]);
    }
    fclose(file);
    delete buffer;
    delete strings;
}

TEST(TestingPrintToFile, Print_Test) {
    FILE* file = fopen("../tests/count_file_size", "w+");
    size_t text_size = 9;
    char* buffer = "testing\n";
    size_t str_n = CountStrings(buffer, &text_size);
    OpenedString* str = ReadStrings(buffer, &text_size);
    PrintString(str, file);
    buffer = ReadBuffer(file, &text_size);
    for (size_t i = 0; i < str[0].len_ - 1; ++i) {
        EXPECT_EQ(buffer[i], str[0].ptr_[i]);
    }
    fclose(file);
    delete buffer;
    delete str;
}

TEST(GetSign, GetSign_Test) {
    EXPECT_EQ(GetSign(-52), -1);
    EXPECT_EQ(GetSign(5), 1);
    EXPECT_EQ(GetSign(0), 0);
}

TEST(Cmp, Cmp_Test) {
    OpenedString lhv;
    OpenedString rhv;
    lhv.ptr_ = "abcd";
    lhv.len_ = 4;
    rhv.ptr_ = "bb";
    rhv.len_ = 2;
    EXPECT_EQ(Cmp(&lhv, &rhv), -1);
    EXPECT_EQ(Cmp(&rhv, &lhv), 1);
}
