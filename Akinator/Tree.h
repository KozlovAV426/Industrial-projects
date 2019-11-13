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

        Node(size_t left_child, size_t right_child, size_t index, size_t size);
    };

public:
    std::vector<Node> tree_;
    std::string string_buffer;
    std::string input;

    Tree();

    void AddNode(size_t left, size_t right, size_t index, size_t size);

};

void ReadInput(char* str);