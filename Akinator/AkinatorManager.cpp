#include "AkinatorManager.h"
#include <cstring>

AkinatorManager::AkinatorManager()
    : akinator(new Akinator())
    {}

AkinatorManager::~AkinatorManager() {
    delete akinator;
}

void AkinatorManager::PlayGame(const char *common_file, const char *first, const char *second) {
    char input[CAPACITY];

    while (strcmp(input, "no")) {

        SayMessage("Hello, choose your mode", 23);
        printf("Hello, choose your mode (train, definition)\n");

        ReadInput(input);

        while (strcmp(input, "train") && strcmp(input, "definition")) {
            SayMessage("Incorrect, type again", 21);
            ReadInput(input);
        }

        if (!strcmp(input, "train")) {
            PlayTraining();
        }
        else {
            PlayDefinitions(common_file, first, second);
        }

        printf("Do you want to continue?\n");
        SayMessage("Do you want to continue?", 24);

        ReadInput(input);

    }
}


void AkinatorManager::PlayTraining() {
    akinator->Train();

    SaveData("../data/data");
}

void AkinatorManager::PlayDefinitions(const char* common_file, const char* first, const char* second) {
    FILE* common = fopen(common_file, "w");
    FILE* first_f = fopen(first, "w");
    FILE* second_f = fopen(second, "w");

    akinator->CompareWords(common, first_f, second_f);

    fclose(common);
    fclose(first_f);
    fclose(second_f);

    system("cat common | festival --tts");
    system("cat first | festival --tts");
    system("cat second | festival --tts");

}

void AkinatorManager::CreateVisualization(const char *file) {
    FILE* ptr = fopen(file, "w");

    fprintf(ptr, "digraph Tree { \nnode [shape = \"rectangle\", style = \"filled\"]\n");
    TraverseForVisualization(0, ptr);

    fprintf(ptr, "} \n");
    fclose(ptr);

    system("dot -Tpng ../vis -o ../visualization");
    system("eog ../visualization");

}

void AkinatorManager::PrintForVisual(size_t node, FILE* ptr, size_t turn) {
    fputc('"', ptr);
    for (size_t i = 0; i < akinator->game_tree.tree_[node].size; ++i) {
        fputc(akinator->game_tree.string_buffer[akinator->game_tree.tree_[node].index + i], ptr);
    }
    fputc('"', ptr);
    fputc('-', ptr);
    fputc('>', ptr);

    if (turn == LEFT) {
        fputc('"', ptr);
        for (size_t i = 0; i < akinator->game_tree.tree_[akinator->game_tree.tree_[node].left_child].size; ++i) {
            fputc(akinator->game_tree.string_buffer[
                          akinator->game_tree.tree_[akinator->game_tree.tree_[node].left_child].index + i], ptr);
        }
        fputc('"', ptr);
        fputc(';', ptr);
        fputc('\n', ptr);
    }
    else if (turn == RIGHT) {
        fputc('"', ptr);
        for (size_t i = 0; i < akinator->game_tree.tree_[akinator->game_tree.tree_[node].right_child].size; ++i) {
            fputc(akinator->game_tree.string_buffer[
                          akinator->game_tree.tree_[akinator->game_tree.tree_[node].right_child].index + i], ptr);
        }
        fputc('"', ptr);
        fputc(';', ptr);

        fputc('\n', ptr);
    }
}

void AkinatorManager::TraverseForVisualization(size_t node, FILE *ptr) {

    if (akinator->game_tree.tree_[node].left_child != 0) {

        PrintForVisual(node, ptr, LEFT);

        TraverseForVisualization(akinator->game_tree.tree_[node].left_child, ptr);
    }

    if (akinator->game_tree.tree_[node].right_child != 0) {

        PrintForVisual(node, ptr, RIGHT);

        TraverseForVisualization(akinator->game_tree.tree_[node].right_child, ptr);
    }

    if (akinator->game_tree.tree_[node].left_child == 0 && akinator->game_tree.tree_[node].right_child == 0) {
        fputc('"', ptr);
        for (size_t i = 0; i < akinator->game_tree.tree_[node].size; ++i) {
            fputc(akinator->game_tree.string_buffer[akinator->game_tree.tree_[node].index + i], ptr);
        }
        fputc('"', ptr);
        fprintf(ptr, "[shape = \"circle\", color = \"burlywood1\"]\n");
    }
}

void AkinatorManager::SaveData(const char *file) {
    FILE* ptr = fopen(file, "w");
    fprintf(ptr, "{\n");

    FillSpace(SHIFT, ptr);
    fputc('"', ptr);
    for (size_t i = 0; i < akinator->game_tree.tree_[0].size; ++i) {
        fputc(akinator->game_tree.string_buffer[akinator->game_tree.tree_[0].index + i], ptr);
    }
    fputc('"', ptr);
    WriteFromNode(akinator->game_tree.tree_[0].left_child, ptr, SHIFT);
    WriteFromNode(akinator->game_tree.tree_[0].right_child, ptr, SHIFT);

    fputc('\n', ptr);
    FillSpace(SHIFT, ptr);
    fputc('}', ptr);

    fclose(ptr);

}

void AkinatorManager::WriteFromNode(size_t node, FILE* ptr, size_t space_n) {
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
    for (size_t i = 0; i < akinator->game_tree.tree_[node].size; ++i) {
        fputc(akinator->game_tree.string_buffer[akinator->game_tree.tree_[node].index + i], ptr);
    }
    fputc('"', ptr);

    WriteFromNode(akinator->game_tree.tree_[node].left_child, ptr, space_n);
    WriteFromNode(akinator->game_tree.tree_[node].right_child, ptr, space_n);

    fputc('\n', ptr);
    FillSpace(space_n, ptr);
    fputc('}', ptr);
}

int AkinatorManager::ParseDataFile(FILE* file, char* text) {
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

void AkinatorManager::PreOrder(size_t parent, size_t turn, char* str, FILE* ptr) {
    int size = ParseDataFile(ptr, str);
    if (size <= 0) return;

    akinator->game_tree.AddNode(0, 0, akinator->game_tree.string_buffer.size(), size);


    akinator->ReserveBuffer();
    std::string_view string(str, size);
    akinator->game_tree.string_buffer += string;

    if (turn == LEFT) {
        akinator->game_tree.tree_[parent].left_child = akinator->game_tree.tree_.size() - 1;
        akinator->game_tree.tree_[akinator->game_tree.tree_[parent].left_child].parent = parent;
        akinator->game_tree.tree_[akinator->game_tree.tree_[parent].left_child].depth =
                akinator->game_tree.tree_[parent].depth + 1;
    }
    else {
        akinator->game_tree.tree_[parent].right_child = akinator->game_tree.tree_.size() - 1;
        akinator->game_tree.tree_[akinator->game_tree.tree_[parent].right_child].parent = parent;
        akinator->game_tree.tree_[akinator->game_tree.tree_[parent].right_child].depth =
                akinator->game_tree.tree_[parent].depth + 1;
    }


    size = akinator->game_tree.tree_.size() - 1;

    PreOrder(size, 0, str, ptr);
    PreOrder(size, 1, str, ptr);

    if (akinator->game_tree.tree_[size].left_child == 0 && akinator->game_tree.tree_[size].right_child == 0) {
        akinator->UpdateHashTable(size);
    }
}

void AkinatorManager::ReadData(const char *file) {
    FILE* ptr = fopen(file, "r");

    char str[CAPACITY];
    int size = ParseDataFile(ptr, str);

    if (size <= 0) return;

    akinator->game_tree.AddNode(0, 0, akinator->game_tree.string_buffer.size(), size);

    std::string_view string(str, size);
    akinator->game_tree.string_buffer += string;

    PreOrder(0, LEFT, str, ptr);

    PreOrder(0, RIGHT, str, ptr);

    fclose(ptr);
}