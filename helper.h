#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//definiton des macros


//definition des types
enum menu {play,settings,save,highscores,character,back,exitgame};
typedef struct button{
	SDL_Texture *texture;
	SDL_Rect position;
	char label[20];
	enum menu type_menu;
	
}button;
//definition des variables globaux
extern SDL_Window *window;
extern SDL_Renderer *renderer;

//definition des fonctions 'helper'
int init_game(SDL_Window *window,SDL_Renderer *renderer);
void exit_game(SDL_Window *window,SDL_Renderer *renderer);
void on_button_click_goto_menu(button *buttons,int count,int mouse_x,int mouse_y);
void switch_menu(enum menu goto_menu);

