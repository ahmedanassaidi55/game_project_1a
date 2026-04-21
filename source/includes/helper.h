#ifndef HELPER_C
#define HELPER_C
#include "decl.h"
extern struct menu_t mainMenu;
extern int main_menu_init;
extern struct menu_t highscores_menu_data;
extern int highscores_menu_init;
extern struct menu_t character_menu_data;
extern int character_menu_init;
extern struct menu_t settings_menu_data;
extern struct menu_t enigma_menu_data;
extern struct menu_t save_menu_data;
extern int save_menu_init;
extern int enigma_menu_init;
extern int quiz_active;
extern int settings_menu_init;
extern int character_avatar_choice;
//definition des fonctions 'helper'
int init_game();
void exit_game();
void init_main_menu(SDL_Renderer *renderer, TTF_Font *font);
void init_highscores_menu(SDL_Renderer *renderer, TTF_Font *font);
void init_character_menu(SDL_Renderer *renderer, TTF_Font *font);
void init_enigma_menu(SDL_Renderer *renderer, TTF_Font *font);
void highlight_hovered(SDL_Renderer *renderer,button * buttons,int btn_count,int mouse_x, int mouse_y);
void on_button_click_goto_menu(button *buttons,int count,int mouse_x,int mouse_y,TTF_Font *font);
void switch_menu(enum menu goto_menu,TTF_Font *font);
void main_menu(SDL_Renderer *renderer,TTF_Font *font);
void init_anim(SDL_Renderer *renderer);
void display_anim(SDL_Renderer *renderer);
void play_menu(SDL_Renderer *renderer,TTF_Font *font);
void settings_menu(SDL_Renderer *renderer,TTF_Font *font);
void save_menu(SDL_Renderer *renderer,TTF_Font *font);
void highscores_menu(SDL_Renderer *renderer,TTF_Font *font);
void character_menu(SDL_Renderer *renderer,TTF_Font *font);
void enigma_menu(SDL_Renderer *renderer,TTF_Font *font);
void exit_anim(SDL_Renderer *renderer,TTF_Font *font);
void clean_up_menu(struct menu_t *menu,int elem_ct,int btn_ct);
#endif
