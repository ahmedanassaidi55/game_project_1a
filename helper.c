#include "helper.h"


int init_game(){
	window = NULL;
	renderer = NULL;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
     	   printf("Erreur SDL_Init : %s\n", SDL_GetError());
        	return 1;
    	}
	window = SDL_CreateWindow(
        "PlaceholderName",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600, 400,
        SDL_WINDOW_SHOWN
    	);
    	if (!window) {
        	printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        	SDL_Quit();
    		return 1;
    	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    	if (renderer == NULL) {
        	printf("Erreur Renderer : %s\n", SDL_GetError());
        	SDL_DestroyWindow(window);
        	SDL_Quit();
        	return 1;
    	}
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        	printf("Erreur IMG_Init : %s\n", IMG_GetError());
    	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        	printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        	return 1;
    	}
	if (TTF_Init() == -1) {
        	printf("Erreur TTF_Init : %s\n", TTF_GetError());
        	return 1;
    	}
	return 0;
}

void exit_game(){
	if (textTexture)
        	SDL_DestroyTexture(textTexture);
    	if (font)
        	TTF_CloseFont(font);
    	TTF_Quit();

    	if (image)
        	SDL_DestroyTexture(image);
    	IMG_Quit();

    	if (son)
        	Mix_FreeChunk(son);
    	Mix_CloseAudio();

    	SDL_DestroyRenderer(renderer);
    	SDL_DestroyWindow(window);
    	SDL_Quit();
}
void switch_menu(enum menu goto_menu){
	switch(goto_menu){
		case play:
			display_anim();
			play_menu();
			break;
		case settings:
			display_anim();
			settings_menu();
			break;
		case save:
			display_anim();
			save_menu();
			break;
		case highscores:
			display_anim();
			highscores_menu();
			break;
		case back:
			display_anim():
			main_menu;
		case exit:
			exit_anim();
			exit_game();
	}
}
void on_button_click_goto_menu(button *buttons,int mouse_x,int mouse_y){
	button *pB;
	SDL_Point p={mouse_x,mouse_y};
	for (pB=buttons;pB<buttons+COUNT;pB++){
		if(SDL_PointInRect(&p,&pB->position)){
			switch_menu(pb->type_menu);
		}
	}
}
