#include "puzzle.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main() {

    // init SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    // إنشاء window
    win = SDL_CreateWindow("Sliding Puzzle",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 0);

    // renderer
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // تحميل الصورة
    img = loadImage("image.png");

    // init اللعبة
    initPuzzle();

    SDL_Event e;
    int running = 1;

    // boucle principale
    while (running) {

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = 0;

            // click souris
            if (e.type == SDL_MOUSEBUTTONDOWN) {

                int x, y;

                // نحولو click → case
                getGridFromMouse(e.button.x, e.button.y, &x, &y);

                // نحرك القطعة
                moveTile(x, y);
            }
        }

        // render
        SDL_RenderClear(ren);
        renderPuzzle();
        SDL_RenderPresent(ren);

        // check win
        if (checkWin()) {
            printf("YOU WIN!\n");
            winAnimation();
            break;
        }

        showTimer();

        SDL_Delay(16); // ~60 FPS
    }

    // nettoyage
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
