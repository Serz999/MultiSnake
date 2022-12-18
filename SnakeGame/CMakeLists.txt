cmake_minimum_required (VERSION 3.24)
project(SnakeGameLib)

set(CMAKE_CXX_STANDARD 23)

add_library(SnakeGameLib STATIC SnakeGame.h SnakeGame.cpp)