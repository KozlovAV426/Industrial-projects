#include "Tree.h"
#include "Config.h"
#include <cstring>

Tree::Tree()
    : tree_(std::vector<Node>())
    , string_buffer(std::string())
    {
        tree_.reserve(NODES_NUMBER);
        string_buffer.reserve(STRING_CAPACITY);
    }

Tree::Node::Node(size_t left_child, size_t right_child, size_t index, size_t size)
    : left_child(left_child)
    , right_child(right_child)
    , index(index)
    , size(size)
    , parent(0)
    , depth(0)
    {}

void Tree::AddNode(size_t left, size_t right, size_t index, size_t size) {
    tree_.emplace_back(left, right, index, size);
}

void FillSpace(size_t n, FILE* ptr) {
    for (size_t i = 0; i < n; ++i) {
        fputc(' ', ptr);
    }
}

void ReadAnswer(char* answer) {
    *answer = getc(stdin);
    while (*answer != 'y' && *answer != 'n') {
        *answer = getc(stdin);
    }
    getc(stdin);
}

int ReadInput(char* str) {
    fgets(str, CAPACITY, stdin);
    int size = 0;
    while (str[size] != '\n') {
        size += 1;
    }
    str[size] = 0;
    return size;
}

void SayMessage(const char* text, size_t sz) {
    uint8_t len = 6;

    char message[CAPACITY * 2] = "echo \"";
    strcpy(message + len, text);

    char rest[CAPACITY] = "\" | festival --tts";
    strcpy(message + len + sz, rest);

    system(message);
}


