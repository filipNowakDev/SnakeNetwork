#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
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
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
                quit = true;
        }

        clearScreen(&game);
        updateGameState(&game);
        drawGame(&game);

        //SDL_Delay(5);
        SDL_RenderPresent(game.renderer);

    }

    free_resources(&game);
    return 0;
}