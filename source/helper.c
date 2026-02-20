#include "helper.h"
struct menu_t mainMenu;
int main_menu_init = 0;
enum current_menu_state current_menu = MENU_MAIN;
//initialiser le jeu 
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int running = 0;
int init_game(){
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
	running=1;
	return 0;
}
void init_main_menu(SDL_Renderer *renderer, TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface.\n");
		return;
	}
	strcpy(mainMenu.buttons[0].label,"Play");
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
	for(int i=0; i<7;i++){
		SDL_Surface *button_surface=TTF_RenderText_Blended(font,
				mainMenu.buttons[i].label,DARK_GREY);
		mainMenu.buttons[i].texture=SDL_CreateTextureFromSurface(renderer,
						button_surface);
		SDL_FreeSurface(button_surface);
		
		button_surface=TTF_RenderText_Blended(font,
				mainMenu.buttons[i].label,LIGHT_GREY);
		mainMenu.buttons[i].texture_hovered=SDL_CreateTextureFromSurface(
					renderer,button_surface);
		mainMenu.buttons[i].position.x=120;
		mainMenu.buttons[i].position.y=90+i*30;
		mainMenu.buttons[i].position.h=button_surface->h;
		mainMenu.buttons[i].position.w=button_surface->w>160?160:button_surface->w;
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

void switch_menu(enum menu goto_menu){
	printf("switch_menu called with: %d\n", goto_menu);
	switch(goto_menu){
		case play:
			printf("  -> Switching to MENU_PLAY\n");
			current_menu = MENU_PLAY;
			break;
		case settings:
			printf("  -> Switching to MENU_SETTINGS\n");
			current_menu = MENU_SETTINGS;
			break;
		case save:
			printf("  -> Switching to MENU_SAVE\n");
			current_menu = MENU_SAVE;
			break;
		case highscores:
			printf("  -> Switching to MENU_HIGHSCORES\n");
			current_menu = MENU_HIGHSCORES;
			break;
		case character:
			printf("  -> Switching to MENU_CHARACTER\n");
			current_menu = MENU_CHARACTER;
			break;
		case enigma:
			//current_menu = MENU_ENIGMA;
			break;
		case back:
			printf("  -> Switching to MENU_MAIN\n");
			current_menu = MENU_MAIN;
			break;
		case exitgame:
			printf("  -> Exiting game\n");
			running = 0;
			break;
		default:
			printf("  -> Unknown menu type\n");
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
		SDL_Surface *curr_frame = IMG_Load(path);
		if(!curr_frame){
			printf("error loading %s",path);
			return;
		}
		SDL_Texture *curr_texture = SDL_CreateTextureFromSurface(renderer,curr_frame);
		SDL_FreeSurface(curr_frame);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,curr_texture,NULL,NULL);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(curr_texture);
		SDL_Delay(100);
	}
}
/*
void play_menu(SDL_Renderer *renderer,TTF_Font *font){
	
}
void settings_menu(SDL_Renderer *renderer,TTF_Font *font){
	
}
void save_menu(SDL_Renderer *renderer,TTF_Font *font){
		
}*/

struct menu_t highscores_menu_data;
int highscores_menu_init = 0;
Score highscores_list[6] = {{"Ahmed", 1500}, {"Sami", 1200}, {"Amira", 900},{"Ali",750},{"Khaled",500},{"Wissem",400}};

void init_highscores_menu(SDL_Renderer *renderer, TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface for highscores.\n");
		return;
	}
	highscores_menu_data.background = SDL_CreateTextureFromSurface(renderer, temp_surf);
	SDL_FreeSurface(temp_surf);
	highscores_menu_data.position.x = 0;
	highscores_menu_data.position.y = 0;
	highscores_menu_data.position.w = 600;
	highscores_menu_data.position.h = 358;
	
	char buffer[100];
	for (int i = 0; i < 6; i++) {
		sprintf(buffer, "%d. %s : %d", i+1, highscores_list[i].name, highscores_list[i].score);
		SDL_Surface* surface = TTF_RenderText_Blended(font, buffer, LIGHT_GREY);
		highscores_menu_data.elements[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		highscores_menu_data.elements[i].position.x = 120 + ((i % 3) * 230);
		highscores_menu_data.elements[i].position.y = 120 + ((i / 3) * 80);
		highscores_menu_data.elements[i].position.w = surface->w;
		highscores_menu_data.elements[i].position.h = surface->h;
		SDL_FreeSurface(surface);
	}
}

void highscores_menu(SDL_Renderer *renderer, TTF_Font *font){
	if(!highscores_menu_init){
		init_highscores_menu(renderer, font);
		highscores_menu_init = 1;
	}
	
	SDL_RenderClear(renderer);
	
	// Draw background
	SDL_RenderCopy(renderer, highscores_menu_data.background, NULL, &highscores_menu_data.position);
	
	// Draw scores
	for(int i = 0; i < 6; i++){
		SDL_RenderCopy(renderer, highscores_menu_data.elements[i].texture, NULL, &highscores_menu_data.elements[i].position);
	}
	
	SDL_RenderPresent(renderer);
}

struct menu_t character_menu_data;
int character_menu_init = 0;
int character_mode = 0;           // 0 = none | 1 = mono | 2 = multi
int character_avatar_choice = 0;  // 0 = none | 1 = avatar1 | 2 = avatar2

void init_character_menu(SDL_Renderer *renderer, TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface for character menu.\n");
		return;
	}
	character_menu_data.background = SDL_CreateTextureFromSurface(renderer, temp_surf);
	SDL_FreeSurface(temp_surf);
	character_menu_data.position.x = 0;
	character_menu_data.position.y = 0;
	character_menu_data.position.w = 600;
	character_menu_data.position.h = 358;
	
	strcpy(character_menu_data.buttons[0].label, "Singleplayer");
	character_menu_data.buttons[0].position = (SDL_Rect){120, 80, 160, 40};
	strcpy(character_menu_data.buttons[1].label, "Multiplayer");
	character_menu_data.buttons[1].position = (SDL_Rect){330, 80, 160, 40};
	strcpy(character_menu_data.buttons[2].label, "Avatar 1");
	character_menu_data.buttons[2].position = (SDL_Rect){100, 150, 160, 160};
	strcpy(character_menu_data.buttons[3].label, "Avatar 2");
	character_menu_data.buttons[3].position = (SDL_Rect){340, 150, 160, 160};
	strcpy(character_menu_data.buttons[4].label, "Confirm");
	character_menu_data.buttons[4].position = (SDL_Rect){225, 320, 150, 30};
	strcpy(character_menu_data.buttons[5].label, "Back");
	character_menu_data.buttons[5].position = (SDL_Rect){10, 10, 80, 30};
	
	for(int i = 0; i < 6; i++){
		SDL_Surface *btn_surface = TTF_RenderText_Blended(font, character_menu_data.buttons[i].label, DARK_GREY);
		character_menu_data.buttons[i].texture = SDL_CreateTextureFromSurface(renderer, btn_surface);
		SDL_FreeSurface(btn_surface);
	}
}

void character_menu(SDL_Renderer *renderer, TTF_Font *font){
	if(!character_menu_init){
		init_character_menu(renderer, font);
		character_menu_init = 1;
	}
	
	SDL_RenderClear(renderer);
	
	// Draw background
	SDL_RenderCopy(renderer, character_menu_data.background, NULL, &character_menu_data.position);
	
	// Draw mode selection buttons
	SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);
	SDL_RenderFillRect(renderer, &character_menu_data.buttons[0].position);
	SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
	SDL_RenderFillRect(renderer, &character_menu_data.buttons[1].position);
	
	// Draw avatar selection if mode is chosen
	if(character_mode != 0){
		// Avatar 1
		if(character_avatar_choice == 1)
			SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
		else
			SDL_SetRenderDrawColor(renderer, 200, 200, 100, 255);
		SDL_RenderFillRect(renderer, &character_menu_data.buttons[2].position);
		
		// Avatar 2
		if(character_avatar_choice == 2)
			SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
		else
			SDL_SetRenderDrawColor(renderer, 200, 200, 100, 255);
		SDL_RenderFillRect(renderer, &character_menu_data.buttons[3].position);
		
		// Confirm button
		if(character_avatar_choice != 0)
			SDL_SetRenderDrawColor(renderer, 150, 255, 150, 255);
		else
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(renderer, &character_menu_data.buttons[4].position);
	}
	
	// Draw back button
	SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255);
	SDL_RenderFillRect(renderer, &character_menu_data.buttons[5].position);
	
	SDL_RenderPresent(renderer);
}
/*
void enigma_menu(SDL_Renderer *renderer,TTF_Font *font){
	
}
*/
void highlight_hovered(SDL_Renderer *renderer,
                       button *buttons,
                       int btn_count,
                       int mouse_x,
                       int mouse_y)
{
    SDL_Point p = {mouse_x, mouse_y};

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer,
                   mainMenu.background,
                   NULL,
                   &mainMenu.position);

    for(int i = 0; i < btn_count; i++)
    {
        if(SDL_PointInRect(&p, &buttons[i].position)){
            SDL_RenderCopy(renderer,
                           buttons[i].texture_hovered,
                           NULL,
                           &buttons[i].position);
        }
        else{
            SDL_RenderCopy(renderer,
                           buttons[i].texture,
                           NULL,
                           &buttons[i].position);
        }
    }

    SDL_RenderPresent(renderer);
}
