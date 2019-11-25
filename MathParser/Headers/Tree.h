#include <vector>
#include <stdint.h>
#include <stdio.h>

class Tree {
    friend class TreeManager;

public:
    class Node {
    public:
        Node* left;
        Node* right;
        Node* parent;

        int value;
        bool is_constant;
        int func_number;

        Node(Node* left, Node* right, Node* parent);
        Node(const Node& other) {
            value = other.value;
            is_constant = other.is_constant;
            func_number = other.func_number;
        }
    };

    void DeleteSubTree(Node* node);

    void Rehang(Node* node);

    Node* CopySubTree(Node* node);

    void CopyNode(Node* node, Node** current);

    void PreOrder(Node* node, Node* parent, char* function, FILE* ptr, char* symbol);

    void Calculate(Node* node);


public:
    Node* root;
    Tree();

    void BuildTree(const char* filename);

    void CalculateConstants(Node* node, bool& res);

    void SimplifyByOne(Node* node, bool& res);

    void SimplifyByZero(Node* node, bool& res);

    Node* Differentiate(Node* node);
};

bool IsDigit(const char* arg, int* value);

uint8_t DecodeFunction(char* arg);
