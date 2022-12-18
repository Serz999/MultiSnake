#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

static size_t field_width;

static size_t field_height;

static size_t cell_size;

static std::vector<SDL_Rect> field;

enum Command {LEFT, RIGHT, UP, DOWN};

class SnakeGame {
public:
    void SetUp(size_t width, size_t height);
    void SetUp(size_t width, size_t height, size_t size);
    void SetUp(size_t width, size_t height, size_t size, size_t game_speed);
    void SetUp(size_t width, size_t height, size_t size, size_t game_speed ,size_t food_count);

    void Loop();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    size_t game_speed;

    size_t food_count;

    bool quit_flag = false;
    Command dir = Command::UP;

    class Snake {
    public:
        Snake(int start_x, int start_y, size_t cell_size);

        void UpdateShape(Command action);

        SDL_Rect head;

        size_t body_size = 3;

        std::vector<SDL_Rect> body;
    };
    
    Snake *_snake;

    std::vector<SDL_Rect> food;

    SDL_Rect SpawnNewFood();

    void UpdateFood();

    void CheckCollision();

    void ButtonHandler();

    void ClearField();

    void Render();
};