#include <iostream>
#include "AkinatorManager.h"

int main() {
    AkinatorManager game;
    game.ReadData("../data/data");

    game.PlayGame();


    game.SaveData("../data/data");
    game.CreateVisualization(PATH_TO_VISUAL);

    return 0;
}