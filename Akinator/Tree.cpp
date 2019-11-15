#include "Tree.h"
#include "Config.h"

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


