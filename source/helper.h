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
//definition des types
typedef struct {
    char name[50];
    int score;
} Score;
enum menu {play,settings,save,highscores,character,enigma,back,exitgame,
	mono,multi,confirm,
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
extern int volume;
extern int running;
extern struct menu_t mainMenu;
extern int main_menu_init;
extern struct menu_t highscores_menu_data;
extern int highscores_menu_init;
extern struct menu_t character_menu_data;
extern int character_menu_init;
extern struct menu_t settings_menu_data;
extern int settings_menu_init;
extern int character_mode;
extern int character_avatar_choice;
extern enum current_menu_state current_menu;
//definition des fonctions 'helper'
int init_game();
void exit_game();
void init_main_menu(SDL_Renderer *renderer, TTF_Font *font);
void init_highscores_menu(SDL_Renderer *renderer, TTF_Font *font);
void init_character_menu(SDL_Renderer *renderer, TTF_Font *font);
void highlight_hovered(SDL_Renderer *renderer,button * buttons,int btn_count,int mouse_x, int mouse_y);
void on_button_click_goto_menu(button *buttons,int count,int mouse_x,int mouse_y);
void switch_menu(enum menu goto_menu);
void main_menu(SDL_Renderer *renderer,TTF_Font *font);
void display_anim(SDL_Renderer *renderer);
void play_menu(SDL_Renderer *renderer,TTF_Font *font);
void settings_menu(SDL_Renderer *renderer,TTF_Font *font);
void save_menu(SDL_Renderer *renderer,TTF_Font *font);
void highscores_menu(SDL_Renderer *renderer,TTF_Font *font);
void character_menu(SDL_Renderer *renderer,TTF_Font *font);
void enigma_menu(SDL_Renderer *renderer,TTF_Font *font);
void exit_anim(SDL_Renderer *renderer,TTF_Font *font);
