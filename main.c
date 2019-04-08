#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "core/lifecycle.h"
#include "core/game.h"
#include "utils/colours.h"
#include "snake/snake.h"
#include "core/drawing.h"


int main()
{

    Game game;
    if (init(&game) < 0)
        return -1;


    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        SegmentDirection direction = game.snake->direction;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            } else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        direction = UP;
                        break;
                    case SDLK_DOWN:
                        direction = DOWN;
                        break;
                    case SDLK_LEFT:
                        direction = LEFT;
                        break;
                    case SDLK_RIGHT:
                        direction = RIGHT;
                        break;
                }
            }
        }




        setSnakeDirection(game.snake, direction);
        updateGameState(&game);
        drawGame(&game);


        SDL_Delay(75);
        SDL_RenderPresent(game.renderer);

    }

    free_resources(&game);
    return 0;
}