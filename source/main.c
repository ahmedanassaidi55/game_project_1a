#include "helper.h"

int main(){
	TTF_Font *game_font = NULL;
	SDL_Event event;
	if(init_game()){
		return 1;
	};
	game_font = TTF_OpenFont("assets/dogicapixel.ttf",24);
	if(!game_font){
		printf("font access error\n");
		exit_game();
		return 1;
	}
	music = Mix_LoadWAV("");
	while (running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = 0;
			}
			if(event.type == SDL_MOUSEBUTTONDOWN){
				on_button_click_goto_menu(mainMenu.buttons,
					7, event.motion.x, event.motion.y);
				switch(current_menu){
					case MENU_MAIN:
				on_button_click_goto_menu(mainMenu.buttons,
					7, event.motion.x, event.motion.y);
						break;
					case MENU_HIGHSCORES:
					on_button_click_goto_menu(
					highscores_menu_data.buttons,
					1, event.motion.x, event.motion.y);
						break;
					case MENU_SETTINGS:
					on_button_click_goto_menu(
					settings_menu_data.buttons,
					1, event.motion.x, event.motion.y);
						break;
					default:
						break;
				}
			}
		}
		
		// Render based on current menu state
		switch(current_menu){
			case MENU_MAIN:
				main_menu(renderer, game_font);
				highlight_hovered(renderer, mainMenu.buttons, 7, event.motion.x, event.motion.y);
				break;
			case MENU_HIGHSCORES:
				highscores_menu(renderer, game_font);
				break;
			case MENU_CHARACTER:
				character_menu(renderer, game_font);
				break;
			case MENU_PLAY:
			case MENU_SETTINGS:
				settings_menu(renderer,game_font);
				break;
			case MENU_SAVE:
				// TODO: implement remaining menus
				main_menu(renderer, game_font);
				break;
			default:
				main_menu(renderer, game_font);
				break;
		}
	}
	exit_game();
	return 0;
}
