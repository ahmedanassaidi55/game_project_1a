#include "puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------- GLOBAL VARIABLES --------
SDL_Window *win;
SDL_Renderer *ren;
SDL_Texture *img;

Tile tiles[GRID * GRID];
int order[GRID][GRID];

Uint32 startTime;
int winGame = 0;

// -------- LOAD IMAGE --------
SDL_Texture* loadImage(const char *path) {
    SDL_Surface *tmp = IMG_Load(path);
    if (!tmp) {
        printf("Erreur image: %s\n", path);
        exit(1);
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tmp);
    SDL_FreeSurface(tmp);
    return tex;
}

// -------- INIT PUZZLE (MATRICE) --------
void initPuzzle() {
    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    int index = 0;

    // création ordonnée
    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            tiles[index].id = index;

            tiles[index].rect.x = x * tileW;
            tiles[index].rect.y = y * tileH;
            tiles[index].rect.w = tileW;
            tiles[index].rect.h = tileH;

            order[y][x] = index;
            index++;
        }
    }

    // shuffle matrice
    srand(time(NULL));

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            int ry = rand() % GRID;
            int rx = rand() % GRID;

            int tmp = order[y][x];
            order[y][x] = order[ry][rx];
            order[ry][rx] = tmp;
        }
    }

    startTime = SDL_GetTicks();
}

// -------- CHECK WIN --------
int checkWin() {
    int expected = 0;

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {
            if (order[y][x] != expected)
                return 0;
            expected++;
        }
    }
    return 1;
}

// -------- RENDER PUZZLE --------
void renderPuzzle() {
    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            int id = order[y][x];

            SDL_Rect src = {
                (id % GRID) * tileW,
                (id / GRID) * tileH,
                tileW,
                tileH
            };

            SDL_Rect dst = {
                x * tileW,
                y * tileH,
                tileW,
                tileH
            };

            SDL_RenderCopy(ren, img, &src, &dst);
        }
    }
}

// -------- SWAP (MATRICE) --------
void swapTiles(int x1, int y1, int x2, int y2) {
    int tmp = order[y1][x1];
    order[y1][x1] = order[y2][x2];
    order[y2][x2] = tmp;
}

// -------- TIMER --------
void showTimer() {
    Uint32 now = SDL_GetTicks();
    int sec = (now - startTime) / 1000;
    printf("Time: %d sec\r", sec);
}

// -------- WIN ANIMATION --------
void winAnimation() {
    for (int i = 0; i < 20; i++) {
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        SDL_Rect r = { 100 - i*2, 100 - i*2, 600 + i*4, 400 + i*4 };
        SDL_RenderCopy(ren, img, NULL, &r);

        SDL_RenderPresent(ren);
        SDL_Delay(50);
    }
}
