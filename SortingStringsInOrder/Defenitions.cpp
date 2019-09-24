#include "Header.h"


const int FORWARD = 1;
const int BACKWARD = -1;

int MOVEMENT = 0;


void SortStringsInOrder(FILE* input, FILE* output) {
    size_t len = 0;
    char *buffer = ReadBuffer(input, &len);  assert(buffer != nullptr);
    size_t size = CountStrings(buffer, &len);

    OpenedString *list_of_strings = ReadStrings(buffer, &len); assert(list_of_strings != nullptr);

    MOVEMENT = FORWARD;
    qsort(list_of_strings, size, sizeof(OpenedString), Cmp);
    WriteToFile(list_of_strings, size, output);

    MOVEMENT = BACKWARD;
    ShiftToEndInArray(list_of_strings, size);
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


OpenedString* ReadStrings(char* buffer, size_t* len) {
    assert(buffer != nullptr);
    size_t size = CountStrings(buffer, len);
    size_t j = 0;
    size_t str_size = 0;
    bool find = true;
    OpenedString* strings = new OpenedString[size];
    for (size_t i = 0; i < *(len); ++i) {
        strings[j].ptr_ = buffer + i;
        if (buffer[i] != '\n') {
            if (find) {
                j += 1;
                find = false;
            }
            str_size += 1;
        }
        else {
            if (!find) {
                strings[j - 1].len_ = str_size;
            }
            str_size = 0;
            find = true;
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

    buffer[*len - 1] = '\n';
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
    while(i < lhv_size && j < rhv_size) {
        if (IsLetter(lhv->ptr_ + MOVEMENT * i) && IsLetter(rhv->ptr_ + MOVEMENT * j)) {
            int sign = GetSign(tolower(*(lhv->ptr_ + MOVEMENT * i)) - tolower(*(rhv->ptr_ + MOVEMENT * j)));
            if (sign) {
                return sign;
            }
            i += 1;
            j += 1;
        }
        if (!IsLetter((lhv->ptr_ + MOVEMENT * i))) {
            i += 1;
        }
        if (!IsLetter(rhv->ptr_ + MOVEMENT * j)) {
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

int GetSign(const int& number) {
    if (number > 0) {
        return 1;
    }
    else if (!number) {
        return 0;
    }
    else {
        return -1;
    }
}