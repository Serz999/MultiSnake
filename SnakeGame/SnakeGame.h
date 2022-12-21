#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

enum Command {LEFT, RIGHT, UP, DOWN};

class SnakeGame {
public:
    void SetUp(size_t field_width, size_t field_height, size_t cell_size, size_t players_count, size_t game_speed, size_t food_count);
    void PushKeyboard(char left, char right, char up, char down);

    void Loop();

private:
    size_t field_width;
    size_t field_height;
    size_t cell_size;
    size_t game_speed;
    size_t food_count;

    std::vector<SDL_Rect> field;

    SDL_Window *window;
    SDL_Renderer *renderer;

    bool quit_flag = false;
    //Command dir = Command::UP;

    class Snake {
    public:
        Snake(int start_x, int start_y, size_t cell_size, std::vector<SDL_Rect> &surface);

        void UpdateShape();

        Command now_dir = Command::UP;

        SDL_Rect head;
        size_t body_size = 3;
        std::vector<SDL_Rect> body;

        std::vector<SDL_Rect> field;
    };

    class Keyboard {
    public:
        Keyboard(char left, char right, char up, char down){this->left = left; this->right; this->up; this->down;};
        char left;
        char right;
        char up;
        char down;
    };

    std::vector<Keyboard*> keyboards_collection;

    //snake direction
    size_t players_count;
    std::vector<Snake*> snakes_family;
    void CheckCollisions();

    //food direction
    std::vector<SDL_Rect> food;
    SDL_Rect GenerateFood();
    void CreateNewFood(size_t count);
    void UpdateFood();

    void ButtonHandler(Snake *snk, Keyboard *keys);
    void ClearField();
    void Render();
};