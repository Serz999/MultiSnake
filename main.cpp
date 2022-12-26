#include <iostream>
#include <ctime>
#include "SnakeGame/SnakeGame.h"

int main() {
    std::srand(time(NULL));

    SnakeGame Game;

    Game.PushKeyboard(SDLK_a, SDLK_d, SDLK_w, SDLK_s);
    Game.PushKeyboard(SDLK_j, SDLK_l, SDLK_i, SDLK_k);
    Game.PushKeyboard(SDLK_f, SDLK_h, SDLK_t, SDLK_g);

    Game.menu_mode = false;
    if(!Game.menu_mode){
        Game.SetUp(70, 40, 20, 10, 40, 10);
        Game.SetPlayersCount(0);
    }
    Game.Loop();
    return 0;
}
