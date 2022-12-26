#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum Command {LEFT, RIGHT, UP, DOWN};

enum ButtonEvent {SET_1, SET_2, SET_3, SET_5, SET_10, SET_20,
                    SET_PLAYERS_0, SET_PLAYERS_1, SET_PLAYERS_2, SET_PLAYERS_3,
                    START_GAME
                    };

class SnakeGame {
public:
    bool menu_mode = true;
    void SetUp(size_t field_width, size_t field_height, size_t cell_size, size_t snakes_count, size_t game_speed, size_t food_count);
    void SetPlayersCount(size_t players_count);
    void PushKeyboard(char left, char right, char up, char down);
    void PushPalette(size_t red, size_t green, size_t blue, size_t alpha);

    void Loop();

private:
    //setup class fields
    size_t field_width;
    size_t field_height;
    size_t cell_size;
    std::vector<SDL_Rect> field;
    size_t game_speed;
    size_t food_count;
    size_t snakes_count;
    size_t players_count;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit_flag = false;

    class Button {
    public:
        Button(SDL_Rect frame, const char * label, ButtonEvent event): frame(frame), label(label), event(event){};
        SDL_Rect frame;
        const char *label = " ";
        ButtonEvent event;
        size_t food_count;
        size_t snakes_count;
        size_t players_count;
        void callEvent(SnakeGame *Game);
    };

    std::vector<Button*> menu_buttons;

    class Snake {
    public:
        Snake(int start_x, int start_y, size_t cell_size, std::vector<SDL_Rect> &surface);

        void UpdateShape();

        Command moving_direction = Command::UP;

        SDL_Rect spawn_point;
        SDL_Rect head;
        size_t body_size = 3;
        std::vector<SDL_Rect> body;

        std::vector<SDL_Rect> field;
        SDL_Rect target;
    };

    class Keyboard {
    public:
        Keyboard(char left, char right, char up, char down):left(left), right(right), up(up), down(down){};
        char left;
        char right;
        char up;
        char down;
    };

    class Palette {
    public:
        Palette(size_t red, size_t green, size_t blue, size_t alpha):red(red), green(green), blue(blue), alpha(alpha){};
        size_t red;
        size_t green;
        size_t blue;
        size_t alpha;
    };

    void AIController(Snake *snk, size_t snk_idx);

    //Snake context
    std::vector<Snake*> snakes_family;
    std::vector<Keyboard*> keyboards_collection;
    std::vector<Palette*> pallets_collection;
    void CheckCollisions();
    void EntityRespawn(Snake *snk, size_t snk_idx);

    //food context
    std::vector<SDL_Rect> food;
    SDL_Rect GenerateFood();
    void CreateNewFood(size_t count);
    void UpdateFood();

    //input-output handlers
    void GenerateMenuBtns();
    void MenuControlHandler(SDL_Event e);
    void ManuallyControlHandler(SDL_Event e);
    void ClearField();
    void Render();
};