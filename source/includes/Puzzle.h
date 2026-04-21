#ifndef PUZZLE_H
#define PUZZLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// حجم اللعبة (3x3)
#define GRID 3

// حجم الشاشة
#define SCREEN_W 600
#define SCREEN_H 400

// -------- STRUCT --------
// تمثل كل قطعة في اللعبة
typedef struct {
    int id;        // رقم القطعة (من 0 → 7)
    SDL_Rect rect; // مكانها (ما نستعملوهاش برشا هنا)
} Tile;

// -------- GLOBALS --------
// window + renderer + image
extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Texture *img;

// tableau يمثل الحالة متاع اللعبة
// -1 = الخانة الفارغة
extern int board[GRID][GRID];

// وقت بداية اللعبة
extern Uint32 startTime;

// حالة الفوز
extern int winGame;

// -------- FUNCTIONS --------

// تحميل الصورة
SDL_Texture* loadImage(const char *path);

// تهيئة اللعبة (initialisation)
void initPuzzle();

// رسم اللعبة
void renderPuzzle();

// التثبت من الفوز
int checkWin();

// تحريك قطعة
void moveTile(int x, int y);

// تحويل click → coordonnées grid
void getGridFromMouse(int mx, int my, int *x, int *y);

// timer
void showTimer();

// animation عند الفوز
void winAnimation();

#endif
