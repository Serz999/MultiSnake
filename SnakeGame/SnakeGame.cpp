#include "SnakeGame.h"

void SnakeGame::SetUp(size_t field_width, size_t field_height, size_t cell_size, size_t snakes_count, size_t game_speed, size_t food_count) {
    this->field_width = field_width;
    this->field_height = field_height;
    this->cell_size = cell_size;
    this->snakes_count = snakes_count;
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

    for(size_t i = 0; i < snakes_count; i++){
        size_t spawn_x = (field_width/(2 * snakes_count)) + i * (field_width / (snakes_count));
        size_t spawn_y = field_width/2;
        Snake *snake = new Snake(field[spawn_x].x, field[spawn_y].y, cell_size, field);
        snakes_family.push_back(snake);
    }

    if(keyboards_collection.size() < snakes_family.size()){
        size_t init_keyboards = keyboards_collection.size();
        for (size_t i = 0; i < snakes_family.size() - init_keyboards; i++) {
            PushKeyboard(' ', ' ', ' ', ' ');
        }
    }

    if(pallets_collection.size() < snakes_family.size()){
        size_t init_pallets = pallets_collection.size();
        for (size_t i = 0; i < snakes_family.size() - init_pallets; i++) {
            PushPalette(rand()%255, rand()%255, rand()%255, 255);
        }
    }
}

void SnakeGame::SetPlayersCount(size_t players_count) {
    this->players_count = players_count;
}

void SnakeGame::Loop() {
    CreateNewFood(food_count);
    while(!quit_flag) {
        if(players_count != 0) ButtonHandler();
        for(size_t i = 0; i < snakes_family.size() ; i++){
            if(i + 1 > players_count) AIController(snakes_family[i], i);
            snakes_family[i]->UpdateShape();
            UpdateFood();
        }
        CheckCollisions();
        ClearField();
        Render();
        SDL_Delay(game_speed);
    }
}

void SnakeGame::AIController(Snake *snk, size_t snk_idx) {
    bool target_is_food = false;
    for (size_t i = 0; i < food.size(); i++) if (snk->target.x == food[i].x && snk->target.y == food[i].y) target_is_food = true;
    if(!target_is_food) snk->target = food[rand()%food.size()];

    //for vertical-oriented snake
    if(snk->head.y == snk->target.y){
        if(snk->head.x > snk->target.x) if (snk->moving_direction != Command::RIGHT) snk->moving_direction = Command::LEFT;
        if(snk->head.x < snk->target.x) if (snk->moving_direction != Command::LEFT) snk->moving_direction = Command::RIGHT;
    }

    //for horizontal-oriented snake
    if(snk->head.x == snk->target.x){
        if(snk->head.y > snk->target.y) if (snk->moving_direction != Command::DOWN) snk->moving_direction = Command::UP;
        if(snk->head.y < snk->target.y) if (snk->moving_direction != Command::UP) snk->moving_direction = Command::DOWN;
    }

    //self-defence behaviour
    size_t observer_radius = 3;
    for(size_t i = 0; i < snakes_family.size(); i++){
        if(i != snk_idx) {
            for(size_t roundY = 0; roundY < observer_radius*2 + 1; roundY++) {
                for (size_t roundX = 0; roundX < observer_radius*2 + 1; roundX++) {
                    if (snk->head.x + (observer_radius * cell_size) - cell_size*roundX == snakes_family[i]->head.x &&
                        snk->head.y + (observer_radius * cell_size) - cell_size*roundY == snakes_family[i]->head.y) {
                        if (snk->moving_direction == Command::UP || snk->moving_direction == Command::DOWN) {
                            if (snk->head.x > snakes_family[i]->head.x) {
                                if (snk->moving_direction != Command::LEFT) snk->moving_direction = Command::RIGHT;
                                //break;
                                continue;
                            }
                            if (snk->head.x < snakes_family[i]->head.x) {
                                if (snk->moving_direction != Command::RIGHT) snk->moving_direction = Command::LEFT;
                                //break;
                                continue;
                            }
                            size_t command_idx = rand() % 1;
                            if(command_idx == 0) if (snk->moving_direction != Command::RIGHT) snk->moving_direction = Command::LEFT;
                            if(command_idx == 1) if (snk->moving_direction != Command::LEFT) snk->moving_direction = Command::RIGHT;
                            //break;
                            continue;
                        }
                        if (snk->moving_direction == Command::LEFT || snk->moving_direction == Command::RIGHT) {
                            if (snk->head.y > snakes_family[i]->head.y) {
                                if (snk->moving_direction != Command::UP) snk->moving_direction = Command::DOWN;
                                //break;
                                continue;
                            }
                            if (snk->head.y < snakes_family[i]->head.y) {
                                if (snk->moving_direction != Command::DOWN) snk->moving_direction = Command::UP;
                                //break;
                                continue;
                            }
                            size_t command_idx = rand() % 1 + 2;
                            if(command_idx == 2) if (snk->moving_direction != Command::DOWN) snk->moving_direction = Command::UP;
                            if(command_idx == 3) if (snk->moving_direction != Command::UP) snk->moving_direction = Command::DOWN;
                            //break;
                            continue;
                        }
                    }
                    for (size_t j = 0; j < snakes_family[i]->body.size(); j++) {
                        if (snk->head.x + (observer_radius * cell_size) - cell_size*roundX == snakes_family[i]->body[j].x &&
                            snk->head.y + (observer_radius * cell_size) - cell_size*roundY == snakes_family[i]->body[j].y) {
                            if (snk->moving_direction == Command::UP || snk->moving_direction == Command::DOWN) {
                                if (snk->head.x > snakes_family[i]->body[j].x) {
                                    if (snk->moving_direction != Command::LEFT) snk->moving_direction = Command::RIGHT;
                                    //break;
                                    continue;
                                }
                                if (snk->head.x < snakes_family[i]->body[j].x) {
                                    if (snk->moving_direction != Command::RIGHT) snk->moving_direction = Command::LEFT;
                                    //break;
                                    continue;
                                }
                                size_t command_idx = rand() % 1;
                                if(command_idx == 0) if (snk->moving_direction != Command::RIGHT) snk->moving_direction = Command::LEFT;
                                if(command_idx == 1) if (snk->moving_direction != Command::LEFT) snk->moving_direction = Command::RIGHT;
                                //break;
                                continue;
                            }
                            if (snk->moving_direction == Command::LEFT || snk->moving_direction == Command::RIGHT) {
                                if (snk->head.y > snakes_family[i]->body[j].y) {
                                    if (snk->moving_direction != Command::UP) snk->moving_direction = Command::DOWN;
                                    //break;
                                    continue;
                                }
                                if (snk->head.y < snakes_family[i]->body[j].y) {
                                    if (snk->moving_direction != Command::DOWN) snk->moving_direction = Command::UP;
                                    //break;
                                    continue;
                                }
                                size_t command_idx = rand() % 1 + 2;
                                if(command_idx == 2) if (snk->moving_direction != Command::DOWN) snk->moving_direction = Command::UP;
                                if(command_idx == 3) if (snk->moving_direction != Command::UP) snk->moving_direction = Command::DOWN;
                                //break;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }

    //self-collision prevention
    for(observer_radius = 1; observer_radius < 2; observer_radius++) {
        if (snk->moving_direction == Command::UP) {
            for (size_t i = 0; i < snk->body.size(); i++) {
                if (snk->head.x == snk->body[i].x && snk->head.y - (observer_radius * cell_size) == snk->body[i].y) {
                    if (snk->head.x - (observer_radius * cell_size) == snk->body[i].x &&
                        snk->head.y == snk->body[i].y) {
                        snk->moving_direction = Command::RIGHT;
                        break;
                    }
                    if (snk->head.x + (observer_radius * cell_size) == snk->body[i].x &&
                        snk->head.y == snk->body[i].y) {
                        snk->moving_direction = Command::LEFT;
                        break;
                    }
                    size_t command_idx = rand() % 1;
                    if (command_idx == 0) snk->moving_direction = Command::LEFT;
                    if (command_idx == 1) snk->moving_direction = Command::RIGHT;
                    break;
                }
            }
        }
        if (snk->moving_direction == Command::DOWN) {
            for (size_t i = 0; i < snk->body.size(); i++) {
                if (snk->head.x == snk->body[i].x && snk->head.y + (observer_radius * cell_size) == snk->body[i].y) {
                    if (snk->head.x - (observer_radius * cell_size) == snk->body[i].x &&
                        snk->head.y == snk->body[i].y) {
                        snk->moving_direction = Command::RIGHT;
                        break;
                    }
                    if (snk->head.x + (observer_radius * cell_size) == snk->body[i].x &&
                        snk->head.y == snk->body[i].y) {
                        snk->moving_direction = Command::LEFT;
                        break;
                    }
                    size_t command_idx = rand() % 1;
                    if (command_idx == 0) snk->moving_direction = Command::LEFT;
                    if (command_idx == 1) snk->moving_direction = Command::RIGHT;
                    break;
                }
            }
        }
        if (snk->moving_direction == Command::LEFT) {
            for (size_t i = 0; i < snk->body.size(); i++) {
                if (snk->head.x - (observer_radius * cell_size) == snk->body[i].x && snk->head.y == snk->body[i].y) {
                    if (snk->head.x == snk->body[i].x &&
                        snk->head.y - (observer_radius * cell_size) == snk->body[i].y) {
                        snk->moving_direction = Command::DOWN;
                        break;
                    }
                    if (snk->head.x == snk->body[i].x &&
                        snk->head.y + (observer_radius * cell_size) == snk->body[i].y) {
                        snk->moving_direction = Command::UP;
                        break;
                    }
                    size_t command_idx = rand() % 1 + 2;
                    if (command_idx == 2) snk->moving_direction = Command::UP;
                    if (command_idx == 3) snk->moving_direction = Command::DOWN;
                    break;
                }
            }
        }
        if (snk->moving_direction == Command::RIGHT) {
            for (size_t i = 0; i < snk->body.size(); i++) {
                if (snk->head.x + (observer_radius * cell_size) == snk->body[i].x && snk->head.y == snk->body[i].y) {
                    if (snk->head.x == snk->body[i].x &&
                        snk->head.y - (observer_radius * cell_size) == snk->body[i].y) {
                        snk->moving_direction = Command::DOWN;
                        break;
                    }
                    if (snk->head.x == snk->body[i].x &&
                        snk->head.y + (observer_radius * cell_size) == snk->body[i].y) {
                        snk->moving_direction = Command::UP;
                        break;
                    }
                    size_t command_idx = rand() % 1 + 2;
                    if (command_idx == 2) snk->moving_direction = Command::UP;
                    if (command_idx == 3) snk->moving_direction = Command::DOWN;
                    break;
                }
            }
        }
    }
}

SnakeGame::Snake::Snake(int start_x, int start_y, size_t cell_size, std::vector<SDL_Rect> &surface) {
    spawn_point.x = start_x;
    spawn_point.y = start_y;
    head.x = spawn_point.x;
    head.y = spawn_point.y;
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

    switch (moving_direction) {
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

void SnakeGame::CheckCollisions() {
    for(size_t i = 0; i < snakes_family.size(); i++) {
        for (size_t j = 0; j < snakes_family[i]->body.size(); j++) {
            if (snakes_family[i]->head.x == snakes_family[i]->body[j].x && snakes_family[i]->head.y == snakes_family[i]->body[j].y) {
                EntityRespawn(snakes_family[i], i);
                break;
            }
        }
        for(size_t j = 0; j < snakes_family.size(); j++) {
//            if (snakes_family[i]->head.x == snakes_family[j]->head.x && snakes_family[i]->head.y == snakes_family[j]->head.y) {
//                delete snakes_family[i];
//                snakes_family[i] = new Snake(field[field_width / (i+2)].x, field[field_width / 2].x, cell_size, field);
//                delete snakes_family[j];
//                snakes_family[j] = new Snake(field[field_width / (i+2)].x, field[field_width / 2].x, cell_size, field);
//                break;
//            }
            for (size_t k = 0; k < snakes_family[j]->body.size(); k++) {
                if (snakes_family[i]->head.x == snakes_family[j]->body[k].x && snakes_family[i]->head.y == snakes_family[j]->body[k].y) {
                    EntityRespawn(snakes_family[i], i);
                    break;
                }
            }
        }
    }
}

void SnakeGame::EntityRespawn(Snake *snk, size_t snk_idx) {
    size_t spawn_x = snk->spawn_point.x;
    size_t spawn_y = snk->spawn_point.y;
    delete snk;
    snakes_family[snk_idx] = new Snake(spawn_x, spawn_y, cell_size, field);
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

void SnakeGame::CreateNewFood(size_t count) {
    for(size_t i = 0; i < count; i++) food.push_back(GenerateFood());
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

void SnakeGame::ClearField() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void SnakeGame::PushKeyboard(char left, char right, char up, char down) {
    Keyboard *k = new Keyboard(left, right, up, down);
    keyboards_collection.push_back(k);
}

void SnakeGame::ButtonHandler(){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT) quit_flag = true;
        if(e.type == SDL_KEYDOWN){
            for(size_t i = 0; i < snakes_family.size(); i++) {
                if (e.key.keysym.sym == keyboards_collection[i]->left) {
                    if (snakes_family[i]->moving_direction != Command::RIGHT) snakes_family[i]->moving_direction = Command::LEFT;
                    break;
                }
                if (e.key.keysym.sym == keyboards_collection[i]->right) {
                    if (snakes_family[i]->moving_direction != Command::LEFT) snakes_family[i]->moving_direction = Command::RIGHT;
                    break;
                }
                if (e.key.keysym.sym == keyboards_collection[i]->up) {
                    if (snakes_family[i]->moving_direction != Command::DOWN) snakes_family[i]->moving_direction = Command::UP;
                    break;
                }
                if (e.key.keysym.sym == keyboards_collection[i]->down) {
                    if (snakes_family[i]->moving_direction != Command::UP) snakes_family[i]->moving_direction = Command::DOWN;
                    break;
                }
            }
        }
    }
}

void SnakeGame::PushPalette(size_t red, size_t green, size_t blue, size_t alpha) {
    Palette *p = new Palette(red, green, blue, alpha);
    pallets_collection.push_back(p);
}

void SnakeGame::Render() {
    for(size_t i = 0; i < snakes_family.size(); i++) {
        SDL_SetRenderDrawColor(renderer, pallets_collection[i]->red, pallets_collection[i]->green, pallets_collection[i]->blue, pallets_collection[i]->alpha);
        SDL_RenderFillRect(renderer, &snakes_family[i]->head);

        SDL_SetRenderDrawColor(renderer, pallets_collection[i]->red, pallets_collection[i]->green, pallets_collection[i]->blue, pallets_collection[i]->alpha);
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