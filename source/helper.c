#include "helper.h"
struct menu_t mainMenu;
int main_menu_init = 0;
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
			//display_anim(renderer);
			//play_menu(renderer,font);
			break;
		case settings:
			//display_anim(renderer);
			//settings_menu(renderer,font);
			break;
		case save:
			//display_anim(renderer);
			//save_menu(renderer,font);
			break;
		case highscores:
			//display_anim(renderer);
			highscores_menu(renderer,font);
			break;
		case character:
			//display_anim(renderer);
			//character_menu(renderer,font);
			break;
		case enigma:
			//display_anim(renderer);
			//enigma_menu(renderer,font);
			break;
		case back:
			//display_anim(renderer);
			main_menu(renderer,font);
			break;
		case exitgame:
			//exit_anim(renderer,font);
			running = 0;
			break;
		default:
			
			break;
	}
}

//detection du bouton appui

void on_button_click_goto_menu(SDL_Renderer *renderer,TTF_Font *font,button *buttons,int count,int mouse_x,int mouse_y){
	button *pB;
	SDL_Point p={mouse_x,mouse_y};
	for (pB=buttons;pB<buttons+count;pB++){
		if(SDL_PointInRect(&p,&pB->position)){
			switch_menu(pB->type_menu,font,renderer);
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

void highscores_menu(SDL_Renderer *renderer,TTF_Font *font){
	Score list[6] = {{"Ahmed", 1500}, {"Sami", 1200}, {"Amira", 900},{"Ali",750},{"Khaled",500},{"Wissem",400}};
	SDL_Color white = {255, 255, 255, 255};
	char buffer[100];
	for (int i = 0; i < 6; i++) {
       		sprintf(buffer, "%d. %s : %d", i+1, list[i].name, list[i].score);
       	SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = { 120+((int)i/3)*230, 120 + (i * 50), surface->w*2, surface->h*2 };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        }
}

/*void character_menu(SDL_Renderer *renderer,TTF_Font *font)
{
    struct menu_t character_menu;
    SDL_Surface *surf=IMG_Load("assets/frame_01.jpg");
    character_menu.background=SDL_CreateTextureFromSurface(renderer,surf);
    SDL_FreeSurface(surf);
    strcpy(character_menu.buttons[0].label,"Singleplayer");
    character_menu.buttons[0].type_menu = mono;
    strcpy(character_menu.buttons[1].label,"Multiplayer");
    character_menu.buttons[1].type_menu = multi;
    strcpy(character_menu.buttons[2].label,"placeholdername1");
    character_menu.buttons[2].type_menu = character1;
    strcpy(character_menu.buttons[3].label,"placeholdername2");
    character_menu.buttons[3].type_menu = character2;
    strcpy(character_menu.buttons[4].label,"Back");
    character_menu.buttons[4].type_menu = back;
    character_menu.elements[0].position=(SDL_Rect){120,80,160,160};
    character_menu.elements[1].position=(SDL_Rect){330,80,160,160};
    
    int mode = 0;           // 0 = rien | 1 = mono | 2 = multi
    int avatarChoice = 0;   // 0 = rien | 1 = avatar1 | 2 = avatar2

    SDL_Event event;

    // ----------- Partie avatar -----------
    SDL_Rect avatar1 = {200, 320, 120, 50};
    SDL_Rect avatar2 = {450, 320, 120, 50};
    SDL_Rect valider = {350, 420, 120, 50};

    while (running)
    {
        // ========================
        // GESTION EVENTS
        // ========================
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                r;

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point p = {event.motion.x,event.motion.y};

                // Choix mode
                if (SDL_PointInRect(&p, &monoBtn))
                {
                    mode = 1;
                    avatarChoice = 0; // reset avatar
                }

                if (SDL_PointInRect(&p, &multiBtn))
                {
                    mode = 2;
                    avatarChoice = 0;
                }

                // Choix avatar
                if (mode != 0)
                {
                    if (SDL_PointInRect(&p, &avatar1))
                        avatarChoice = 1;

                    if (SDL_PointInRect(&p, &avatar2))
                        avatarChoice = 2;
                }

                // Valider
                if (mode != 0 && avatarChoice != 0 &&
                    SDL_PointInRect(&p, &valider))
                {
                    printf("Mode: %d | Avatar: %d\n",
                           mode, avatarChoice);
                    printf("Go to best scores menu\n");
                }

                // Retour
                if (SDL_PointInRect(&p, &retourBtn)){
                    display_anim(renderer);
                    main_menu(renderer,font);}
                    
            }

            // Touche ENTER
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN &&
                    mode != 0 && avatarChoice != 0)
                {
                    printf("Mode: %d | Avatar: %d\n",
                           mode, avatarChoice);
                    printf("Go to best scores menu (ENTER)\n");
                }
            }
        }

        // ========================
        // RENDER
        // ========================
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Mono
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &monoBtn);

        // Multi
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &multiBtn);

        // Retour
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &retourBtn);

        // Partie Avatar visible seulement si mode choisi
        if (mode != 0)
        {
            // Avatar 1
            if (avatarChoice == 1)
                SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

            SDL_RenderFillRect(renderer, &avatar1);

            // Avatar 2
            if (avatarChoice == 2)
                SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

            SDL_RenderFillRect(renderer, &avatar2);

            // Valider
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &valider);
        }

        SDL_RenderPresent(renderer);
    }
}*/
/*
void enigma_menu(SDL_Renderer *renderer,TTF_Font *font){
	
}
*/
void highlight_hovered(SDL_Renderer *renderer,
                       button *buttons,
                       int btn_count,
                       TTF_Font *font,
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
        SDL_Color color;

        if(SDL_PointInRect(&p, &buttons[i].position))
            color = LIGHT_GREY;
        else
            color = DARK_GREY;

        SDL_Surface *text_surf =
            TTF_RenderText_Blended(font,
                                   buttons[i].label,
                                   color);

        if(!text_surf)
            continue;

        if(buttons[i].texture)
            SDL_DestroyTexture(buttons[i].texture);

        buttons[i].texture =
            SDL_CreateTextureFromSurface(renderer, text_surf);

        SDL_FreeSurface(text_surf);

        SDL_RenderCopy(renderer,
                       buttons[i].texture,
                       NULL,
                       &buttons[i].position);
    }

    SDL_RenderPresent(renderer);
}
