//
// Created by filip on 4/4/19.
//

#include <time.h>
#include "game.h"
#include "../genetics/population.h"

int initSDL(Game *game);

void updateGameState(Game *game)
{
    if (areAnySnakesAlive(&game->population))
        updatePopulation(&game->population);
    else
        nextGeneration(&game->population);
}

int init(Game *game)
{
    srand(time(NULL));
    initPopulation(&game->population);
    return initSDL(game);
}

int initSDL(Game *game)
{
    game->window = NULL;
    game->renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error while initializing video: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -2;
    }

    game->window = SDL_CreateWindow("Snake",
                                    SDL_WINDOWPOS_CENTERED, // NOLINT(hicpp-signed-bitwise)
                                    SDL_WINDOWPOS_CENTERED, // NOLINT(hicpp-signed-bitwise)
                                    SCREEN_WIDTH,
                                    SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);

    if (game->window == NULL)
    {
        printf("Error while creating window: %s\n", SDL_GetError());
        return -3;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL)
    {
        printf("Error while creating renderer: %s\n", SDL_GetError());
        return -4;
    }

    game->font = TTF_OpenFont("../resources/Zyana-Regular.ttf", SCREEN_HEIGHT / 15);
    if (game->font == NULL)
    {
        printf("Error while creating font: %s\n", SDL_GetError());
        return -5;
    }


    return 0;
}

void free_resources(Game *game)
{
    destroyPopulation(&game->population);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_CloseFont(game->font);
    game->font = NULL;
    game->window = NULL;
    game->renderer = NULL;
    //game->snake = NULL;
    TTF_Quit();
    SDL_Quit();
}

