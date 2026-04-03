#include "puzzle.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    win = SDL_CreateWindow("Puzzle Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 0);

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // ⚠️ Remplace l'image ici
    img = loadImage("image.png");

    initPuzzle();

    SDL_Event e;

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                return 0;

            if (e.type == SDL_MOUSEBUTTONDOWN && !winGame) {
                int x = e.button.x / (SCREEN_W / GRID);
                int y = e.button.y / (SCREEN_H / GRID);
                int index = y * GRID + x;

                if (selected == -1) {
                    selected = index;
                } else {
                    swapTiles(selected, index);
                    selected = -1;

                    if (checkWin()) {
                        winGame = 1;
                        winAnimation();
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        renderPuzzle();

        SDL_RenderPresent(ren);

        showTimer();
        SDL_Delay(16);
    }

    return 0;
}
