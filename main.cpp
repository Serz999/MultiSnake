#include <iostream>
#include "SnakeGame/SnakeGame.h"

int main() {
    SnakeGame Game;
    Game.SetUp(40, 30, 20, 30, 1);
    Game.Loop();
    return 0;
}
