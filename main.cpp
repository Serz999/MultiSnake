#include <iostream>
#include <ctime>
#include "SnakeGame/SnakeGame.h"

int main() {
    std::srand(time(NULL));

    SnakeGame Game;

    Game.PushKeyboard(SDLK_a, SDLK_d, SDLK_w, SDLK_s);
    //Game.PushKeyboard(SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN);
    Game.PushKeyboard(SDLK_f, SDLK_h, SDLK_t, SDLK_g);
    //Game.PushKeyboard(SDLK_j, SDLK_l, SDLK_i, SDLK_k);

    Game.SetUp(70, 40, 20, 5, 40, 5);
    Game.SetPlayersCount(0);
    Game.Loop();
    return 0;
}
