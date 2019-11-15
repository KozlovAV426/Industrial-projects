#include "Akinator.h"
#include <string_view>

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

void Akinator::Descent() {
    size_t current_node = 0;
    char answer = ' ';
    size_t parent = 0;
    char str[CAPACITY];

    while (game_tree.tree_[current_node].left_child != 0 || game_tree.tree_[current_node].right_child != 0) {
        PrintNode(current_node);
        putc('\n', stdout);

        ReadAnswer(&answer);

        while (answer != 'y' && answer != 'n') {
            printf("Incorrect input! Try again\n");
            ReadAnswer(&answer);
        }

        if (answer == 'y') {
            if (game_tree.tree_[current_node].right_child != 0) {
                parent = current_node;
                current_node = game_tree.tree_[current_node].right_child;
                continue;
            }
            AddObject(current_node, RIGHT, str);
            return;
        }
        else {
            if (game_tree.tree_[current_node].left_child != 0) {
                parent = current_node;
                current_node = game_tree.tree_[current_node].left_child;
                continue;
            }
            AddObject(current_node, LEFT, str);
            return;
        }
    }
    printf("Maybe it is a ");
    PrintNode(current_node);
    putc('\n', stdout);

    ReadAnswer(&answer);

    if (answer != 'y') {
        printf("Type difference between my answer and your object\n");
        size_t added_num = game_tree.tree_.size();
        InsertProperty(str, current_node, parent);

        printf("Type your object\n");

        ReHang(str, added_num, current_node);
    }
    else {
        printf("I won\n");
    }
    printf("End of session\n\n");
}

void Akinator::ReHang(char *str, size_t added_num, size_t current_node) {
    size_t prev_index = game_tree.string_buffer.size();

    int size = ReadInput(str);

    game_tree.string_buffer += std::basic_string_view(str, size);
    game_tree.tree_[added_num].right_child = game_tree.tree_.size();
    game_tree.tree_[added_num].left_child = current_node;

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

    AddObject(0, RIGHT, str);
}

void Akinator::AddObject(size_t parent, size_t child, char* str) {
    size_t prev_index = game_tree.string_buffer.size();

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

    char first_word[CAPACITY];
    char second_word[CAPACITY];

    int first_sz = ReadInput(first_word);

    printf("Type second object\n");
    int second_sz = ReadInput(second_word);

    first_word[first_sz] = 0;
    second_word[second_sz] = 0;

    int first_index = table[std::string_view((const char*)first_word, first_sz)];
    int second_index = table[std::string_view((const char*)second_word, second_sz)];

    int common_parent = FindLCA(first_index, second_index);

    printf("%s and %s are: ", first_word, second_word);
    fprintf(common, "%s and %s are ", first_word, second_word);

    PrintCommon(game_tree.tree_[common_parent].parent, common_parent, 0, common);

    printf("\nDifferences\n%s is/are: ", first_word);
    fprintf(first, "%s is/are", first_word, first_word);
    PrintCommon(game_tree.tree_[first_index].parent, first_index, common_parent, first);

    printf("\n%s is/are: ", second_word);
    fprintf(second, "%s is/are", first_word, second_word);
    PrintCommon(game_tree.tree_[second_index].parent, second_index, common_parent, second);
    printf("\n");
}

void Akinator::PrintCommon(int node, int prev, int limit, FILE* output) {
    if (node != limit) {
        PrintCommon(game_tree.tree_[node].parent, node, limit, output);
    }

    if (game_tree.tree_[node].left_child == prev && prev != 0) {
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


void Akinator::UpdateHashTable(int node) {
    table[std::string_view(game_tree.string_buffer.data() + game_tree.tree_[node].index,
                           game_tree.tree_[node].size)] = node;
}
