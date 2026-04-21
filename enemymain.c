#include "enemy.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "Enemy Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, 0);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        return 1;
    }

    /* PLAYER (factice pour les tests) */
    Player player = { 400.0f, 270.0f, 50, 50, 10 };

    /* BUG FIX : Ć©tait "spritesheet.png", le fichier s'appelle "enemy.jpeg" */
    Enemy *enemy = enemy_create(ren, "enemy.jpeg", 100.0f, 270.0f, 1);
    if (!enemy) {
        fprintf(stderr, "Impossible de crĆ©er l'ennemi.\n");
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    MiniMap mm = minimap_create();

    int running = 1;
    Uint32 last = SDL_GetTicks();

    while (running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT)
                running = 0;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
            /* Touche D : infliger des dĆ©gĆ¢ts Ć  l'ennemi (test) */
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_d)
                enemy_take_damage(enemy, 1);
        }

        /* DĆ©placer le joueur avec les flĆØches */
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_RIGHT]) player.x += 3.0f;
        if (keys[SDL_SCANCODE_LEFT])  player.x -= 3.0f;
        if (keys[SDL_SCANCODE_UP])    player.y -= 3.0f;
        if (keys[SDL_SCANCODE_DOWN])  player.y += 3.0f;

        Uint32 now   = SDL_GetTicks();
        Uint32 delta = now - last;
        last = now;

        enemy_update(enemy, &player, delta);

        /* Rendu */
        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        enemy_draw(enemy, ren);
        minimap_draw(&mm, enemy, &player, ren);

        SDL_RenderPresent(ren);
        SDL_Delay(16); /* ~60 FPS */
    }

    enemy_destroy(enemy);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
