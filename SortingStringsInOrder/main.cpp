#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<vector>
#include<ctype.h>
#include<cassert>

#define BEGIN 1
#define END -1


struct OpenedString {
    char* ptr_;
    size_t len_ = 0;
    int movement_ = 0;
};

void PrintString(const OpenedString* string, FILE* output);

int Cmp(const void* farg, const void* sarg);

void ShiftToEnd(OpenedString* string);

void ShiftToEndInArray(OpenedString* strings, size_t len);

void ShiftToBegin(OpenedString* string);

void ShiftToBeginInArray(OpenedString* strings, size_t len);

bool IsLetter(const char* symbol);

char* ReadBuffer(FILE* file_ptr, size_t* len);

void SetParameters(OpenedString* strings, size_t size, int param);

void PrintBuffer(const char* buffer, size_t size, FILE* output);

OpenedString* ReadStrings(char* buffer, size_t* len);

size_t CountStrings(char* buffer, size_t* len);

size_t CountFileSize(FILE* file_ptr);

void SortStringsInOrder(FILE* input, FILE* output);

void WriteToFile(const OpenedString* strings, size_t size, FILE* output);

int main() {
    FILE* input  = fopen("../input", "r+"); assert(input  != nullptr);
    FILE* output = fopen("../output", "w"); assert(output != nullptr);

    SortStringsInOrder(input, output);

    fclose(input);
    fclose(output);
    return 0;
}

void SortStringsInOrder(FILE* input, FILE* output) {
    size_t len = 0;
    char *buffer = ReadBuffer(input, &len); assert(buffer != nullptr);
    size_t size = CountStrings(buffer, &len);

    OpenedString *list_of_strings = ReadStrings(buffer, &len); assert(list_of_strings != nullptr);

    SetParameters(list_of_strings, size, BEGIN);
    qsort(list_of_strings, size, sizeof(OpenedString), Cmp);
    WriteToFile(list_of_strings, size, output);

    ShiftToEndInArray(list_of_strings, size);

    SetParameters(list_of_strings, size, END);
    qsort(list_of_strings, size, sizeof(OpenedString), Cmp);

    ShiftToBeginInArray(list_of_strings, size);
    WriteToFile(list_of_strings, size, output);

    PrintBuffer(buffer, len, output);

    delete buffer;
    delete list_of_strings;
}

void WriteToFile(const OpenedString* strings, size_t size, FILE* output) {
    assert(strings != nullptr && output != nullptr);
    for (size_t i = 0; i < size; ++i) {
        PrintString(strings + i, output);
    }
    for (size_t i = 0; i < 5; ++i) {
        fputc('\n', output);
    }
}

void ShiftToEndInArray(OpenedString* strings, size_t len) {
    assert(strings != nullptr);
    for (size_t i = 0; i < len; ++i) {
        ShiftToEnd(strings + i);
    }
}

void ShiftToBeginInArray(OpenedString* strings, size_t len) {
    assert(strings != nullptr);
    for (size_t i = 0; i < len; ++i) {
        ShiftToBegin(strings + i);
    }
}

void PrintBuffer(const char* buffer, size_t size, FILE* output) {
    assert(buffer != nullptr && output != nullptr);
    for (size_t i = 0; i < size; ++i) {
        (*(buffer + i) == 0) ? fputc('\n', output) : fputc(*(buffer + i), output);
    }
}

void ShiftToEnd(OpenedString* string) {
    assert(string != nullptr);
    string->ptr_ += (string->len_ - 1);
}

void ShiftToBegin(OpenedString* string) {
    assert(string != nullptr);
    string->ptr_ -= string->len_ - 1;
}

void PrintString(const OpenedString* string, FILE* output) {
    assert(string != nullptr && output != nullptr);
    size_t size = string->len_;
    if (size) {
        for (size_t i = 0; i < size; ++i) {
            fputc(*(string->ptr_ + i), output);
        }
    }
    fputc('\n', output);
}

void SetParameters(OpenedString* strings, size_t size, int param) {
    assert(strings != nullptr);
    for (size_t i = 0; i < size; ++i) {
        strings[i].movement_ = param;
    }
}

OpenedString* ReadStrings(char* buffer, size_t* len) {
    assert(buffer != nullptr);
    size_t size = CountStrings(buffer, len);
    size_t j = 0;
    size_t str_size = 0;
    OpenedString* strings = new OpenedString[size];
    bool find = false;
    for (size_t i = 0; i < *(len); ++i) {
        if (j >= size) {
            break;
        }
        if (!find) {
            strings[j].ptr_ = buffer + i;
            find = true;
        }
        if (buffer[i] != '\n') {
            str_size += 1;
        }
        if (buffer[i] == '\n') {
            strings[j].len_ = str_size;
            if (str_size) {
                j += 1;
            }
            buffer[i] = 0;
            str_size = 0;
            find = false;
        }
    }
    return strings;
}

char* ReadBuffer(FILE* file_ptr, size_t* len) {
    assert(file_ptr != nullptr);
    size_t file_size = CountFileSize(file_ptr);
    *len = file_size / sizeof(char) + 1;
    char* buffer = new char[sizeof(char) * (*len)];
    fseek(file_ptr, 0, SEEK_SET);
    fread(buffer, sizeof(char), *len, file_ptr);
    buffer[*len - 1] = 10;
    return buffer;
}

size_t CountFileSize(FILE* file_ptr) {
    assert(file_ptr != nullptr);
    fseek(file_ptr, 0, SEEK_END);
    size_t file_size = ftell(file_ptr);
    return file_size;
}


size_t CountStrings(char* buffer, size_t* len) {
    assert(buffer != nullptr);
    bool find = false;
    size_t counter = 0;
    for (size_t i = 0; i < (*len); ++i) {
        if (buffer[i] != '\n' && !find) {
            counter += 1;
            find = true;
        }
        if (buffer[i] == '\n') {
            find = false;
        }
    }
    return counter;
}


int Cmp(const void* farg, const void* sarg) {
    size_t i = 0;  assert(farg != nullptr);
    size_t j = 0;  assert(sarg != nullptr);
    const OpenedString* lhv = (const OpenedString*)farg; assert(lhv != nullptr);
    const OpenedString* rhv = (const OpenedString*)sarg; assert(rhv != nullptr);
    size_t lhv_size = lhv->len_;
    size_t rhv_size = rhv->len_;
    int condition = lhv->movement_;
    while(i < lhv_size && j < rhv_size) {
        if (IsLetter(lhv->ptr_ + condition * i) && IsLetter(rhv->ptr_ + condition * j)) {
            if (tolower(*(lhv->ptr_ + condition * i)) < tolower(*(rhv->ptr_ + condition * j))) {
                return -1;
            }
            if (tolower(*(lhv->ptr_ + condition * i)) > tolower(*(rhv->ptr_ + condition * j))) {
                return 1;
            }
            i += 1;
            j += 1;
        }
        if (!IsLetter((lhv->ptr_ + condition * i))) {
            i += 1;
        }
        if (!IsLetter(rhv->ptr_ + condition * j)) {
            j += 1;
        }
    }
    if (i >= lhv_size) {
        return -1;
    }
    else {
        return 1;
    }
}


bool IsLetter(const char* symbol) {
    if (tolower(*symbol) <= 'z' && tolower(*symbol) >= 'a') {
        return true;
    }
    else {
        return false;
    }
}