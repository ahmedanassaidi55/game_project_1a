#include "helper.h"

//initialiser le jeu 

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
void init_main_menu(SDL_Renderer *renderer){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface.\n");
		return;
	}
	strcpy(main_menu.buttons[0].label,);
	strcpy(main_menu.buttons[1].label,);
	strcpy(main_menu.buttons[2].label,);
	strcpy(main_menu.buttons[3].label,);
	main_menu.background =SDL_CreateTextureFromSurface(renderer,temp_surf);
	SDL_FreeSurface(temp_surf);
	main_menu.position.x = 0;
	main_menu.position.y = 0;
	main_menu.position.w = 600;
	main_menu.position.h = 358;
	
}

//quitter le jeu et nettoyer la memoire occupée

void exit_game(){
    	TTF_Quit();
    	IMG_Quit();
    	Mix_CloseAudio();
    	SDL_DestroyRenderer(renderer);
    	SDL_DestroyWindow(window);
    	SDL_Quit();
}

//changement de menu

void switch_menu(enum menu goto_menu){
	switch(goto_menu){
		case play:
			display_anim(renderer);
			play_menu(renderer);
			break;
		case settings:
			display_anim(renderer);
			settings_menu(renderer);
			break;
		case save:
			display_anim(renderer);
			save_menu(renderer);
			break;
		case highscores:
			display_anim(renderer);
			highscores_menu(renderer);
			break;
		case character:
			display_anim(renderer);
			character_menu(renderer);
			break;
		case enigma:
			display_anim(renderer);
			enigma_menu(renderer);
		case back:
			display_anim(renderer);
			main_menu(renderer);
			break;
		case exitgame:
			exit_anim(renderer);
			exit_game(window,renderer);
			break;
	}
}

//detection du bouton appui

void on_button_click_goto_menu(button *buttons,int count,int mouse_x,int mouse_y){
	button *pB;
	SDL_Point p={mouse_x,mouse_y};
	for (pB=buttons;pB<buttons+count;pB++){
		if(SDL_PointInRect(&p,&pB->position)){
			switch_menu(pB->type_menu);
		}
	}
}	
