//
// Created by filip on 4/4/19.
//


#ifndef SNAKENETWORK_DRAWING_H
#define SNAKENETWORK_DRAWING_H

#include "game.h"

#define GAME_OBJECT_SIZE (SCREEN_HEIGHT / (BOARD_SIZE + 10))
#define GAME_OBJECT_MARGIN  (SCREEN_HEIGHT/BOARD_SIZE  - GAME_OBJECT_SIZE)

void drawGameObject(Game* game, int x, int y, int r, int g, int b, int a);
void clearScreen(Game *game);
void drawGameBoardBorder(Game* game);
void drawText(char *text, int x, int y, Game *game);
void drawScore(Game *game);
void drawGame(Game* game);



#endif //SNAKENETWORK_DRAWING_H
