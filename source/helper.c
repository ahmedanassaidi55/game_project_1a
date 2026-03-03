#include "helper.h"
struct menu_t mainMenu;
Mix_Chunk *music;
int volume = 5;
int paused = 0;
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
        600, 358,
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
	strcpy(mainMenu.buttons[2].label,"Highscores");
	mainMenu.buttons[2].type_menu=highscores;
	strcpy(mainMenu.buttons[3].label,"Story");
	mainMenu.buttons[3].type_menu=story;
	strcpy(mainMenu.buttons[4].label,"Quit");
	mainMenu.buttons[4].type_menu=exitgame;
	mainMenu.background =SDL_CreateTextureFromSurface(renderer,temp_surf);
	SDL_FreeSurface(temp_surf);
	mainMenu.position.x = 0;
	mainMenu.position.y = 0;
	mainMenu.position.w = 600;
	mainMenu.position.h = 358;
	for(int i=0; i<5;i++){
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
    SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y);
    for(int i = 0; i < 7; i++){
   	if(SDL_PointInRect(&p,&mainMenu.buttons[i].position)){
        SDL_RenderCopy(renderer,
                       mainMenu.buttons[i].texture_hovered,
                       NULL,
                       &mainMenu.buttons[i].position);
    	}else{
    	SDL_RenderCopy(renderer,
                       mainMenu.buttons[i].texture,
                       NULL,
                       &mainMenu.buttons[i].position);
                       }
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

void switch_menu(enum menu goto_menu,TTF_Font *font){
	printf("switch_menu called with: %d\n", goto_menu);
	switch(goto_menu){
		case play:
			current_menu = MENU_CHARACTER;
			break;
		case settings:
			current_menu = MENU_SETTINGS;
			break;
		case highscores:
			current_menu = MENU_HIGHSCORES;
			break;
		case character:
			printf("  -> Switching to MENU_CHARACTER\n");
			current_menu = MENU_CHARACTER;
			break;
		case enigma:
			printf("  -> Switching to MENU_ENIGMA\n");
			current_menu = MENU_ENIGMA;
			break;
		case back:
			printf("  -> Switching to MENU_MAIN\n");
			current_menu = MENU_MAIN;
			break;
		case exitgame:
			printf("  -> Exiting game\n");
			running = 0;
			break;
		case audio_inc:
			volume++;
			volume = volume>10?10:volume;
			Mix_Volume(-1,volume*13);
			SDL_Surface *elem_surf=TTF_RenderText_Blended(font,
		settings_menu_data.elements[3].label+(10-volume), BLACK);
	settings_menu_data.elements[3].texture=
		SDL_CreateTextureFromSurface(renderer,elem_surf);
	settings_menu_data.elements[3].position.w = elem_surf->w;
	settings_menu_data.elements[3].position.h = elem_surf->h;
	SDL_FreeSurface(elem_surf);
			break;
		case audio_dec:
			volume--;
			volume = volume<1?1:volume;
			Mix_Volume(-1,volume*13);
			SDL_Surface *elemsurf=TTF_RenderText_Blended(font,
		settings_menu_data.elements[3].label+(10-volume), BLACK);
		settings_menu_data.elements[3].texture=
		SDL_CreateTextureFromSurface(renderer,elemsurf);
		settings_menu_data.elements[3].position.w = elemsurf->w;
		settings_menu_data.elements[3].position.h = elemsurf->h;
		SDL_FreeSurface(elemsurf);
			break;
		case fullscreen:
		if(SDL_GetWindowFlags(window)&SDL_WINDOW_FULLSCREEN || SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP){
		strcpy(settings_menu_data.buttons[3].label,"O");
		SDL_SetWindowFullscreen(window,0);
		}else{
		strcpy(settings_menu_data.buttons[3].label,"X");
		SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
		}
		SDL_Surface *btn_surf=TTF_RenderText_Blended(font,
			settings_menu_data.buttons[3].label, BLACK);
			SDL_Surface *hvr=TTF_RenderText_Blended(font,
			settings_menu_data.buttons[3].label, LIGHT_GREY);
		settings_menu_data.buttons[3].texture=
		SDL_CreateTextureFromSurface(renderer,btn_surf);
		settings_menu_data.buttons[3].texture_hovered=
		SDL_CreateTextureFromSurface(renderer,hvr);
		settings_menu_data.buttons[3].position.w = btn_surf->w;
		settings_menu_data.buttons[3].position.h = btn_surf->h;
		SDL_FreeSurface(btn_surf);
		SDL_FreeSurface(hvr);
			break;
		case mono:
			character_mode = 1;
			break;
		case multi:
			character_mode = 2;
			break;
		case character1:
			character_avatar_choice = 1;
			break;
		case character2:
			character_avatar_choice = 2;
			break;
		case confirm:
			current_menu = MENU_SAVE;
			break;
		case quiz:
		quiz_active = 1;
		break;
		case puzzle:
		break;
		case new_game:
		printf("newgame\n");
		break;
		case load_game:
		printf("load\n");
		break;
		default:
			printf("  -> Unknown menu type\n");
			break;
			
	}
}

//detection du bouton appui

void on_button_click_goto_menu(button *buttons,int count,int mouse_x,int mouse_y,TTF_Font *font){
	button *pB;
	SDL_Point p={mouse_x,mouse_y};
	for (pB=buttons;pB<buttons+count;pB++){
		if(SDL_PointInRect(&p,&pB->position)){
			switch_menu(pB->type_menu,font);
			break;
		}
	}
}
//animation lors de changement de menu
SDL_Texture *frames[11];
void init_anim(SDL_Renderer *renderer){
	char path[64];
	for(int i = 10;i >=1;i--){
		sprintf(path,"assets/frame_%02d.jpg",i);
		frames[i] = IMG_LoadTexture(renderer,path);
	}
}
/*void display_anim(SDL_Renderer *renderer){
	for(int i = 10;i >=1;i--){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) exit(0);
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, frames[i],NULL,NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(30);
	}
}*/
/*
void play_menu(SDL_Renderer *renderer,TTF_Font *font){
	
}*/
struct menu_t settings_menu_data;
int settings_menu_init = 0;
void init_settings_menu(SDL_Renderer *renderer,TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface for highscores.\n");
		return;
	}
	settings_menu_data.background = SDL_CreateTextureFromSurface(
						renderer,temp_surf);
	SDL_FreeSurface(temp_surf);
	settings_menu_data.position.x = 0;
	settings_menu_data.position.y = 0;
	settings_menu_data.position.w = 600;
	settings_menu_data.position.h = 358;
	strcpy(settings_menu_data.buttons[0].label,"back");
	settings_menu_data.buttons[0].type_menu = back;
	settings_menu_data.buttons[0].position.x = 390;
	settings_menu_data.buttons[0].position.y = 270;
	strcpy(settings_menu_data.buttons[1].label,"+");
	settings_menu_data.buttons[1].type_menu = audio_inc;
	settings_menu_data.buttons[1].position.x = 450;
	settings_menu_data.buttons[1].position.y = 130;
	strcpy(settings_menu_data.buttons[2].label,"-");
	settings_menu_data.buttons[2].type_menu = audio_dec;
	settings_menu_data.buttons[2].position.x = 340;
	settings_menu_data.buttons[2].position.y = 130;
	strcpy(settings_menu_data.buttons[3].label,"O");
	settings_menu_data.buttons[3].type_menu = fullscreen;
	settings_menu_data.buttons[3].position.x = 400;
	settings_menu_data.buttons[3].position.y = 200;
	strcpy(settings_menu_data.elements[0].label,"Settings");
	settings_menu_data.elements[0].position.x = 130;
	settings_menu_data.elements[0].position.y = 75;
	strcpy(settings_menu_data.elements[1].label,"Volume");
	settings_menu_data.elements[1].position.x = 105;
	settings_menu_data.elements[1].position.y = 130;
	strcpy(settings_menu_data.elements[2].label,"FullScreen");
	settings_menu_data.elements[2].position.x = 105;
	settings_menu_data.elements[2].position.y = 200;
	strcpy(settings_menu_data.elements[3].label,"||||||||||");
	settings_menu_data.elements[3].position.x = 360;
	settings_menu_data.elements[3].position.y = 130;
	for(int i=0; i<4;i++){
		SDL_Surface *btn_surf=TTF_RenderText_Blended(font,
			settings_menu_data.buttons[i].label, BLACK);
		settings_menu_data.buttons[i].texture=
		SDL_CreateTextureFromSurface(renderer,btn_surf);
		settings_menu_data.buttons[i].position.w = btn_surf->w;
		settings_menu_data.buttons[i].position.h = btn_surf->h;
		SDL_FreeSurface(btn_surf);
		SDL_Surface *surf = TTF_RenderText_Blended(font, settings_menu_data.buttons[i].label, LIGHT_GREY);
        settings_menu_data.buttons[i].texture_hovered = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
	}
	for(int i=0; i<3;i++){
		SDL_Surface *elem_surf=TTF_RenderText_Blended(font,
			settings_menu_data.elements[i].label, BLACK);
		settings_menu_data.elements[i].texture=
			SDL_CreateTextureFromSurface(renderer,elem_surf);
		settings_menu_data.elements[i].position.w = elem_surf->w;
		settings_menu_data.elements[i].position.h = elem_surf->h;
		SDL_FreeSurface(elem_surf);
	}
	SDL_Surface *elem_surf=TTF_RenderText_Blended(font,
		settings_menu_data.elements[3].label+(10-volume), BLACK);
	settings_menu_data.elements[3].texture=
		SDL_CreateTextureFromSurface(renderer,elem_surf);
	settings_menu_data.elements[3].position.w = elem_surf->w;
	settings_menu_data.elements[3].position.h = elem_surf->h;
	SDL_FreeSurface(elem_surf);
	
}
void settings_menu(SDL_Renderer *renderer,TTF_Font *font){
	if(!settings_menu_init){
		init_settings_menu(renderer,font);
		settings_menu_init = 1;
	}
	SDL_RenderClear(renderer);
	
	SDL_RenderCopy(renderer, settings_menu_data.background, NULL,
					&settings_menu_data.position);
	
	for(int i = 0; i < 4; i++){
		SDL_RenderCopy(renderer,
			settings_menu_data.elements[i].texture, NULL,
			&settings_menu_data.elements[i].position);
	}
	SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y);
    for(int i = 0; i < 4; i++){
   	if(SDL_PointInRect(&p,&settings_menu_data.buttons[i].position)){
        SDL_RenderCopy(renderer,
                       settings_menu_data.buttons[i].texture_hovered,
                       NULL,
                       &settings_menu_data.buttons[i].position);
    	}else{
    	SDL_RenderCopy(renderer,
                       settings_menu_data.buttons[i].texture,
                       NULL,
                       &settings_menu_data.buttons[i].position);
                       }
	}
	SDL_RenderPresent(renderer);
}

struct menu_t save_menu_data;
int save_menu_init = 0;
void init_save_menu(SDL_Renderer *renderer,TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface for highscores.\n");
		return;
	}
	save_menu_data.background = SDL_CreateTextureFromSurface(
						renderer,temp_surf);
	save_menu_data.position=(SDL_Rect){0,0,600,358};
	SDL_FreeSurface(temp_surf);
	strcpy(save_menu_data.buttons[0].label,"New game");
	save_menu_data.buttons[0].position.x = 110;
	save_menu_data.buttons[0].position.y = 120;
	save_menu_data.buttons[0].type_menu = new_game;
	strcpy(save_menu_data.buttons[1].label,"Load game");
	save_menu_data.buttons[1].position.x = 320;
	save_menu_data.buttons[1].position.y = 120;
	save_menu_data.buttons[1].type_menu = load_game;
	strcpy(save_menu_data.buttons[2].label,"Back");
	save_menu_data.buttons[2].position.x = 120;
	save_menu_data.buttons[2].position.y = 250;
	save_menu_data.buttons[2].type_menu = character;

for(int i=0; i<3;i++){
		SDL_Surface *elem_surf=TTF_RenderText_Blended(font,
			save_menu_data.buttons[i].label, BLACK);
		save_menu_data.buttons[i].texture=
			SDL_CreateTextureFromSurface(renderer,elem_surf);
		save_menu_data.buttons[i].position.w = elem_surf->w;
		save_menu_data.buttons[i].position.h = elem_surf->h;
		SDL_FreeSurface(elem_surf);
		SDL_Surface *surf=TTF_RenderText_Blended(font,
			save_menu_data.buttons[i].label, LIGHT_GREY);
		save_menu_data.buttons[i].texture_hovered=
			SDL_CreateTextureFromSurface(renderer,surf);
		SDL_FreeSurface(surf);
	}
}
void save_menu(SDL_Renderer *renderer,TTF_Font *font){
	if(!save_menu_init){
	init_save_menu(renderer,font);
	save_menu_init = 1;
	}
	SDL_RenderClear(renderer);
	
	SDL_RenderCopy(renderer, save_menu_data.background, NULL,
					&save_menu_data.position);
	SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y);
    for(int i = 0; i < 3; i++){
   	if(SDL_PointInRect(&p,&save_menu_data.buttons[i].position)){
        SDL_RenderCopy(renderer,
                       save_menu_data.buttons[i].texture_hovered,
                       NULL,
                       &save_menu_data.buttons[i].position);
    	}else{
    	SDL_RenderCopy(renderer,
                       save_menu_data.buttons[i].texture,
                       NULL,
                       &save_menu_data.buttons[i].position);
                       }
	}
	SDL_RenderPresent(renderer);
}

struct menu_t highscores_menu_data;
int highscores_menu_init = 0;
Score highscores_list[6];
//va etre remplacer par une propre fonction a trouver les scores
void init_highscores_menu(SDL_Renderer *renderer, TTF_Font *font){
	SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
	if(!temp_surf){
		printf("error creating surface for highscores.\n");
		return;
	}
	FILE* f = fopen("saves/highscores.txt","r");
	if(!f){
		printf("couldn't find highscores\n");
	}
	for(int i = 0;i<6;i++){
	fscanf(f,"%s %d",highscores_list[i].name,&highscores_list[i].score);
	}
	fclose(f);
	highscores_menu_data.background = SDL_CreateTextureFromSurface(renderer, temp_surf);
	SDL_FreeSurface(temp_surf);
	highscores_menu_data.position.x = 0;
	highscores_menu_data.position.y = 0;
	highscores_menu_data.position.w = 600;
	highscores_menu_data.position.h = 358;
	
	char buffer[100];
	for (int i = 0; i < 6; i++) {
		sprintf(buffer, "%d. %s : %d", i+1, highscores_list[i].name, highscores_list[i].score);
		SDL_Surface* surface = TTF_RenderText_Blended(font, buffer, (SDL_Color){255,255,255,255});
		highscores_menu_data.elements[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		highscores_menu_data.elements[i].position.x = 120 + ((i % 2) * 200);
		highscores_menu_data.elements[i].position.y = 120 + ((i / 2) * 50);
		highscores_menu_data.elements[i].position.w = 160;
		highscores_menu_data.elements[i].position.h = 21;
		SDL_FreeSurface(surface);
	}
	strcpy(highscores_menu_data.buttons[0].label,"Back");
	highscores_menu_data.buttons[0].type_menu=back;
	SDL_Surface *surf=TTF_RenderText_Blended(font, highscores_menu_data.buttons[0].label,BLACK);
	highscores_menu_data.buttons[0].texture =SDL_CreateTextureFromSurface(renderer, surf);
	highscores_menu_data.buttons[0].position =(SDL_Rect){120,270,80,20};
	SDL_FreeSurface(surf);
	surf = TTF_RenderText_Blended(font, highscores_menu_data.buttons[0].label, LIGHT_GREY);
        highscores_menu_data.buttons[0].texture_hovered = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
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
		SDL_RenderCopy(renderer,
			highscores_menu_data.elements[i].texture, NULL,
			&highscores_menu_data.elements[i].position);
	}
	SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y);
   	if(SDL_PointInRect(&p,&highscores_menu_data.buttons[0].position)){
        SDL_RenderCopy(renderer,
                       highscores_menu_data.buttons[0].texture_hovered,
                       NULL,
                       &highscores_menu_data.buttons[0].position);
    	}else{
    	SDL_RenderCopy(renderer,
                       highscores_menu_data.buttons[0].texture,
                       NULL,
                       &highscores_menu_data.buttons[0].position);
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
	character_menu_data.buttons[0].type_menu = mono;
	character_menu_data.buttons[0].position = (SDL_Rect){160, 250, 80, 21};
	strcpy(character_menu_data.buttons[1].label, "Multiplayer");
	character_menu_data.buttons[1].type_menu = multi;
	character_menu_data.buttons[1].position = (SDL_Rect){370, 250, 80, 21};
	strcpy(character_menu_data.buttons[2].label, "Confirm");
	character_menu_data.buttons[2].type_menu = confirm;
	character_menu_data.buttons[2].position = (SDL_Rect){120, 275, 80, 21};
	
	strcpy(character_menu_data.buttons[3].label, "Back");
	character_menu_data.buttons[3].type_menu = back;
	character_menu_data.buttons[3].position = (SDL_Rect){370, 275, 80, 21};
	character_menu_data.buttons[4].type_menu = character1;
	character_menu_data.buttons[4].position = (SDL_Rect){120, 80, 160, 160};
	character_menu_data.buttons[5].type_menu = character2;
	character_menu_data.buttons[5].position = (SDL_Rect){330, 80, 160, 160};
	
	for(int i = 0; i < 4; i++){
		SDL_Surface *btn_surface = TTF_RenderText_Blended(font, character_menu_data.buttons[i].label, BLACK);
		character_menu_data.buttons[i].texture = SDL_CreateTextureFromSurface(renderer, btn_surface);
		SDL_FreeSurface(btn_surface);
		SDL_Surface *surf = TTF_RenderText_Blended(font, character_menu_data.buttons[i].label, LIGHT_GREY);
        character_menu_data.buttons[i].texture_hovered = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
	}
	for(int i = 0; i < 2; i++){
		SDL_Surface *btn_surface = TTF_RenderText_Blended(font, character_menu_data.elements[i].label, BLACK);
		character_menu_data.elements[i].texture = SDL_CreateTextureFromSurface(renderer, btn_surface);
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
	SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y);
    for(int i = 0; i < 4; i++){
   	if(SDL_PointInRect(&p,&character_menu_data.buttons[i].position)){
        SDL_RenderCopy(renderer,
                       character_menu_data.buttons[i].texture_hovered,
                       NULL,
                       &character_menu_data.buttons[i].position);
    	}else{
    	SDL_RenderCopy(renderer,
                       character_menu_data.buttons[i].texture,
                       NULL,
                       &character_menu_data.buttons[i].position);
                       }
	}
	
	// Draw avatar selection if mode is chosen
	if(character_mode != 0){
		// Avatar 1
		if(character_avatar_choice == 1)
			SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
		else
			SDL_SetRenderDrawColor(renderer, 200, 200, 100, 255);
		SDL_RenderFillRect(renderer, &character_menu_data.buttons[4].position);
		
		// Avatar 2
		if(character_avatar_choice == 2)
			SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
		else
			SDL_SetRenderDrawColor(renderer, 200, 200, 100, 255);
		SDL_RenderFillRect(renderer, &character_menu_data.buttons[5].position);
	}  
	
	SDL_RenderPresent(renderer);
}  
struct menu_t enigma_menu_data;
int enigma_menu_init = 0;
void init_enigma_menu(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Surface *temp_surf = IMG_Load("assets/frame_01.jpg");
    if(!temp_surf){
        printf("Error loading enigma background\n");
        return;
    }
    enigma_menu_data.background = SDL_CreateTextureFromSurface(renderer,temp_surf);
    SDL_FreeSurface(temp_surf);

    enigma_menu_data.position.x = 0;
    enigma_menu_data.position.y = 0;
    enigma_menu_data.position.w = 600;
    enigma_menu_data.position.h = 358;

    // Boutons Quiz et Puzzle
    strcpy(enigma_menu_data.buttons[0].label,"Quiz");
    enigma_menu_data.buttons[0].type_menu = quiz; 
    enigma_menu_data.buttons[0].position = (SDL_Rect){150,200,80,30};

    strcpy(enigma_menu_data.buttons[1].label,"Puzzle");
    enigma_menu_data.buttons[1].type_menu = puzzle;
    enigma_menu_data.buttons[1].position = (SDL_Rect){370,200,80,30};
    strcpy(enigma_menu_data.buttons[2].label,"Back");
    enigma_menu_data.buttons[2].type_menu = back;
    enigma_menu_data.buttons[2].position = (SDL_Rect){120,270,60,20};

    for(int i=0;i<3;i++){
        SDL_Surface *surf = TTF_RenderText_Blended(font, enigma_menu_data.buttons[i].label, DARK_GREY);
        enigma_menu_data.buttons[i].texture = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(font, enigma_menu_data.buttons[i].label, LIGHT_GREY);
        enigma_menu_data.buttons[i].texture_hovered = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
    }
    enigma_menu_init = 1;
}
int quiz_active = 0;
SDL_Rect quiz_band = {50,50,500,60};
SDL_Rect ans_buttons[3] = {{160,150,70,30},{350,150,70,30},{290,220,70,30}};
char *quiz_question = "*****"; // Question placeholder
char *quiz_answers[3] = {"*****","*****","*****"};

void enigma_menu(SDL_Renderer *renderer, TTF_Font *font){
    if(!enigma_menu_init) init_enigma_menu(renderer,font);

    SDL_RenderClear(renderer);
    SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y); SDL_RenderCopy(renderer,enigma_menu_data.background,NULL,&enigma_menu_data.position);
    if(quiz_active){
        // Bande pour question
        SDL_SetRenderDrawColor(renderer,0,0,0,200);
        SDL_RenderFillRect(renderer,&quiz_band);
        SDL_Surface *q_surf = TTF_RenderText_Blended(font,quiz_question,WHITE);
        SDL_Texture *q_tex = SDL_CreateTextureFromSurface(renderer,q_surf);
        SDL_FreeSurface(q_surf);
        SDL_RenderCopy(renderer,q_tex,NULL,&quiz_band);
        SDL_DestroyTexture(q_tex);

        for(int i=0;i<3;i++){
            SDL_Surface *ans_surf = TTF_RenderText_Blended(font,quiz_answers[i],DARK_GREY);
            SDL_Texture *ans_tex = SDL_CreateTextureFromSurface(renderer,ans_surf);
            SDL_FreeSurface(ans_surf);

            SDL_RenderCopy(renderer,ans_tex,NULL,&ans_buttons[i]);
            SDL_DestroyTexture(ans_tex);
        }
    }else{
    
    for(int i = 0; i < 4; i++){
   	if(SDL_PointInRect(&p,&enigma_menu_data.buttons[i].position)){
        SDL_RenderCopy(renderer,
                       enigma_menu_data.buttons[i].texture_hovered,
                       NULL,
                       &enigma_menu_data.buttons[i].position);
    	}else{
    	SDL_RenderCopy(renderer,
                       enigma_menu_data.buttons[i].texture,
                       NULL,
                       &enigma_menu_data.buttons[i].position);
                       }
	}
    }

    SDL_RenderPresent(renderer);
}
