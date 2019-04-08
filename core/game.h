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

typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Snake* snake;
    Fruit fruit;
    TTF_Font *font;

} Game;

void updateGameState(Game*game);





#endif //SNAKENETWORK_GAME_H
