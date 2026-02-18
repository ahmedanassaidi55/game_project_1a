#include "helper.h"

int main(){
	TTF_Font game_font = NULL;
	SDL_Event event;
	int running =1;
	if(init_game()){
		return 1;
	};
	game_font = TTF_OpenFont("assets/dogicapixel.ttf",20);
	if(!game_font){
		printf("font access error\n");
		return 1;
	}
	while (running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
			running =0;
			}
			if(event.type ==SDL_MOUSEBUTTONDOWN){
				
			}
		}
		main_menu(renderer);
		
	}
	exit_game();
	return 0;
}
