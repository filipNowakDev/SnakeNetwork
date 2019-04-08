//
// Created by filip on 4/3/19.
//

#ifndef SNAKENETWORK_GAME_H
#define SNAKENETWORK_GAME_H

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "../snake/snake.h"
#include "../snake/fruit.h"
#include "drawing.h"
#include "../genetics/population.h"

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 720
#define BOARD_SIZE 45

typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Population population;
    TTF_Font *font;

} Game;



int init(Game* game);
void free_resources(Game *game);
void updateGameState(Game*game);





#endif //SNAKENETWORK_GAME_H
