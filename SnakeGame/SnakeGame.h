#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

enum Command {LEFT, RIGHT, UP, DOWN};

class SnakeGame {
public:
    void SetUp(size_t width, size_t height);
    void SetUp(size_t width, size_t height, size_t size);
    void SetUp(size_t width, size_t height, size_t size, size_t game_speed);
    void SetUp(size_t width, size_t height, size_t size, size_t game_speed ,size_t food_count);

    void Loop();

private:
    size_t field_width;
    size_t field_height;
    size_t cell_size;

    SDL_Window *window;
    SDL_Renderer *renderer;

    size_t game_speed;
    size_t food_count;

    bool quit_flag = false;
    Command dir = Command::UP;

    class Snake {
    public:
        Snake(int start_x, int start_y, size_t cell_size, std::vector<SDL_Rect> &surface);

        void UpdateShape(Command action);

        SDL_Rect head;

        size_t body_size = 3;

        std::vector<SDL_Rect> body;

        std::vector<SDL_Rect> field;
    };
    
    Snake *_snake;

    std::vector<SDL_Rect> field;

    std::vector<SDL_Rect> food;
    SDL_Rect GenerateFood();

    void CreateNewFood(size_t count);

    void UpdateFood();

    void CheckCollision();

    void ButtonHandler();

    void ClearField();

    void Render();
};