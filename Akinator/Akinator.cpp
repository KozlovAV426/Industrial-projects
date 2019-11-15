#include "Akinator.h"
#include <string_view>

void FillSpace(size_t n, FILE* ptr) {
    for (size_t i = 0; i < n; ++i) {
        fputc(' ', ptr);
    }
}

void ReadAnswer(char* answer) {
    *answer = getc(stdin);
    while (*answer == '\n') {
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
    return size;
}

Akinator::Akinator() {
    game_tree = Tree();
}

void Akinator::StartGame() {
    char answer = 0;
    while (answer != 'n') {
//        if (game_tree.tree_.empty()) {
//            FirstLaunch();
//        }
//        Descent();
        CompareWords();

        printf("Do you want to continue?\n");
        ReadAnswer(&answer);
    }
    SaveData("../data/data");
    CreateVisualization(PATH_TO_VISUAL);
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

void Akinator::CompareWords() {
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
    PrintCommon(game_tree.tree_[common_parent].parent, common_parent, 0);

    printf("\nDifferences\n%s is/are: ", first_word);
    PrintCommon(game_tree.tree_[first_index].parent, first_index, common_parent);

    printf("\n%s is/are: ", second_word);
    PrintCommon(game_tree.tree_[second_index].parent, second_index, common_parent);
    printf("\n");
}

void Akinator::PrintCommon(int node, int prev, int limit) {
    if (node != limit) {
        PrintCommon(game_tree.tree_[node].parent, node, limit);
    }

    if (game_tree.tree_[node].left_child == prev && prev != 0) {
        printf("not ");
    }

    if (prev != 0) PrintNode(node);

    putc(',', stdout);
    putc(' ', stdout);
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



void Akinator::SaveData(const char *file) {
    FILE* ptr = fopen(file, "w");
    fprintf(ptr, "{\n");

    FillSpace(SHIFT, ptr);
    fputc('"', ptr);
    for (size_t i = 0; i < game_tree.tree_[0].size; ++i) {
        fputc(game_tree.string_buffer[game_tree.tree_[0].index + i], ptr);
    }
    fputc('"', ptr);
    WriteFromNode(game_tree.tree_[0].left_child, ptr, SHIFT);
    WriteFromNode(game_tree.tree_[0].right_child, ptr, SHIFT);

    fputc('\n', ptr);
    FillSpace(SHIFT, ptr);
    fputc('}', ptr);

    fclose(ptr);

}

void Akinator::WriteFromNode(size_t node, FILE* ptr, size_t space_n) {
    space_n += SHIFT;

    if (!node) {
        fputc('{', ptr);
        fputc('}', ptr);
        return;
    }

    fputc('{', ptr);
    fputc('\n', ptr);

    FillSpace(space_n, ptr);

    fputc('"', ptr);
    for (size_t i = 0; i < game_tree.tree_[node].size; ++i) {
        fputc(game_tree.string_buffer[game_tree.tree_[node].index + i], ptr);
    }
    fputc('"', ptr);

    WriteFromNode(game_tree.tree_[node].left_child, ptr, space_n);
    WriteFromNode(game_tree.tree_[node].right_child, ptr, space_n);

    fputc('\n', ptr);
    FillSpace(space_n, ptr);
    fputc('}', ptr);
}

void Akinator::CreateVisualization(const char *file) {
    FILE* ptr = fopen(file, "w");

    fprintf(ptr, "digraph Tree { \nnode [shape = \"rectangle\", style = \"filled\"]\n");
    TraverseForVisualization(0, ptr);

    fprintf(ptr, "} \n");
    fclose(ptr);

}

void Akinator::PrintForVisual(size_t node, FILE* ptr, size_t turn) {
    fputc('"', ptr);
    for (size_t i = 0; i < game_tree.tree_[node].size; ++i) {
        fputc(game_tree.string_buffer[game_tree.tree_[node].index + i], ptr);
    }
    fputc('"', ptr);
    fputc('-', ptr);
    fputc('>', ptr);

    if (turn == LEFT) {
        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[game_tree.tree_[node].left_child].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[game_tree.tree_[node].left_child].index + i], ptr);
        }
        fputc('"', ptr);
        fputc(';', ptr);
        fputc('\n', ptr);
    }

    else if (turn == RIGHT) {
        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[game_tree.tree_[node].right_child].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[game_tree.tree_[node].right_child].index + i], ptr);
        }
        fputc('"', ptr);
        fputc(';', ptr);

        fputc('\n', ptr);
    }
}

void Akinator::TraverseForVisualization(size_t node, FILE *ptr) {

    if (game_tree.tree_[node].left_child != 0) {

        PrintForVisual(node, ptr, LEFT);

        TraverseForVisualization(game_tree.tree_[node].left_child, ptr);
    }

    if (game_tree.tree_[node].right_child != 0) {

        PrintForVisual(node, ptr, RIGHT);

        TraverseForVisualization(game_tree.tree_[node].right_child, ptr);
    }

    if (game_tree.tree_[node].left_child == 0 && game_tree.tree_[node].right_child == 0) {
        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[node].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[node].index + i], ptr);
        }
        fputc('"', ptr);
        fprintf(ptr, "[shape = \"circle\", color = \"burlywood1\"]\n");
    }
}


int  ParseDataFile(FILE* file, char* text) {
    char symbol = 0;
    int size = 0;

    while (symbol != '{') {
        symbol = fgetc(file);
        if (symbol == EOF) return -1;
    }

    while (symbol != '"') {
        symbol = fgetc(file);
        if (symbol == '}') return size;
    }

    while ((symbol = (fgetc(file))) != '"') {
        if (symbol == '?') continue;
        text[size] = symbol;
        size += 1;
    }

    return size;
}

void Akinator::PreOrder(size_t parent, size_t turn, char* str, FILE* ptr) {
    int size = ParseDataFile(ptr, str);
    if (size <= 0) return;

    game_tree.AddNode(0, 0, game_tree.string_buffer.size(), size);

    std::string_view string(str, size);
    game_tree.string_buffer += string;

    if (turn == LEFT) {
        game_tree.tree_[parent].left_child = game_tree.tree_.size() - 1;
        game_tree.tree_[game_tree.tree_[parent].left_child].parent = parent;
        game_tree.tree_[game_tree.tree_[parent].left_child].depth = game_tree.tree_[parent].depth + 1;
    }

    else {
        game_tree.tree_[parent].right_child = game_tree.tree_.size() - 1;
        game_tree.tree_[game_tree.tree_[parent].right_child].parent = parent;
        game_tree.tree_[game_tree.tree_[parent].right_child].depth = game_tree.tree_[parent].depth + 1;
    }


    size =  game_tree.tree_.size() - 1;

    PreOrder(size, 0, str, ptr);
    PreOrder( size, 1, str, ptr);

    if (game_tree.tree_[size].left_child == 0 && game_tree.tree_[size].right_child == 0) {
        UpdateHashTable(size);
    }

}

void Akinator::ReadData(const char *file) {
    FILE* ptr = fopen(file, "r");

    char str[CAPACITY];
    int size = ParseDataFile(ptr, str);

    if (size <= 0) return;

    game_tree.AddNode(0, 0, game_tree.string_buffer.size(), size);

    std::string_view string(str, size);
    game_tree.string_buffer += string;

    PreOrder(0, LEFT, str, ptr);

    PreOrder(0, RIGHT, str, ptr);

    fclose(ptr);
}

void Akinator::UpdateHashTable(int node) {
    table[std::string_view(game_tree.string_buffer.data() + game_tree.tree_[node].index,
                           game_tree.tree_[node].size)] = node;
}
