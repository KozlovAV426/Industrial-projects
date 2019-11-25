#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>

#include "../Headers/Tree.h"
#include "../Headers/Constants.h"

Tree::Node* Tree::CopySubTree(Tree::Node *node) {
    Node* copy = new Node(*node);
    if (node->left) {
        CopyNode(node->left, &copy->left);
        copy->left->parent = copy;
    }

    if (node->right) {
        CopyNode(node->right, &copy->right);
        copy->right->parent = copy;
    }

    return copy;
}

void Tree::CopyNode(Tree::Node *node, Tree::Node** current) {
    *current = new Node(*node);

    if (node->left) {
        CopyNode(node->left, &(*current)->left);
        (*current)->left->parent = *current;
    }

    if (node->right) {
        CopyNode(node->right, &(*current)->right);
        (*current)->right->parent = *current;
    }

}


Tree::Node* Tree::Differentiate(Tree::Node *node) {
    Node* begin = new Node(nullptr, nullptr, nullptr);
    if (node->func_number == ADD) {
        begin->func_number = ADD;
        begin->left = Differentiate(node->left);
        begin->right = Differentiate(node->right);
    }

    else if (node->func_number == MUL && (node->left->is_constant || node->right->is_constant)) {
        begin->func_number = MUL;
        if (node->left->is_constant) {
            begin->left = CopySubTree(node->left);
            begin->right = Differentiate(node->right);
        }
        else {
            begin->right = CopySubTree(node->right);
            begin->left = Differentiate(node->left);
        }
        begin->left->parent = begin;
        begin->right->parent = begin;
    }

    else if (node->func_number == MUL) {
        begin->func_number = ADD;

        begin->left = new Node(nullptr, nullptr, begin);
        begin->left->func_number = MUL;
        begin->left->right = CopySubTree(node->right);
        begin->left->left = Differentiate(node->right);

        begin->right = new Node(nullptr, nullptr, begin);
        begin->right->func_number = MUL;
        begin->right->left = CopySubTree(node->left);
        begin->right->right = Differentiate(node->right);

    }

    else if (node->func_number == POW) {
        Node* second = new Node(nullptr, nullptr, begin);
        second->func_number = MUL;
        second->left = CopySubTree(node->right);
        second->left->parent = second;

        second->right = new Node(nullptr, nullptr, second);
        second->right->func_number = POW;
        second->right->left = CopySubTree(node->left);
        second->right->right = CopySubTree(node->right);
        second->right->right->parent = second->right;
        second->right->right->value = node->right->value - 1;

        begin->func_number = MUL;
        begin->right = second;
        begin->left = Differentiate(node->left);
        begin->left->parent = begin;

    }

    else if (node->func_number == SIN) {
        Node* second = new Node(nullptr, nullptr, begin);
        second->func_number = COS;
        second->right = CopySubTree(node->right);
        second->right->parent = second;

        begin->func_number = MUL;
        begin->left = second;

        begin->right = Differentiate(node->right);
        begin->right->parent = begin;
    }

    else if (node->func_number == COS) {
        Node* second = new Node(nullptr, nullptr, begin);

        second->func_number = MUL;
        second->right = new Node(nullptr, nullptr, second);
        second->right->func_number = SIN;
        second->right->right = CopySubTree(node->right);
        second->right->right->parent = second->right;

        second->left = new Node(nullptr, nullptr, second);
        second->left->is_constant = true;
        second->left->value = -1;


        begin->func_number = MUL;
        begin->left = second;

        begin->right = Differentiate(node->right);
        begin->right->parent = begin;
    }

    else if (node->func_number == VARIABLE) {
        begin->is_constant = true;
        begin->value = 1;
    }

    else if (node->is_constant) {
        begin->is_constant = true;
        begin->value = 0;
    }

    return begin;

}


void Tree::Rehang(Tree::Node *node) {
    assert(node != nullptr);
    DeleteSubTree(node->left);

    if (node != root) {
        if (node == node->parent->left) {
            node->parent->left = node->right;
        }
        else {
            node->parent->right = node->right;
        }
        node->right->parent = node->parent;
        delete node;
    }

    else {
        delete root;
        root = node->right;
        root->left = nullptr;
        root->right = nullptr;
    }
}

void Tree::SimplifyByZero(Tree::Node *node, bool &res) {
    if (node->left) {
        SimplifyByZero(node->left, res);
    }

    if (node->right) {
        SimplifyByZero(node->right, res);
    }

    if (node->is_constant) return;

    if (node->left && node->right && node->func_number == MUL) {
        if (node->left->is_constant && node->left->value == 0) {
            std::swap(node->left, node->right);
            Rehang(node);
        }
        else if (node->right->is_constant && node->right->value == 0) {
            Rehang(node);
        }
    }

    if (node->left && node->right && node->func_number == DIV && node->left->value == 0) {
        std::swap(node->left, node->right);
        Rehang(node);
    }

}

void Tree::DeleteSubTree(Tree::Node *node) {
    if (node->left) {
        DeleteSubTree(node->left);
    }

    if (node->right) {
        DeleteSubTree(node->right);
    }
    delete node;

}

void Tree::SimplifyByOne(Tree::Node *node, bool &res) {
    if (node->left) {
        SimplifyByOne(node->left, res);
    }

    if (node->right) {
        SimplifyByOne(node->right, res);
    }

    if (node->is_constant) return;

    if (node->left && node->right && node->func_number == MUL) {
        if (node->left->is_constant && node->left->value == 1) {
            Rehang(node);
        }
        else if (node->right->is_constant && node->right->value == 1) {
            std::swap(node->left, node->right);
            Rehang(node);
        }
    }

    if (node->left && node->right && node->func_number == DIV && node->right->value == 1) {
        std::swap(node->left, node->right);
        Rehang(node);
    }

}




void Tree::CalculateConstants(Tree::Node *node, bool& res) {
    if (node->left) {
        CalculateConstants(node->left, res);
    }

    if (node->right) {
        CalculateConstants(node->right, res);
    }

    if (node->is_constant) return;

    if (node->left && node->right) {
        if (node->left->is_constant && node->right->is_constant) {
            Calculate(node);
            res = true;
        }
    }

}

void Tree::Calculate(Tree::Node *node) {
    if (node->func_number <= POW) {
        switch (node->func_number) {
            case ADD: {
                node->right->value += node->left->value;
                break;
            }
            case SUB: {
                node->right->value = node->left->value - node->right->value;
                break;
            }
            case MUL: {
                node->right->value *= node->left->value;
                break;
            }
            case DIV: {
                assert(node->right->value != 0);
                node->right->value = node->left->value / node->right->value;
                break;
            }
            case POW: {
                node->right->value = pow(node->left->value, node->right->value);
                break;
            }
        }
        Rehang(node);
    }
}

void Tree::PreOrder(Tree::Node *node, Tree::Node* parent, char* function, FILE *ptr, char* symbol) {
    while (*symbol == ' ' || *symbol == '\n') {
        *symbol = fgetc(ptr);
    }

    if (*symbol == '(') {
        node->left = new Node(nullptr, nullptr, nullptr);
        node->left->parent = node;

        *symbol = fgetc(ptr);
        PreOrder(node->left, node, function, ptr, symbol);
    }

    if (*symbol == ')') {
        return;
    }

    size_t  sz = 0;

    while (*symbol == ' ' || *symbol == '\n') {
        *symbol = fgetc(ptr);
    }

    while (*symbol != ')' && *symbol != '(') {
        function[sz] = *symbol;
        ++sz;
        *symbol = fgetc(ptr);
    }
    function[sz] = 0;

    int value = 0;
    if (IsDigit(function, &value)) {
        node->is_constant = true;
        node->value = value;
    }

    else {
        node->func_number = DecodeFunction(function);
    }

    if (*symbol == '(') {
        node->right = new Node(nullptr, nullptr, nullptr);
        node->right->parent = node;
        *symbol = fgetc(ptr);
        PreOrder(node->right, node, function, ptr, symbol);
    }

    *symbol = fgetc(ptr);
}


void Tree::BuildTree(const char *filename) {
    FILE* ptr = fopen(filename, "r");
    char function[256];

    char symbol;
    symbol = fgetc(ptr);
    while (symbol != '(' || symbol == ')') {
        symbol = fgetc(ptr);
    }
    symbol = fgetc(ptr);
    PreOrder(root, root, function, ptr, &symbol);
}

Tree::Tree()
    : root(new Node(nullptr, nullptr, nullptr))
    {}

Tree::Node::Node(Node* left, Node* right, Node* parent)
    : left(left)
    , right(right)
    , parent(parent)
    , value(0)
    , is_constant(false)
    , func_number(-1)
    {}


bool IsDigit(const char* arg, int* value) {
    char* end;
    *value = strtol(arg, &end, 10);
    return (!*end);

}

uint8_t DecodeFunction(char* arg) {
    size_t i = 0;
    while (arg[i] != 0) {
        if (arg[i] == ' ') {
            arg[i] = 0;
            break;
        }
        ++i;
    }

    for (size_t j = 0; j < FUNCTION_SIZE; ++j) {
        if (!strcmp(arg, FUNCTION_NAME[j])) {
            return j;
        }
    }
}