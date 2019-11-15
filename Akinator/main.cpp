#include <iostream>
#include "Akinator.h"

int main() {
    Akinator game;
    game.ReadData("../data/data");
    game.StartGame();

    return 0;
}