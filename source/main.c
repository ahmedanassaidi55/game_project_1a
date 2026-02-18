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

//animation lors de changement de menu

void display_anim(SDL_Renderer *renderer){
	char path[64];
	for(int i = 11;i >=1;i--){
		sprintf(path,"assets/frame_%02d.jpg",i);
		SDL_Surface *curr_frame = IMG_load(path);
		if(!surf){
			printf("error loading %s",path);
			return;
		}
		SDL_Texture *curr_texture = SDL_CreateTextureFromSurface(curr_frame);
		SDL_FreeSurface(curr_frame);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,curr_texture,NULL,NULL);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(curr_texture);
		SDL_Delay(10);
	}
}
