#include "SnakeGame.h"

//void SnakeGame::SetUp(size_t width, size_t height) {
//    SetUp(field_width, field_height, 20);
//}
//
//void SnakeGame::SetUp(size_t width, size_t height, size_t size) {
//    SetUp(field_width, field_height, size, 30);
//}
//
//void SnakeGame::SetUp(size_t width, size_t height, size_t size, size_t game_speed) {
//    SetUp(field_width, field_height, size, game_speed, 1);
//}

void SnakeGame::SetUp(size_t field_width, size_t field_height, size_t cell_size, size_t players_count, size_t game_speed, size_t food_count) {
    this->field_width = field_width;
    this->field_height = field_height;
    this->cell_size = cell_size;
    this->players_count = players_count;
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

    for(size_t i = 0; i < players_count; i++){
        Snake *snake = new Snake(field[field_width/(i+2)].x, field[field_width/(2)].y, cell_size, field);
        snakes_family.push_back(snake);
    }
}

void SnakeGame::Loop() {
    CreateNewFood(food_count);
    while(!quit_flag) {
        for(size_t i = 0; i < snakes_family.size() ; i++){
            ButtonHandler(snakes_family[i], keyboards_collection[i]);
            snakes_family[i]->UpdateShape();
        }
        CheckCollisions();
        UpdateFood();
        ClearField();
        Render();
        SDL_Delay(game_speed);
    }
}

SnakeGame::Snake::Snake(int start_x, int start_y, size_t cell_size, std::vector<SDL_Rect> &surface) {
    head.x = start_x;
    head.y = start_y;
    head.w = cell_size;
    head.h = head.w;
    this->field = surface;
}

void SnakeGame::Snake::UpdateShape() {
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

    switch (now_dir) {
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

    for(size_t i = 0; i < snakes_family.size(); i++) {
        if (fruit.x == snakes_family[i]->head.x && fruit.y == snakes_family[i]->head.y) {
            fruit = GenerateFood();
        }

        for (size_t j = 0; j < snakes_family[i]->body.size(); j++) {
            if (fruit.x == snakes_family[i]->body[j].x && fruit.y == snakes_family[i]->body[j].y) {
                fruit = GenerateFood();
            }
        }

        for (size_t j = 0; j < food.size(); j++) {
            if (fruit.x == food[j].x && fruit.y == food[j].y) {
                fruit = GenerateFood();
            }
        }
    }

    return fruit;
}

void SnakeGame::UpdateFood() {
    for(size_t i = 0; i < food.size(); i++) {
        for(size_t j = 0; j < snakes_family.size(); j++) {
            if (snakes_family[j]->head.x == food[i].x && snakes_family[j]->head.y == food[i].y) {
                food.erase(food.begin() + i);
                snakes_family[j]->body_size++;
                food.push_back(GenerateFood());
                break;
            }
        }
    }
}

void SnakeGame::CheckCollisions() {
    for(size_t i = 0; i < snakes_family.size(); i++) {
        for (size_t j = 0; j < snakes_family[i]->body.size(); j++) {
            if (snakes_family[i]->head.x == snakes_family[i]->body[j].x && snakes_family[i]->head.y == snakes_family[i]->body[j].y) {
                delete snakes_family[i];
                snakes_family[i] = new Snake(field[field_width / 2].x, field[field_width / 2].x, cell_size, field);
                break;
            }
        }
        for (size_t j = 0; j < snakes_family.size(); j++) {
            for (size_t k = 0; k < snakes_family[j]->body.size(); k++) {
                if (snakes_family[i]->head.x == snakes_family[j]->body[k].x && snakes_family[i]->head.y == snakes_family[j]->body[k].y) {
                    delete snakes_family[i];
                    snakes_family[i] = new Snake(field[field_width / 2].x, field[field_width / 2].x, cell_size, field);
                    break;
                }
            }
        }
    }
}

void SnakeGame::ClearField() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void SnakeGame::Render() {
    for(size_t i = 0; i < snakes_family.size(); i++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &snakes_family[i]->head);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (size_t j = 0; j < snakes_family[i]->body.size(); j++) {
            SDL_RenderFillRect(renderer, &snakes_family[i]->body[j]);
        }
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

void SnakeGame::PushKeyboard(char left, char right, char up, char down) {
    Keyboard *k = new Keyboard(left, right, up, down);
    keyboards_collection.push_back(k);
}

void SnakeGame::ButtonHandler(Snake *snk, Keyboard *keys){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT) quit_flag = true;
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == keys->left){
                std::cout << keys->left << '\n';
                if(snk->now_dir != Command::RIGHT) snk->now_dir = Command::LEFT;
                break;
            }
            if(e.key.keysym.sym == keys->right){
                std::cout << keys->right << '\n';
                if(snk->now_dir != Command::LEFT) snk->now_dir = Command::RIGHT;
                break;
            }
            if(e.key.keysym.sym == keys->up){
                if(snk->now_dir != Command::DOWN) snk->now_dir = Command::UP;
                break;
            }
            if(e.key.keysym.sym == keys->down){
                if(snk->now_dir != Command::UP) snk->now_dir = Command::DOWN;
                break;
            }
        }
    }
}