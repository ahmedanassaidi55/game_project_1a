// =======================
// LIBRARIES
// =======================
#include "puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// =======================
// GLOBAL VARIABLES
// =======================
SDL_Window *win;        // نافذة اللعبة
SDL_Renderer *ren;      // الرسام (drawing engine)
SDL_Texture *img;       // الصورة الأصلية

Tile tiles[GRID * GRID];  // معلومات كل قطعة
int order[GRID][GRID];    // ترتيب القطع في اللعبة

Uint32 startTime;        // وقت بداية اللعبة
int winGame = 0;         // هل اللاعب ربح


// =======================
// LOAD IMAGE
// =======================
SDL_Texture* loadImage(const char *path) {

    // نحمل الصورة في RAM
    SDL_Surface *tmp = IMG_Load(path);

    if (!tmp) {
        printf("Erreur image: %s\n", path);
        exit(1);
    }

    // نحول الصورة إلى Texture (باش تنجم تتعرض في SDL_Render)
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tmp);

    SDL_FreeSurface(tmp); // نحذف surface بعد التحويل

    return tex;
}


// =======================
// INIT PUZZLE
// =======================
void initPuzzle() {

    int tileW = SCREEN_W / GRID; // عرض كل قطعة
    int tileH = SCREEN_H / GRID; // طول كل قطعة

    int index = 0;

    // 1️⃣ إنشاء ترتيب صحيح (0,1,2,3,...)
    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            tiles[index].id = index;

            // مكان كل قطعة في الشاشة
            tiles[index].rect.x = x * tileW;
            tiles[index].rect.y = y * tileH;
            tiles[index].rect.w = tileW;
            tiles[index].rect.h = tileH;

            // نخزن الترتيب الصحيح
            order[y][x] = index;

            index++;
        }
    }

    // 2️⃣ خلط (Shuffle) القطع
    srand(time(NULL));

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            int ry = rand() % GRID;
            int rx = rand() % GRID;

            // تبديل عشوائي
            int tmp = order[y][x];
            order[y][x] = order[ry][rx];
            order[ry][rx] = tmp;
        }
    }

    // 3️⃣ بداية التايمر
    startTime = SDL_GetTicks();
}


// =======================
// CHECK WIN
// =======================
int checkWin() {

    int expected = 0;

    // نتأكد إذا الترتيب صحيح
    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            if (order[y][x] != expected)
                return 0; // مازال ما فازش

            expected++;
        }
    }

    return 1; // فاز
}


// =======================
// DRAW PUZZLE
// =======================
void renderPuzzle() {

    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {

            int id = order[y][x];

            // منين ناخذو الجزء من الصورة الأصلية
            SDL_Rect src = {
                (id % GRID) * tileW,
                (id / GRID) * tileH,
                tileW,
                tileH
            };

            // فين نرسموه في الشاشة
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


// =======================
// SWAP TWO TILES
// =======================
void swapTiles(int x1, int y1, int x2, int y2) {

    // تبديل مكان قطعتين
    int tmp = order[y1][x1];
    order[y1][x1] = order[y2][x2];
    order[y2][x2] = tmp;
}


// =======================
// TIMER
// =======================
void showTimer() {

    Uint32 now = SDL_GetTicks();

    int sec = (now - startTime) / 1000;

    printf("Time: %d sec\r", sec);
}


// =======================
// WIN ANIMATION
// =======================
void winAnimation() {

    for (int i = 0; i < 20; i++) {

        // لون الخلفية
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        // تكبير الصورة تدريجياً
        SDL_Rect r = {
            100 - i * 2,
            100 - i * 2,
            600 + i * 4,
            400 + i * 4
        };

        SDL_RenderCopy(ren, img, NULL, &r);

        SDL_RenderPresent(ren);

        SDL_Delay(50);
    }
}


// =======================
// GET GRID FROM MOUSE
// =======================
void getGridFromMouse(int mx, int my, int *x, int *y) {

    int tileW = SCREEN_W / GRID;
    int tileH = SCREEN_H / GRID;

    // نحول pixel → grid index
    *x = mx / tileW;
    *y = my / tileH;
}


// =======================
// MAIN GAME LOOP
// =======================
int main() {

    // 1️⃣ Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    // 2️⃣ Create window
    win = SDL_CreateWindow("Puzzle Game",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           SCREEN_W, SCREEN_H, 0);

    // 3️⃣ Renderer
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // 4️⃣ Load image
    img = loadImage("image.png");

    // 5️⃣ Init puzzle
    initPuzzle();

    int running = 1;
    SDL_Event e;

    int selected = 0;
    int sx = -1, sy = -1;

    // =======================
    // GAME LOOP
    // =======================
    while (running) {

        while (SDL_PollEvent(&e)) {

            // ❌ خروج
            if (e.type == SDL_QUIT)
                running = 0;

            // 🖱️ click mouse
            if (e.type == SDL_MOUSEBUTTONDOWN) {

                int x, y;
                getGridFromMouse(e.button.x, e.button.y, &x, &y);

                // أول click → نحفظ القطعة
                if (!selected) {
                    sx = x;
                    sy = y;
                    selected = 1;
                }

                // ثاني click → نبدل
                else {
                    swapTiles(sx, sy, x, y);
                    selected = 0;
                }
            }
        }

        // 🖼️ clear screen
        SDL_RenderClear(ren);

        // 🎨 draw puzzle
        renderPuzzle();

        SDL_RenderPresent(ren);

        // 🏁 check win
        if (checkWin()) {
            printf("YOU WIN!\n");
            winAnimation();
            running = 0;
        }

        // ⏱️ show time
        showTimer();

        SDL_Delay(16);
    }

    // =======================
    // CLEAN UP
    // =======================
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
