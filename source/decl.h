#ifndef DECL_H
#define DECL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//definiton des macros
#define MAX_BUTTON_COUNT 8
#define MAX_ELEM_COUNT 5
#define DARK_GREY (SDL_Color){105,105,105,255}
#define LIGHT_GREY (SDL_Color){200,200,200,255}
#define BLACK (SDL_Color){0,0,0,255}
#define WHITE (SDL_Color){255,255,255,255}
//definition des types
typedef struct {
    char name[50];
    int score;
} Score;
enum menu {play,settings,save,highscores,character,enigma,back,exitgame,
	mono,multi,confirm,character1,character2,story,
	quiz,puzzle,correctAns,load_game,new_game,
	audio_inc,audio_dec,fullscreen};
enum current_menu_state {MENU_MAIN, MENU_HIGHSCORES, MENU_CHARACTER, MENU_PLAY, MENU_SETTINGS,MENU_ENIGMA, MENU_SAVE};
typedef struct button{
	SDL_Texture *texture;
	SDL_Texture *texture_hovered;
	SDL_Rect position;
	char label[20];
	enum menu type_menu;
	
}button;
typedef struct element{
	SDL_Texture *texture;
	SDL_Rect position;
	char label[20];
}element;
struct menu_t{
	SDL_Texture *background;
	SDL_Rect position;
	element elements[MAX_ELEM_COUNT];
	button buttons[MAX_BUTTON_COUNT];
};
//definition des variables globaux
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern Mix_Chunk *music;
extern int volume,paused;
extern int running;
#endif
