#include "Tree.h"
#include <unordered_map>

class Akinator {
    friend class AkinatorManager;
private:
    Tree game_tree;
    std::unordered_map<std::string_view, int> table;

    void AddObject(size_t parent, size_t child, char* str);

    void ReHang(char *str, size_t added_num, size_t current_node);

    void InsertProperty(char* str, size_t current_node, size_t parent);

    void Descent();

    void FirstLaunch();

    void ReserveBuffer();

    int FindLCA(int first, int second);

    void PrintCommon(int node, int prev, int limit, FILE* output);

    void SaveVoiceText(int node, FILE* output);

    void PrintNode(size_t node);

    void UpdateHashTable(int node);

public:
    Akinator();

    void Train();

    void CompareWords(FILE* common, FILE* first, FILE* second);

};