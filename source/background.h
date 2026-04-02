
#ifndef BACKGROUND_H
#define BACKGROUND_H
//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//const
#define MAX_OBJ_COUNT 6  //will be changed later once map is finalized
#define TILE_HEIGHT 10
#define TILE_WIDTH 15
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 600
#define WHITE (SDL_Color){255,255,255,255}
//types
typedef struct {
    SDL_Texture* texture;
    int offsetX,offsetY;
    SDL_Rect position;
    interactable objects[MAX_OBJ_COUNT];
} Background;
typedef struct {
    SDL_Texture* base,used;
    int is_interactedwith;
    SDL_Rect position;
    }interactable;
typedef struct {
    int start;
    int elapsed;
}game_clock;
typedef struct {
    int time, score;
    char name[20];
}ScoreInfo;
typedef struct {
    SDL_Texture *tut_display;
    SDL_Rect position;
}tutorial_piece;
enum direction{up,up_right,right,down_right,down,down_left,left,up_left};

//globals
extern Background bg;
extern tutorial_piece tutorial[3];
extern int tilemap[TILE_HEIGHT][TILE_WIDTH];
//functions
void init_background(Background *bg,SDL_Renderer *renderer);
void load_tilemap(char *tm_file); //loading and saving will be provided with a save file if loading a save
void save_curr_tilemap(char *tm_file);//will instead load from a default file and create new save if new game
void display_background(Background bg,SDL_Renderer *renderer);
void scroll_background(Background *bg, enum direction d, int step);
void is_blocked(int futureX, int futureY, enum direction d);
                                            // to check if player/enemy character can move to specific location
                                            // should be called with currentX+velX, currentY+velY
                                            // if it returns true, the specified location cannot be traveled to
int is_interactive(int x, int y); //check if tile close to player is usable, to activate doors or pick up items
void interact(int x,int y,interactable * objects); // updates interactable object's texture and changes the tile value in the matrix
void manage_time(game_clock *t);
void display_tutorial(SDL_Renderer *renderer,tutorial_piece tut);
void save_score(ScoreInfo s, char* score_file);
void display_highscores(char * score_file,SDL_Renderer *renderer);
#endif
