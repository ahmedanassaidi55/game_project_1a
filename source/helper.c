#include "helper.h"
struct menu mainMenu;
int main_menu_init = 0;
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
void init_main_menu(SDL_Renderer *renderer, TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface.\n");
		return;
	}
	strcpy(MainMenu.buttons[0].label,"Play");
	mainMenu.buttons[0].type_menu=play;
	strcpy(mainMenu.buttons[1].label,"Settings");
	mainMenu.buttons[1].type_menu=settings;
	strcpy(mainMenu.buttons[2].label,"Save/Load");
	mainMenu.buttons[2].type_menu=save;
	strcpy(mainMenu.buttons[3].label,"Highscores");
	mainMenu.buttons[3].type_menu=highscores;
	strcpy(mainMenu.buttons[4].label,"Character menu");
	mainMenu.buttons[4].type_menu=character;
	strcpy(mainMenu.buttons[5].label,"Enigma");
	mainMenu.buttons[5].type_menu=enigma;
	strcpy(mainMenu.buttons[6].label,"Quit");
	mainMenu.buttons[6].type_menu=exitgame;
	mainMenu.background =SDL_CreateTextureFromSurface(renderer,temp_surf);
	SDL_FreeSurface(temp_surf);
	mainMenu.position.x = 0;
	mainMenu.position.y = 0;
	mainMenu.position.w = 600;
	mainMenu.position.h = 358;
	for(int i=0; i<5;i++){
		mainMenu.buttons[i].position.x=120;
		mainMenu.buttons[i].position.y=90+i*30;
		mainMenu.buttons[i].position.h=24;
		mainMenu.buttons[i].position.w=160;
		SDL_Surface *button_surface=TTF_RenderText_Blended(font,
				mainMenu.buttons[i].label,DARK_GREY);
		mainMenu.buttons[i].texture=SDL_CreateTextureFromSurface(renderer,button_surface);
		SDL_FreeSurface(button_surface);
	}
	
}
void main_menu(SDL_Renderer *renderer, TTF_Font *font){

    if(!main_menu_init){
        init_main_menu(renderer, font);
        main_menu_init = 1;
    }

    SDL_RenderClear(renderer);

    // Draw background
    SDL_RenderCopy(renderer,
                   mainMenu.background,
                   NULL,
                   &mainMenu.position);

    // Draw buttons
    for(int i = 0; i < 7; i++){
        SDL_RenderCopy(renderer,
                       mainMenu.buttons[i].texture,
                       NULL,
                       &mainMenu.buttons[i].position);
    }

    SDL_RenderPresent(renderer);
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

void switch_menu(enum menu goto_menu,TTF_Font *font,SDL_Renderer *renderer){
	switch(goto_menu){
		case play:
			display_anim(renderer);
			play_menu(renderer,font);
			break;
		case settings:
			display_anim(renderer);
			settings_menu(renderer,font);
			break;
		case save:
			display_anim(renderer);
			save_menu(renderer,font);
			break;
		case highscores:
			display_anim(renderer);
			highscores_menu(renderer,font);
			break;
		case character:
			display_anim(renderer);
			character_menu(renderer,font);
			break;
		case enigma:
			display_anim(renderer);
			enigma_menu(renderer,font);
		case back:
			display_anim(renderer);
			main_menu(renderer,font);
			break;
		case exitgame:
			exit_anim(renderer);
			running = 0;
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
