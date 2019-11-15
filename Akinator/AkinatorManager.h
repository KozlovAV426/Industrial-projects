#include "Akinator.h"
#include "Config.h"

class AkinatorManager {
private:
    Akinator* akinator;

    int  ParseDataFile(FILE* file, char* text);

    void PreOrder(size_t parent, size_t turn, char* str, FILE* ptr);

    void WriteFromNode(size_t node, FILE* ptr, size_t space_n);

    void TraverseForVisualization(size_t node, FILE* ptr);

    void PrintForVisual(size_t node, FILE* ptr, size_t turn);

public:
    AkinatorManager();

    ~AkinatorManager();

    void CreateVisualization(const char* file);

    void ReadData(const char* file);

    void SaveData(const char* file);

    void PlayTraining();

    void PlayDefinitions(const char* common_file, const char* first, const char* second);
};