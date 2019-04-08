//
// Created by filip on 4/3/19.
//

#ifndef SNAKENETWORK_LIFECYCLE_H
#define SNAKENETWORK_LIFECYCLE_H

#include "game.h"

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 720
#define BOARD_SIZE 45



int init(Game* game);

void free_resources(Game *game);

#endif //SNAKENETWORK_LIFECYCLE_H
