#include "SnakeGame.h"

void SnakeGame::SetUp(size_t width, size_t height) {
    SetUp(field_width, field_height, 20);
}

void SnakeGame::SetUp(size_t width, size_t height, size_t size) {
    SetUp(field_width, field_height, size, 30);
}

void SnakeGame::SetUp(size_t width, size_t height, size_t size, size_t game_speed) {
    SetUp(field_width, field_height, size, game_speed, 1);
}


void SnakeGame::SetUp(size_t width, size_t height, size_t size, size_t game_speed, size_t food_count) {
    field_width = width;
    field_height = height;
    cell_size = size;
    this->game_speed = game_speed;
    this->food_count = food_count;

    for(size_t i = 0 ; i < field_height; i++){
        for(size_t j = 0 ; j < field_width; j++){
            SDL_Rect cell;
            cell.w = cell_size;
            cell.h = cell.w;
            cell.x = j * cell_size;
            cell.y = i * cell_size;
            field.push_back(cell);
        }
    }

    size_t screen_width = field_width * cell_size;
    size_t screen_height = field_height * cell_size;

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Snake",
                              SDL_WINDOWPOS_CENTERED_MASK,
                              SDL_WINDOWPOS_CENTERED_MASK,
                              screen_width,
                              screen_height,
                              SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    _snake = new Snake(field[field_width/2].x, field[field_width/2].x, cell_size, field);
}

void SnakeGame::Loop() {
    CreateNewFood(food_count);
    while(!quit_flag) {
        ButtonHandler();
        _snake->UpdateShape(dir);
        CheckCollision();
        UpdateFood();
        ClearField();
        Render();
        SDL_Delay(game_speed);
    }
}

void SnakeGame::ButtonHandler(){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT) quit_flag = true;
        if(e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym){
                case SDLK_a:
                    if(dir != Command::RIGHT) dir = Command::LEFT;
                    break;
                case SDLK_d:
                    if(dir != Command::LEFT) dir = Command::RIGHT;
                    break;
                case SDLK_w:
                    if(dir != Command::DOWN) dir = Command::UP;
                    break;
                case SDLK_s:
                    if(dir != Command::UP) dir = Command::DOWN;
                    break;
            }
        }
    }
}

SnakeGame::Snake::Snake(int start_x, int start_y, size_t cell_size, std::vector<SDL_Rect> &surface) {
    head.x = start_x;
    head.y = start_y;
    head.w = cell_size;
    head.h = head.w;
    this->field = surface;
}

void SnakeGame::Snake::UpdateShape(Command action) {
    size_t cell_size = head.w;

    body.resize(body_size);
    SDL_Rect prev_head = body[0];
    body[0] = head;
    SDL_Rect prev_prev_head;
    for(size_t i = 1; i < body.size(); i++) {
        prev_prev_head = body[i];
        body[i] = prev_head;
        prev_head = prev_prev_head;
    }

    switch (action) {
        case LEFT:
            head.x -= cell_size;
            if(head.x < 0 ) head.x = field[field.size() - 1].x;
            break;
        case RIGHT:
            head.x += cell_size;
            if(head.x > field[field.size() - 1].x) head.x = field[0].x;
            break;
        case UP:
            head.y -= cell_size;
            if(head.y < 0 ) head.y = field[field.size() - 1].y;
            break;
        case DOWN:
            head.y += cell_size;
            if(head.y > field[field.size() - 1].y ) head.y = field[0].y;
            break;
    }
}

SDL_Rect SnakeGame::GenerateFood() {
    SDL_Rect fruit;
    fruit.w = cell_size;
    fruit.h = fruit.w;

    fruit.x = field[rand()%(field.size() - 1)].x;
    fruit.y = field[rand()%(field.size() - 1)].y;

    if(fruit.x == _snake->head.x && fruit.y == _snake->head.y){
        fruit = GenerateFood();
    }

    for(size_t i = 0; i < _snake->body.size() ; i++) {
        if(fruit.x == _snake->body[i].x && fruit.y == _snake->body[i].y){
            fruit = GenerateFood();
        }
    }

    for(size_t i = 0; i < food.size() ; i++) {
        if(fruit.x == food[i].x && fruit.y == food[i].y){
            fruit = GenerateFood();
        }
    }

    return fruit;
}

void SnakeGame::UpdateFood() {
    for(size_t i = 0; i < food.size(); i++) {
        if(_snake->head.x == food[i].x && _snake->head.y == food[i].y) {
            food.erase(food.begin() + i);
            _snake->body_size++;
            food.push_back(GenerateFood());
            break;
        }
    }
}

void SnakeGame::CheckCollision() {
    for(size_t i = 0; i < _snake->body.size(); i++){
        if(_snake->head.x == _snake->body[i].x && _snake->head.y == _snake->body[i].y) {
            delete _snake;
            _snake = new Snake(field[field_width/2].x, field[field_width/2].x, cell_size, field);
            break;
        }
    }
}

void SnakeGame::ClearField() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void SnakeGame::Render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &_snake->head);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(size_t i = 0; i < _snake->body.size(); i++) {
        SDL_RenderFillRect(renderer, &_snake->body[i]);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for(size_t i = 0; i < food.size(); i++) {
        SDL_RenderFillRect(renderer, &food[i]);
    }

    SDL_RenderPresent(renderer);
}

void SnakeGame::CreateNewFood(size_t count) {
    for(size_t i = 0; i < count; i++) food.push_back(GenerateFood());
}
