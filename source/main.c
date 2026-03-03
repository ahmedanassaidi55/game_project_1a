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
	music = Mix_LoadWAV("assets/placeholdermusic.wav");
	if(!music){
		printf("audio access error\n");
		Mix_FreeChunk(music);
		exit_game();
		}
	Mix_Volume(-1,volume*13);
	Mix_PlayChannel(-1,music,-1);
	while (running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = 0;
			}
			if(event.type == SDL_MOUSEBUTTONDOWN){
				switch(current_menu){
					case MENU_MAIN:
				on_button_click_goto_menu(mainMenu.buttons,
					7, event.motion.x, event.motion.y,game_font);
						break;
					case MENU_HIGHSCORES:
					on_button_click_goto_menu(
					highscores_menu_data.buttons,
					1, event.motion.x, event.motion.y,game_font);
						break;
					case MENU_SETTINGS:
					on_button_click_goto_menu(
					settings_menu_data.buttons,
					4, event.motion.x, event.motion.y,game_font);
						break;
					case MENU_CHARACTER:
					on_button_click_goto_menu(
					character_menu_data.buttons,
					6, event.motion.x, event.motion.y,game_font);
						break;					case MENU_ENIGMA:
					on_button_click_goto_menu(
					enigma_menu_data.buttons,
					3, event.motion.x, event.motion.y,game_font);
						break;
					case MENU_SAVE:
					on_button_click_goto_menu(
					save_menu_data.buttons,
					3, event.motion.x,
					event.motion.y,game_font);
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
				break;
			case MENU_HIGHSCORES:
				highscores_menu(renderer, game_font);
				break;
			case MENU_CHARACTER:
				character_menu(renderer, game_font);
				break;
			case MENU_PLAY:
				break;
			case MENU_SETTINGS:
				settings_menu(renderer,game_font);
				break;
			case MENU_SAVE:
				save_menu(renderer,game_font);
				break;
			case MENU_ENIGMA:
				enigma_menu(renderer,game_font);
				break;
			default:
				main_menu(renderer, game_font);
				break;
		}
	}
	exit_game();
	return 0;
}
