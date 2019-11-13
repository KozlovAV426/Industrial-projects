#include <iostream>
#include "Akinator.h"

int main() {
    Akinator game;
    game.ReadData("../data/save");
    game.StartGame();

    return 0;
}