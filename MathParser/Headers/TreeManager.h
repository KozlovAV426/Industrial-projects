#include "Tree.h"

class TreeManager {
private:
    const Tree* tree;

    void PrintForVisual(Tree::Node* node, Tree::Node* parent, FILE* ptr);

    void TraverseForVisualization(Tree::Node* node, FILE* ptr);
public:
    TreeManager(const Tree* tree);

    void CreateVisualization(const char* filename);
};




