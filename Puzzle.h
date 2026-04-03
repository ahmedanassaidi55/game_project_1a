#ifndef PUZZLE_H
#define PUZZLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define GRID 3   // 3x3 puzzle

typedef struct {
    int id;
    SDL_Rect rect;
} Tile;

// Variables globales externes
extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *img;

extern Tile tiles[GRID * GRID];
extern int order[GRID * GRID];
extern int selected;
extern Uint32 startTime;
extern int winGame;

// Fonctions
SDL_Texture* loadImage(const char *path);
void initPuzzle();
int checkWin();
void renderPuzzle();
void swapTiles(int a, int b);
void showTimer();
void winAnimation();

#endif
