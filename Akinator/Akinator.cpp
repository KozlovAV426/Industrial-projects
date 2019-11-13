#include "Akinator.h"
#include <string_view>

Akinator::Akinator() {
    game_tree = Tree();
}

void Akinator::StartGame() {

    for (size_t i = 0; i < 3; ++i) {
        if (game_tree.tree_.empty()) {
            FirstLaunch();
        }
        Descent();
    }
    SaveData("../data/save");
    CreateVisualization("../vis");
}

void Akinator::Descent() {
    size_t current_node = 0;
    std::string answer = "";
    size_t parent = 0;
    while (game_tree.tree_[current_node].left_child != 0 || game_tree.tree_[current_node].right_child != 0) {
        PrintNode(current_node);

        std::cin >> answer;

        while (answer != "y" && answer != "n") {
            printf("Incorrect input! Try again\n");
            std::cin >> answer;
        }

        if (answer == "y") {
            if (game_tree.tree_[current_node].right_child != 0) {
                parent = current_node;
                current_node = game_tree.tree_[current_node].right_child;
                continue;
            }
            AddObject(current_node, 1);
        }
        else {
            if (game_tree.tree_[current_node].left_child != 0) {
                parent = current_node;
                current_node = game_tree.tree_[current_node].left_child;
                continue;
            }
            AddObject(current_node, 0);
        }
    }
    printf("Is it ?\n");
    PrintNode(current_node);

    std::cin >> answer;
    if (answer != "y") {
        printf("Type difference between my answer and your object\n");
        size_t prev_index = game_tree.string_buffer.size();
        size_t added_num = game_tree.tree_.size();

        fgetc(stdin);

        std::getline(std::cin, game_tree.input);
        game_tree.string_buffer += game_tree.input;
        if (current_node == game_tree.tree_[parent].left_child) {
            game_tree.tree_[parent].left_child = added_num;
        }
        else game_tree.tree_[parent].right_child = added_num;
        game_tree.AddNode(0, 0, prev_index, game_tree.input.size());

        prev_index = game_tree.string_buffer.size();

        printf("Type your object\n");

        std::getline(std::cin, game_tree.input);
        game_tree.string_buffer += game_tree.input;
        game_tree.tree_[added_num].right_child = game_tree.tree_.size();
        game_tree.tree_[added_num].left_child = current_node;
        game_tree.AddNode(0, 0, prev_index, game_tree.input.size());

    }
    else {
        printf("I won\n");
    }
    std::cout << "End of session " << std::endl;
}

void Akinator::FirstLaunch() {
    printf("Type the first question\n");
    size_t prev_index = game_tree.string_buffer.size();

    std::cin >> game_tree.input;
    game_tree.string_buffer += game_tree.input;

    game_tree.AddNode(0, 0, prev_index, game_tree.input.size());

    AddObject(0, 1);
}

void Akinator::AddObject(size_t parent, size_t child) {
    printf("Sorry I don't know answer \nType your object \n");

    size_t prev_index = game_tree.string_buffer.size();

    std::cin >> game_tree.input;
    game_tree.string_buffer += game_tree.input;

    if (!child) {
        game_tree.tree_[parent].left_child = game_tree.tree_.size();
    }
    else {
        game_tree.tree_[parent].right_child = game_tree.tree_.size();
    }
    game_tree.AddNode(0, 0, prev_index, game_tree.input.size());



}

void Akinator::PrintNode(size_t node) {
    size_t index = game_tree.tree_[node].index;
    size_t len = game_tree.tree_[node].size;
    std::cout << static_cast<std::string_view>(game_tree.string_buffer.data()).substr(index, len) << std::endl;
}



void Akinator::SaveData(const char *file) {
    FILE* ptr = fopen(file, "w");
    for (size_t i = 0; i < game_tree.tree_[0].size; ++i) {
        fputc(game_tree.string_buffer[game_tree.tree_[0].index + i], ptr);
    }
    fputc('\n', ptr);
    Down(game_tree.tree_[0].left_child, ptr);
    Down(game_tree.tree_[0].right_child, ptr);

    fclose(ptr);

}

void Akinator::Down(size_t node, FILE* ptr) {
    if (!node) {
        fputc('\n', ptr);
        return;
    }
    for (size_t i = 0; i < game_tree.tree_[node].size; ++i) {
        fputc(game_tree.string_buffer[game_tree.tree_[node].index + i], ptr);
    }
    fputc('\n', ptr);
    Down(game_tree.tree_[node].left_child, ptr);
    Down(game_tree.tree_[node].right_child, ptr);
}

void Akinator::CreateVisualization(const char *file) {
    FILE* ptr = fopen(file, "w");
    fprintf(ptr, "digraph Tree { \nnode [shape = \"rectangle\", style = \"filled\"]\n");
    Traversal(0, ptr);
    fprintf(ptr, "} \n");
    fclose(ptr);

}

void Akinator::Traversal(size_t node, FILE *ptr) {

    if (game_tree.tree_[node].left_child != 0) {
        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[node].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[node].index + i], ptr);
        }
        fputc('"', ptr);
        fputc('-', ptr);
        fputc('>', ptr);

        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[game_tree.tree_[node].left_child].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[game_tree.tree_[node].left_child].index + i], ptr);
        }
        fputc('"', ptr);
        fputc(';', ptr);

        fputc('\n', ptr);

        Traversal(game_tree.tree_[node].left_child, ptr);
    }

    if (game_tree.tree_[node].right_child != 0) {
        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[node].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[node].index + i], ptr);
        }
        fputc('"', ptr);
        fputc('-', ptr);
        fputc('>', ptr);

        fputc('"', ptr);
        for (size_t i = 0; i < game_tree.tree_[game_tree.tree_[node].right_child].size; ++i) {
            fputc(game_tree.string_buffer[game_tree.tree_[game_tree.tree_[node].right_child].index + i], ptr);
        }
        fputc('"', ptr);
        fputc(';', ptr);

        fputc('\n', ptr);

        Traversal(game_tree.tree_[node].right_child, ptr);
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
    while ((symbol != '{' && symbol != '}') {
        symbol = fgetc(file);
        if (symbol == '}') return size;
        if (symbol == EOF) return -1;
    }
    while (fgetc(file) != '"');
    while ((symbol = (fgetc(file)) != '"')) {
        if (symbol == '?') continue;
        text[size] = symbol;
    }
    return size;
}

void Akinator::PreOrder(size_t current_node, size_t parent, size_t turn, char* str, FILE* ptr) {
    fgets(str, CAPACITY, ptr);
    size_t  sz = 0;
    while (str[sz] != '\n') {
        sz += 1;
    }
    if (!sz) return;
    if (!turn) {
        game_tree.tree_[parent].left_child = game_tree.tree_.size();
    }
    else {
        game_tree.tree_[parent].right_child = game_tree.tree_.size();
    }
    game_tree.AddNode(0, 0, game_tree.string_buffer.size(), sz);
    std::string_view string(str, sz);
    game_tree.string_buffer += string;
    sz =  game_tree.tree_.size() - 1;
    PreOrder(game_tree.tree_.size() - 1,sz, 0, str, ptr);
    PreOrder(game_tree.tree_.size() - 1, sz, 1, str, ptr);

}

void Akinator::ReadData(const char *file) {
    FILE* ptr = fopen(file, "r");

    char str[CAPACITY];
    fgets(str, CAPACITY, ptr);

    size_t  sz = 0;
    while (str[sz] != '\n') {
        sz += 1;
    }

    game_tree.AddNode(0, 0, game_tree.string_buffer.size(), sz);
    std::string_view string(str, sz);
    game_tree.string_buffer += string.substr(0, sz);

    PreOrder(0, 0, 0, str, ptr);

    PreOrder(0, 0, 1, str, ptr);

    fclose(ptr);

}