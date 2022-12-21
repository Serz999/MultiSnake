#include <iostream>
#include "SnakeGame/SnakeGame.h"

int main() {
    SnakeGame Game;
    Game.PushKeyboard(SDLK_a, SDLK_d, SDLK_w, SDLK_s);
    Game.PushKeyboard(SDLK_f, SDLK_h, SDLK_t, SDLK_g);
    Game.PushKeyboard(SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN);
    Game.SetUp(40, 30, 20,2, 50, 1);
    Game.Loop();
    return 0;
}
