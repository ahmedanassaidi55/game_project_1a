#include "helper.h"
#include "gameplay.h"
#include <stdlib.h>
#include <time.h>

int main(){
	TTF_Font *game_font = NULL;
	SDL_Event event;
	GameplaySession gameplay;
	enum current_menu_state previous_menu = current_menu;
	gameplay_session_reset(&gameplay);
	if(init_game()){
		return 1;
	};
	game_font = TTF_OpenFont("assets/fonts/dogicapixel.ttf",24);
	if(!game_font){
		printf("font access error\n");
		exit_game();
		return 1;
	}
	music = Mix_LoadWAV("assets/audio/placeholdermusic.wav");
	if(!music){
		printf("audio access error\n");
		Mix_FreeChunk(music);
		exit_game();
		}
	Mix_Volume(-1,volume*13);
	Mix_PlayChannel(-1,music,-1);
	srand((unsigned int)time(NULL));
	while (running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = 0;
			}
			if(current_menu == MENU_PLAY &&
				gameplay_handle_event(&gameplay, &event, renderer, game_font, &current_menu)){
				continue;
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
			if(event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_ESCAPE){
				if(current_menu == MENU_MAIN)
				running = 0;
				else if(current_menu == MENU_PLAY)
				current_menu = MENU_MAIN;
				}
				if(event.key.keysym.sym == SDLK_j){
					if(current_menu == MENU_MAIN)	
					switch_menu(play,game_font);
				}
				if(event.key.keysym.sym == SDLK_o){
					if(current_menu == MENU_MAIN)	
					switch_menu(settings,game_font);
				}
				if(event.key.keysym.sym == SDLK_m){
					if(current_menu == MENU_MAIN)	
					switch_menu(highscores,game_font);
				}
				if(event.key.keysym.sym == SDLK_n){
					if(current_menu == MENU_SAVE)	
					switch_menu(new_game,game_font);
				}
				if(event.key.keysym.sym == SDLK_e){
					if(current_menu == MENU_HIGHSCORES)	
					switch_menu(enigma,game_font);
				}
				if(event.key.keysym.sym == SDLK_PLUS){
					if(current_menu == MENU_SETTINGS)	
					switch_menu(audio_inc,game_font);
				}
				if(event.key.keysym.sym == SDLK_MINUS){
					if(current_menu == MENU_SETTINGS)	
					switch_menu(audio_dec,game_font);
				}
					
				/*switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					if(current_menu == MENU_SETTINGS)	
					running = 0;
				break;
				case SDLK_j:
					if(current_menu == MENU_MAIN)	
						current_menu = MENU_PLAY;
				break;
				case SDLK_o:
					if(current_menu == MENU_MAIN)	
						current_menu = MENU_SETTINGS;
				break;
				case SDLK_m:
					if(current_menu == MENU_MAIN)	
						current_menu = MENU_HIGHSCORES;
				break;
				break;
				
				case SDLK_PLUS:
					if(current_menu == MENU_SETTINGS)	
					switch_menu(audio_inc,game_font);
				break;
				case SDLK_MINUS:
					if(current_menu == MENU_SETTINGS)	
					switch_menu(audio_dec,game_font);
				break;
				case SDLK_n:
					if(current_menu == MENU_SAVE)	
					switch_menu(new_game,game_font);
				break;
				case SDLK_e:
					if(current_menu == MENU_HIGHSCORES)	
					current_menu = MENU_ENIGMA;
				break;
				}*/
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
				if(gameplay_enter(&gameplay, renderer) != 0){
					current_menu = MENU_MAIN;
					break;
				}
				gameplay_frame(&gameplay, renderer, game_font, &current_menu);
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
		if(previous_menu == MENU_PLAY && current_menu != MENU_PLAY){
			gameplay_leave(&gameplay);
		}
		previous_menu = current_menu;
	}
	gameplay_leave(&gameplay);
	exit_game();
	return 0;
}
