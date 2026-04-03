#include "helper.h"
#include "ennemi.h"

int main()
{
    if(init_game() != 0)
        return -1;

    TTF_Font *font = TTF_OpenFont("assets/font.ttf", 24);

    Enemy enemy;
    Player player;

    player.x = 100;
    player.y = 100;
    player.rect.w = 50;
    player.rect.h = 50;

    initEnemy(&enemy, renderer);

    Uint32 lastShoot = 0;
    SDL_Event event;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = 0;

            if(event.type == SDL_KEYDOWN)
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
        }

        if(current_menu == MENU_PLAY)
        {
            int mx, my;
            SDL_GetMouseState(&mx, &my);

            player.x = mx;
            player.y = my;

            player.rect.x = player.x;
            player.rect.y = player.y;

            pathfindingSimple(&enemy, player);
            animateEnemy(&enemy);

            if(SDL_GetTicks() - lastShoot > 1000)
            {
                shootProjectile(&enemy, player);
                lastShoot = SDL_GetTicks();
            }

            updateProjectiles(&enemy);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            renderEnemy(&enemy, renderer);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &player.rect);

            SDL_RenderPresent(renderer);
        }
    }

    exit_game();
    return 0;
}
