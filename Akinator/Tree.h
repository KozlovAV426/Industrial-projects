#include <iostream>
#include <vector>
#include "Config.h"

class Tree {
private:
    class Node {
    public:
        size_t left_child;
        size_t right_child;
        size_t index;
        size_t size;
        int parent;
        int depth;

        Node(size_t left_child, size_t right_child, size_t index, size_t size);
    };

public:
    std::vector<Node> tree_;
    std::string string_buffer;

    Tree();

    void AddNode(size_t left, size_t right, size_t index, size_t size);

};

void FillSpace(size_t n, FILE* ptr);

void ReadAnswer(char* answer);

int ReadInput(char* str);

void SayMessage(const char* text, size_t sz);
