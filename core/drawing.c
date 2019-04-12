//
// Created by filip on 4/4/19.
//

#include "drawing.h"
#include "../snake/snake.h"
#include "../utils/colours.h"
#include <SDL2/SDL_ttf.h>



void drawGameObject(Game *game, int x, int y, int r, int g, int b, int a)
{
    SDL_Rect fillRect =
            {
                    x * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN),
                    y * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN),
                    GAME_OBJECT_SIZE,
                    GAME_OBJECT_SIZE
            };
    SDL_SetRenderDrawColor(game->renderer, r, g, b, a);
    SDL_RenderFillRect(game->renderer, &fillRect);
}


void clearScreen(Game *game)
{
    SDL_SetRenderDrawColor((*game).renderer, COLOUR_BACKGROUND);
    SDL_RenderClear((*game).renderer);
}

void drawGameBoardBorder(Game *game)
{
    SDL_Rect fillRect =
            {
                    BOARD_SIZE * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN),
                    0,
                    2,
                    SCREEN_HEIGHT
            };

    SDL_SetRenderDrawColor(game->renderer, COLOUR_SNAKE_SEGMENT);
    SDL_RenderFillRect(game->renderer, &fillRect);
}

void drawText(char *text, int x, int y, Game *game)
{
    SDL_Color textColor = {TEXT_COLOUR};
    SDL_Surface *textSurface = TTF_RenderText_Solid(game->font, text, textColor);
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &destination.w, &destination.h);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(game->renderer, textTexture, NULL, &destination);
    SDL_UpdateWindowSurface(game->window);
}

void drawScoreLabel(Game *game)
{
    char buff[64];
    sprintf(buff, "Score:         %3d", game->population.snakes[game->population.currentBestSnake].length - 1);
    drawText(buff, (BOARD_SIZE + 5) * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN),
             5 * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN), game);

}


void drawGame(Game *game)
{
    drawGameBoardBorder(game);
    drawScoreLabel(game);
    drawGenerationLabel(game);
    drawBestSnake(game);

}

void drawBestSnake(Game *game)
{
    drawSnake(&game->population.snakes[game->population.currentBestSnake], game);
    //for(int i = 0; i < POPULATION_SIZE; i++)
    //    drawSnake(&game->population.snakes[i], game);
}

void drawGenerationLabel(Game *game)
{
    char buff[64];
    sprintf(buff, "Generation: %3d", game->population.generation);
    drawText(buff, (BOARD_SIZE + 5) * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN),
             8 * (GAME_OBJECT_SIZE + GAME_OBJECT_MARGIN), game);
}
