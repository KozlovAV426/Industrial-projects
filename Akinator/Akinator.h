#include "Tree.h"

class Akinator {
private:
    Tree game_tree;

    void AddObject(size_t parent, size_t child);

    void Descent();

    void FirstLaunch();

    void PrintNode(size_t node);

    void CheckEmpty();

    void PreOrder(size_t current_node, size_t parent, size_t turn, char* str, FILE* ptr);

    void SaveData(const char* file);

    void Down(size_t node, FILE* ptr);

    void CreateVisualization(const char* file);

    void Traversal(size_t node, FILE* ptr);



public:
    Akinator();

    void StartGame();

    void ReadData(const char* file);
};