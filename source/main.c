#include "helper.h"

int main(){
	TTF_Font game_font = NULL;
	SDL_Event event;
	if(init_game()){
		return 1;
	};
	game_font = TTF_OpenFont("assets/dogicapixel.ttf",20);
	if(!game_font){
		printf("font access error\n");
		return 1;
	}
	int running =1;
	while (running){
		main_menu(renderer,game_font);
		
	}
	exit_game();
	return 0;
}
