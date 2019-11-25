#include <iostream>
#include "./Headers/TreeManager.h"

int main() {
    Tree tree;
    tree.BuildTree("../Examples/test_1");
    bool ok = false;

    Tree::Node* oth;
    oth = tree.Differentiate(tree.root);
    Tree n;
    n.root = oth;
    TreeManager manager(&n);
    //tree.SimplifyByZero(tree.root, ok);
    manager.CreateVisualization("../visual");
    return 0;
}