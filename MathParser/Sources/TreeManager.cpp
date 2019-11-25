#include "../Headers/TreeManager.h"
#include "../Headers/Constants.h"


void TreeManager::TraverseForVisualization(Tree::Node *node, FILE *ptr) {

    if (node->left != nullptr) {
        PrintForVisual(node->left, node, ptr);
        TraverseForVisualization(node->left, ptr);
    }

    if (node->right != nullptr) {
        PrintForVisual(node->right, node, ptr);
        TraverseForVisualization(node->right, ptr);
    }
    else {
        fprintf(ptr, "node%zu [label = ", node);

        if (!node->is_constant) {
            fprintf(ptr, "%s", FUNCTION_NAME[node->func_number]);
        }
        else {
            char str[12];
            sprintf(str, "%d", node->value);
            fprintf(ptr, "%s", str);
        }

        fprintf(ptr, "];\n");
    }
}

void TreeManager::PrintForVisual(Tree::Node *node, Tree::Node* parent, FILE *ptr) {
    fprintf(ptr, "node%zu [label = ", parent);
    fputc('"', ptr);

    if (!parent->is_constant) {
        fprintf(ptr, "%s", FUNCTION_NAME[parent->func_number]);
    }
    else {
        char str[12];
        sprintf(str, "%d", parent->value);
        fprintf(ptr, "%s", str);
    }
    fprintf(ptr, "\"];\n");

    fprintf(ptr, "node%zu [label = ", node);
    fputc('"', ptr);

    if (!node->is_constant) {
        fprintf(ptr, "%s", FUNCTION_NAME[node->func_number]);
    }
    else {
        char str[12];
        sprintf(str, "%d", node->value);
        fprintf(ptr, "%s", str);
    }

    fprintf(ptr, "\"];\n");
    if (node->is_constant) {
        fprintf(ptr, "node%zu [shape = \"circle\", color = \"burlywood1\"]\n", node);
    }
    fprintf(ptr, "node%zu -> node%zu;\n", parent, node);
}


void TreeManager::CreateVisualization(const char *filename) {
    FILE* ptr = fopen(filename, "w");

    fprintf(ptr, "digraph Tree { \nnode [shape = \"rectangle\", style = \"filled\"]\n");

    TraverseForVisualization(tree->root, ptr);
    fprintf(ptr, "} \n");
    fclose(ptr);
}

TreeManager::TreeManager(const Tree *tree)
        : tree(tree)
{}


