cmake_minimum_required(VERSION 3.13)
project(SnakeNetwork C)

set(CMAKE_C_STANDARD 11)

add_executable(SnakeNetwork main.c core/lifecycle.h core/lifecycle.c core/game.h utils/colours.h snake/snake.h snake/snake.c core/drawing.h core/drawing.c core/game.c snake/fruit.c snake/fruit.h neural_network/network.c neural_network/network.h)
target_link_libraries(SnakeNetwork SDL2 SDL2_ttf m)