#include "Tree.h"
#include <unordered_map>

class Akinator {
private:
    Tree game_tree;
    std::unordered_map<std::string_view, int> table;

    void AddObject(size_t parent, size_t child, char* str);

    void ReHang(char *str, size_t added_num, size_t current_node);

    void InsertProperty(char* str, size_t current_node, size_t parent);

    void Descent();

    void FirstLaunch();

    int FindLCA(int first, int second);

    void PrintCommon(int node, int prev, int limit);

    void PrintNode(size_t node);

    void PreOrder(size_t parent, size_t turn, char* str, FILE* ptr);

    void SaveData(const char* file);

    void WriteFromNode(size_t node, FILE* ptr, size_t space_n);

    void CreateVisualization(const char* file);

    void UpdateHashTable(int node);

    void TraverseForVisualization(size_t node, FILE* ptr);

    void PrintForVisual(size_t node, FILE* ptr, size_t turn);


public:
    Akinator();

    void StartGame();

    void CompareWords();

    void ReadData(const char* file);
};