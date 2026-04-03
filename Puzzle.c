#include "puzzle.h"
#include <stdio.h>
#include <stdlib.h>

// Déclaration des variables globales
SDL_Window *win;
SDL_Renderer *ren;
SDL_Texture *img;

Tile tiles[GRID * GRID];
int order[GRID * GRID];

int selected = -1;
Uint32 startTime;
int winGame = 0;

// -------------------- LOAD IMAGE --------------------
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

// -------------------- INIT PUZZLE --------------------
void initPuzzle() {
    int index = 0;
    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {
            tiles[index].id = index;
            tiles[index].rect.x = x * tileW;
            tiles[index].rect.y = y * tileH;
            tiles[index].rect.w = tileW;
            tiles[index].rect.h = tileH;
            order[index] = index;
            index++;
        }
    }

    // Shuffle des tuiles
    srand(time(NULL));
    for (int i = 0; i < GRID * GRID; i++) {
        int r = rand() % (GRID * GRID);
        int tmp = order[i];
        order[i] = order[r];
        order[r] = tmp;
    }

    startTime = SDL_GetTicks();
}

// -------------------- CHECK WIN --------------------
int checkWin() {
    for (int i = 0; i < GRID * GRID; i++) {
        if (order[i] != i) return 0;
    }
    return 1;
}

// -------------------- RENDER PUZZLE --------------------
void renderPuzzle() {
    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    for (int i = 0; i < GRID * GRID; i++) {
        int id = order[i];
        SDL_Rect src = { (id % GRID) * tileW, (id / GRID) * tileH, tileW, tileH };
        SDL_Rect dst = { (i % GRID) * tileW, (i / GRID) * tileH, tileW, tileH };
        SDL_RenderCopy(ren, img, &src, &dst);
    }
}

// -------------------- SWAP --------------------
void swapTiles(int a, int b) {
    int tmp = order[a];
    order[a] = order[b];
    order[b] = tmp;
}

// -------------------- TIMER --------------------
void showTimer() {
    Uint32 now = SDL_GetTicks();
    int sec = (now - startTime) / 1000;
    printf("Time: %d sec\r", sec);
}

// -------------------- WIN ANIMATION SIMPLE --------------------
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
