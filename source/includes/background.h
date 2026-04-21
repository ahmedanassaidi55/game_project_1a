
#ifndef BACKGROUND_H
#define BACKGROUND_H
//includes
#include "decl.h"
//const
#define OBJ_COUNT 0  //will be changed later once map is finalized
#define BACKGROUND_HEIGHT 2304
#define BACKGROUND_WIDTH 1296
#define INTERACT_RAD 25
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 600
#define WHITE (SDL_Color){255,255,255,255}
//types
typedef struct {
    SDL_Texture* base,*used;
    SDL_Rect position;
    int is_interactedwith;
    }interactable;
typedef struct {
    SDL_Texture* texture;
    SDL_Surface* bg_collision_map;
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
extern Background *bg;
extern tutorial_piece tut[3];
extern game_clock clk;
extern int background_init;
//functions
Background* init_background(SDL_Renderer *renderer,int player_count);
void center_background_on_player(Background *bg, int player_world_x, int player_world_y, int view_width, int view_height);
void display_background(Background* bg,SDL_Renderer *renderer,int player_count);
void scroll_background(Background *bg, enum direction d, int step);
int is_blocked(SDL_Surface* collision_map,int x, int y);
SDL_Color get_pixel(SDL_Surface *surface, int x, int y);
void init_tutorial(SDL_Renderer *renderer,tutorial_piece *tutorialArr, int tut_count);
int input_name(SDL_Renderer *renderer,SDL_Event *e,char name_buff[],int len_max,TTF_Font *font);                     
void manage_time(game_clock *t,SDL_Renderer *renderer,TTF_Font *font);
void display_tutorial(SDL_Renderer *renderer,tutorial_piece tut);
void save_score(ScoreInfo s,char* time, char* score_file);
void display_name(SDL_Renderer *renderer, TTF_Font *font, char *name);
void clean_up_bg(Background *bg,tutorial_piece *tut,game_clock *clk,int player_count);
#endif
