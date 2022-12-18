cmake_minimum_required(VERSION 3.24)
project(SnakeTheGame)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED SDL2)

add_executable(SnakeTheGame main.cpp SnakeGame/SnakeGame.cpp SnakeGame/SnakeGame.h)
target_link_libraries(SnakeTheGame PRIVATE SDL2)

add_subdirectory(SnakeGame)
target_link_libraries(SnakeGameLib)