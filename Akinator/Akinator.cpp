#include "Akinator.h"
#include <string_view>
#include <stack>
#include <cstring>



void Akinator::ReserveBuffer() {
    if (game_tree.string_buffer.capacity() < game_tree.string_buffer.size() + CAPACITY) {
        game_tree.string_buffer.reserve(STRING_CAPACITY * 2);
    }
}

Akinator::Akinator() {
    game_tree = Tree();
}

void Akinator::Train() {
    if (game_tree.tree_.empty()) {
        FirstLaunch();
    }
    Descent();
}

bool IsCorrectInput(const char* str) {
    return (!strcmp(str, "yes") || !strcmp(str, "no") || !strcmp(str, "rather yes") || !strcmp(str, "rather no"));
}

void Akinator::Descent() {
    size_t current_node = 0;
    int size = 0;
    size_t parent = 0;
    char str[CAPACITY];
    std::stack<std::pair<int, int>> nodes;
    nodes.push({0, 0});

    while (!nodes.empty()) {
        current_node = nodes.top().first;
        parent = nodes.top().second;
        nodes.pop();

        while (game_tree.tree_[current_node].left_child != 0 || game_tree.tree_[current_node].right_child != 0) {
            SayNode(current_node);
            PrintNode(current_node);
            putc('\n', stdout);

            ReadInput(str);

            while (!IsCorrectInput(str)) {
                printf("Incorrect input! Try again\n");
                ReadInput(str);
            }

            if (!strcmp(str, "yes") || !strcmp(str, "rather yes")) {
                if (game_tree.tree_[current_node].right_child != 0 && !strcmp(str, "rather yes")) {
                    nodes.push({game_tree.tree_[current_node].right_child, current_node});
                }

                if (game_tree.tree_[current_node].left_child != 0) {
                    parent = current_node;
                    current_node = game_tree.tree_[current_node].left_child;
                    continue;
                }

                if (nodes.empty()) {
                    AddObject(current_node, LEFT, str);
                    return;
                }

                current_node = nodes.top().first;
                parent = nodes.top().second;
                nodes.pop();

            } else if (!strcmp(str, "no") || !strcmp(str, "rather no")) {
                if (game_tree.tree_[current_node].left_child != 0 && !strcmp(str, "rather no")) {
                    nodes.push({game_tree.tree_[current_node].left_child, current_node});
                }

                if (game_tree.tree_[current_node].right_child != 0) {
                    parent = current_node;
                    current_node = game_tree.tree_[current_node].right_child;
                    continue;
                }

                if (nodes.empty()) {
                    AddObject(current_node, RIGHT, str);
                    return;
                }

                current_node = nodes.top().first;
                parent = nodes.top().second;
                nodes.pop();
            }
        }
        printf("Maybe it is a ");
        SayMessage("Maybe it is a ", 14);

        PrintNode(current_node);
        SayNode(current_node);
        putc('\n', stdout);

        ReadInput(str);

        if (strcmp(str, "yes") && nodes.empty()) {
            SayMessage("Sorry I don't know it", 21);
            printf("Type difference between my answer and your object\n");
            SayMessage("Type difference between my answer and your object", 49);

            size_t added_num = game_tree.tree_.size();
            InsertProperty(str, current_node, parent);

            printf("Type your object\n");
            SayMessage("Type your object", 16);

            ReHang(str, added_num, current_node);
        }

        else if (!strcmp(str, "yes")){
            printf("I won\n");
            SayMessage("I won", 5);
            return;
        }
    }
}

void Akinator::ReHang(char *str, size_t added_num, size_t current_node) {
    size_t prev_index = game_tree.string_buffer.size();

    int size = ReadInput(str);

    game_tree.string_buffer += std::basic_string_view(str, size);
    game_tree.tree_[added_num].left_child = game_tree.tree_.size();
    game_tree.tree_[added_num].right_child = current_node;

    game_tree.AddNode(0, 0, prev_index, size);

    game_tree.tree_[game_tree.tree_[added_num].right_child].parent = added_num;
    game_tree.tree_[game_tree.tree_[added_num].left_child].parent = added_num;

    game_tree.tree_[game_tree.tree_[added_num].left_child].depth = game_tree.tree_[added_num].depth + 1;
    game_tree.tree_[game_tree.tree_[added_num].right_child].depth = game_tree.tree_[added_num].depth + 1;

    UpdateHashTable(game_tree.tree_[added_num].left_child);
    UpdateHashTable(game_tree.tree_[added_num].right_child);

}

void Akinator::InsertProperty(char *str, size_t current_node, size_t parent) {
    size_t prev_index = game_tree.string_buffer.size();
    size_t added_num = game_tree.tree_.size();

    ReserveBuffer();

    int size = ReadInput(str);
    game_tree.string_buffer += std::basic_string_view(str, size);

    if (current_node == game_tree.tree_[parent].left_child) {
        game_tree.tree_[parent].left_child = added_num;
    }
    else {
        game_tree.tree_[parent].right_child = added_num;
    }

    game_tree.AddNode(0, 0, prev_index, size);
    game_tree.tree_[added_num].parent = parent;
    game_tree.tree_[added_num].depth = game_tree.tree_[parent].depth + 1;
}

void Akinator::FirstLaunch() {
    char str[CAPACITY];
    printf("Type the first question\n");

    size_t prev_index = game_tree.string_buffer.size();

    int size = ReadInput(str);
    game_tree.string_buffer += std::basic_string_view(str, size);

    game_tree.AddNode(0, 0, prev_index, size);

    AddObject(0, LEFT, str);
}

void Akinator::AddObject(size_t parent, size_t child, char* str) {
    size_t prev_index = game_tree.string_buffer.size();
    printf("Sorry I dont know it. Type your answer\n");
    SayMessage("Sorry I dont know it. Type your answer", 38);

    ReserveBuffer();
    int size = ReadInput(str);
    game_tree.string_buffer += std::basic_string_view(str, size);

    if (!child) {
        game_tree.tree_[parent].left_child = game_tree.tree_.size();
    }
    else {
        game_tree.tree_[parent].right_child = game_tree.tree_.size();
    }
    game_tree.AddNode(0, 0, prev_index, size);

    game_tree.tree_[game_tree.tree_.size() - 1].parent = parent;
    game_tree.tree_[game_tree.tree_.size() - 1].depth = game_tree.tree_[parent].depth + 1;

    UpdateHashTable(game_tree.tree_.size() - 1);



}

void Akinator::CompareWords(FILE* common, FILE* first, FILE* second) {
    printf("Type first object\n");
    SayMessage("Type first object", 17);

    char first_word[CAPACITY];
    char second_word[CAPACITY];

    int first_sz = ReadInput(first_word);

    printf("Type second object\n");
    SayMessage("Type second object", 18);
    int second_sz = ReadInput(second_word);

    first_word[first_sz] = 0;
    second_word[second_sz] = 0;

    int first_index = table[std::string_view((const char*)first_word, first_sz)];
    int second_index = table[std::string_view((const char*)second_word, second_sz)];

    int common_parent = FindLCA(first_index, second_index);

    printf("%s and %s are: ", first_word, second_word);
    fprintf(common, "%s and %s are ", first_word, second_word);

    PrintCommon(game_tree.tree_[common_parent].parent, common_parent, 0, common);

    printf("\nDifferences\n%s is: ", first_word);
    fprintf(first, "%s is", first_word);
    PrintCommon(game_tree.tree_[first_index].parent, first_index, common_parent, first);

    printf("\n%s is ", second_word);
    fprintf(second, "%s is", second_word);
    PrintCommon(game_tree.tree_[second_index].parent, second_index, common_parent, second);
    printf("\n");
}

void Akinator::PrintCommon(int node, int prev, int limit, FILE* output) {
    if (node != limit) {
        PrintCommon(game_tree.tree_[node].parent, node, limit, output);
    }

    if (game_tree.tree_[node].right_child == prev && prev != 0) {
        printf("not ");

        fprintf(output, "not ");
    }

    if (prev != 0) {
        PrintNode(node);
        SaveVoiceText(node, output);
    }

    putc(',', stdout);
    putc(' ', stdout);
}

void Akinator::SaveVoiceText(int node, FILE *output) {
    size_t index = game_tree.tree_[node].index;
    size_t len = game_tree.tree_[node].size;

    for (size_t i = 0; i < len; ++i) {
        fputc(game_tree.string_buffer[index + i], output);
    }

    fputc('\n', output);
}


int Akinator::FindLCA(int first, int second) {
    while (game_tree.tree_[second].depth < game_tree.tree_[first].depth) {
        first = game_tree.tree_[first].parent;
    }

    while (game_tree.tree_[second].depth > game_tree.tree_[first].depth) {
        second = game_tree.tree_[second].parent;
    }

    while (game_tree.tree_[first].parent != game_tree.tree_[second].parent) {
        first = game_tree.tree_[first].parent;
        second = game_tree.tree_[second].parent;
    }

    return game_tree.tree_[first].parent;
}

void Akinator::PrintNode(size_t node) {
    size_t index = game_tree.tree_[node].index;
    size_t len = game_tree.tree_[node].size;
    for (size_t i = 0; i < len; ++i) {
        putc(game_tree.string_buffer[index + i], stdout);
    }
}

void Akinator::SayNode(size_t node) {
    char text[CAPACITY];

    size_t index = game_tree.tree_[node].index;
    size_t len = game_tree.tree_[node].size;

    for (size_t i = 0; i < len; ++i) {
        text[i] = game_tree.string_buffer[index + i];
    }
    SayMessage(text, len);
}


void Akinator::UpdateHashTable(int node) {
    table[std::string_view(game_tree.string_buffer.data() + game_tree.tree_[node].index,
                           game_tree.tree_[node].size)] = node;
}
