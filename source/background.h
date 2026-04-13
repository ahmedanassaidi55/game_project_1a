
#ifndef BACKGROUND_H
#define BACKGROUND_H
//includes
#include "decl.h"
//const
#define OBJ_COUNT 6  //will be changed later once map is finalized
#define BACKGROUND_HEIGHT 3200
#define BACKGROUND_WIDTH 2000
#define INTERACT_RAD 25
#define ELEM_HEIGHT 18
#define ELEM_WIDTH 26
#define TILE_HEIGHT 10
#define TILE_WIDTH 15
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 600
#define WHITE (SDL_Color){255,255,255,255}
//types
typedef struct {
    SDL_Texture* base,*used;
    int is_interactedwith;
    int tilex,tiley;
    SDL_Rect position;
    }interactable;
typedef struct {
    SDL_Texture* texture;
    int offsetX,offsetY;
    SDL_Rect position;
    interactable objects[OBJ_COUNT];
} Background;

typedef struct {
    int start;
    int elapsed;
    char disp_clock[10];
    SDL_Texture *texture;
}game_clock;
typedef struct {
    int score;
    char name[20];
}ScoreInfo;
typedef struct {
    SDL_Texture *tut_display;
    SDL_Rect position;
}tutorial_piece;
enum direction{up,up_right,right,down_right,down,down_left,left,left_up};

//globals
extern Background bg;
extern tutorial_piece tutorial[3];
extern int tilemap[TILE_HEIGHT][TILE_WIDTH];
extern game_clock clk;
//functions
void init_background(Background *bg,SDL_Renderer *renderer);
void load_tilemap(char *tm_file); //loading and saving will be provided with a save file if loading a save
void save_curr_tilemap(char *tm_file);//will instead load from a default file and create new save if new game
void display_background(Background *bg,SDL_Renderer *renderer);
void scroll_background(Background *bg, enum direction d, int step);
int is_blocked(int futureX, int futureY);
                                            // to check if player/enemy character can move to specific location
                                            // should be called with currentX+velX, currentY+velY
                                            // if it returns true, the specified location cannot be traveled to
void interact(int x,int y,interactable * objects); // updates interactable object's texture and changes the tile value in the matrix
void manage_time(game_clock *t,SDL_Renderer *renderer,TTF_Font *font);
void display_tutorial(SDL_Renderer *renderer,tutorial_piece tut);
void save_score(ScoreInfo s,char* time, char* score_file);
void display_highscores(char * score_file,SDL_Renderer *renderer);
#endif
