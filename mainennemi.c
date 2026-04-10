#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "ennemi.h"

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow(
        "Enemy AI",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Enemy enemy;
    Player player = {100, 100};

    initEnemy(&enemy, renderer);

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        /* UPDATE */
        pathfindingSimple(&enemy, player);
        animateEnemy(&enemy);
        shootProjectile(&enemy, player);
        updateProjectiles(&enemy);

        /* RENDER */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderEnemy(&enemy, renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
