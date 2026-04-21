#include "puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------- GLOBAL VARIABLES --------
SDL_Window *win;
SDL_Renderer *ren;
SDL_Texture *img;

int board[GRID][GRID]; // tableau اللعبة
Uint32 startTime;      // وقت البداية
int winGame = 0;

// -------- LOAD IMAGE --------
SDL_Texture* loadImage(const char *path) {
    // تحميل الصورة
    SDL_Surface *tmp = IMG_Load(path);

    if (!tmp) {
        printf("Image error\n");
        exit(1);
    }

    // تحويلها texture
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tmp);

    SDL_FreeSurface(tmp); // تنظيف الميموار

    return tex;
}

// -------- INIT PUZZLE --------
void initPuzzle() {
    int val = 0;

    // تعبئة التابلو (0 → 8)
    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {
            board[y][x] = val;
            val++;
        }
    }

    // آخر خانة فارغة
    board[GRID-1][GRID-1] = -1;

    srand(time(NULL));

    // mélange (shuffle)
    for (int i = 0; i < 1000; i++) {
        int x = rand() % GRID;
        int y = rand() % GRID;
        moveTile(x, y); // نحرك عشوائي
    }

    // تسجيل وقت البداية
    startTime = SDL_GetTicks();
}

// -------- MOVE TILE --------
void moveTile(int x, int y) {
    int ex = -1, ey = -1;

    // نلقاو البلاصة الفارغة
    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            if (board[i][j] == -1) {
                ex = j;
                ey = i;
            }
        }
    }

    // نثبتو إذا القطعة جنب الفراغ
    if ((abs(ex - x) == 1 && ey == y) ||
        (abs(ey - y) == 1 && ex == x)) {

        // swap
        int tmp = board[y][x];
        board[y][x] = -1;
        board[ey][ex] = tmp;
    }
}

// -------- CHECK WIN --------
int checkWin() {
    int expected = 0;

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            // آخر خانة لازمها تكون فارغة
            if (y == GRID-1 && x == GRID-1) {
                if (board[y][x] != -1)
                    return 0;
            } else {
                // لازم الترتيب يكون صحيح
                if (board[y][x] != expected)
                    return 0;
                expected++;
            }
        }
    }

    return 1; // فزت 🎉
}

// -------- RENDER --------
void renderPuzzle() {
    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            int id = board[y][x];

            if (id == -1)
                continue; // الخانة الفارغة

            // منين نقص الصورة
            SDL_Rect src = {
                (id % GRID) * tileW,
                (id / GRID) * tileH,
                tileW,
                tileH
            };

            // وين نرسمها
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

// -------- MOUSE → GRID --------
void getGridFromMouse(int mx, int my, int *x, int *y) {
    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    // نحولو pixel → case
    *x = mx / tileW;
    *y = my / tileH;
}

// -------- TIMER --------
void showTimer() {
    Uint32 now = SDL_GetTicks();

    int sec = (now - startTime) / 1000;

    printf("Time: %d sec\r", sec); // نفس السطر
}

// -------- WIN ANIMATION --------
void winAnimation() {
    for (int i = 0; i < 20; i++) {

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        // تكبير الصورة تدريجيا
        SDL_Rect r = {
            100 - i*2,
            100 - i*2,
            600 + i*4,
            400 + i*4
        };

        SDL_RenderCopy(ren, img, NULL, &r);

        SDL_RenderPresent(ren);
        SDL_Delay(50);
    }
}
