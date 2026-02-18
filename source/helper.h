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
#define DARK_GREY 105,105,105,255
#define DARK_GREY 200,200,200,255
//definition des types
enum menu {play,settings,save,highscores,character,enigma,back,exitgame};
typedef struct button{
	SDL_Texture *texture;
	SDL_Rect position;
	char label[20];
	enum menu type_menu;
	
}button;
typedef struct element{
	SDL_Texture *texture;
	SDL_Rect position;
};
struct menu{
	SDL_Texture *background;
	SDL_Rect position;
	element elements[MAX_ELEM_COUNT];
	button buttons[MAX_BUTTON_COUNT];
};
//definition des variables globaux
extern SDL_Window *window;
extern SDL_Renderer *renderer;
struct menu main_menu;
int main_menu_init = 0;
//definition des fonctions 'helper'
int init_game();
void exit_game();
void init_main_menu(SDL_Renderer *renderer)
void on_button_click_goto_menu(button *buttons,int count,int mouse_x,int mouse_y);
void switch_menu(enum menu goto_menu);
void main_menu(SDL_Renderer *renderer);
void display_anim(SDL_Renderer *renderer);
void play_menu(SDL_Renderer *renderer);
void settings_menu(SDL_Renderer *renderer);
void save_menu(SDL_Renderer *renderer);
void highscores_menu(SDL_Renderer *renderer);
void character_menu(SDL_Renderer *renderer);
void enigma_menu(SDL_Renderer *renderer);
void exit_anim(SDL_Renderer *renderer);
