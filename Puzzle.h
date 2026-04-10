#ifndef PUZZLE_H
#define PUZZLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define GRID 3
#define SCREEN_W 600
#define SCREEN_H 600

// -------- STRUCT --------
typedef struct {
    int id;
    SDL_Rect rect;
} Tile;

// -------- GLOBALS --------
extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *img;

extern Tile tiles[GRID * GRID];
extern int order[GRID][GRID];

extern Uint32 startTime;
extern int winGame;

// -------- FUNCTIONS --------
SDL_Texture* loadImage(const char *path);

void initPuzzle();
void renderPuzzle();
int checkWin();
void swapTiles(int x1, int y1, int x2, int y2);

void showTimer();
void winAnimation();

#endif
