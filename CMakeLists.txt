cmake_minimum_required(VERSION 3.13)
project(SnakeNetwork C)

set(CMAKE_C_STANDARD 11)

add_executable(SnakeNetwork main.c core/game.h utils/colours.h snake/snake.h snake/snake.c core/drawing.h core/drawing.c core/game.c snake/fruit.c snake/fruit.h neural_network/network.c neural_network/network.h genetics/population.c genetics/population.h neural_network/matrix.c neural_network/matrix.h)
target_link_libraries(SnakeNetwork SDL2 SDL2_ttf m)