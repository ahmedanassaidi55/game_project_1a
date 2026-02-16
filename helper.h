#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//definiton des macros


//definition des types
enum menu {play,settings,save,highscores,back,exit};
typedef struct button{
	SDL_Texture texture;
	SDL_Rect position;
	char[20] label;
	enum menu type_menu;
	
};
//definition des variables globaux
SDL_Window *window;
SDL_Renderer *renderer;

//definition des fonctions 'helper'
int init_game();
void exit_game();
void on_button_click_goto_menu(button *buttons,int mouse_x,int mouse_y);
void switch_menu(enum menu goto_menu);

