// ======== Enigma Menu =========
struct menu_t enigma_menu_data;
int enigma_menu_init = 0;
Mix_Chunk *quiz_music = NULL;
Mix_Chunk *hover_sound = NULL;

void init_enigma_menu(SDL_Renderer *renderer, TTF_Font *font){
    // Charger le 4ème arrière-plan
    SDL_Surface *temp_surf = IMG_Load("assets/images/frame_04.jpg");
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
    enigma_menu_data.buttons[0].type_menu = play; // on traitera dans handle_events
    enigma_menu_data.buttons[0].position = (SDL_Rect){180,150,100,40};

    strcpy(enigma_menu_data.buttons[1].label,"Puzzle");
    enigma_menu_data.buttons[1].type_menu = play; // Placeholder
    enigma_menu_data.buttons[1].position = (SDL_Rect){320,150,100,40};

    // Textures des boutons
    for(int i=0;i<2;i++){
        SDL_Surface *surf = TTF_RenderText_Blended(font, enigma_menu_data.buttons[i].label, DARK_GREY);
        enigma_menu_data.buttons[i].texture = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(font, enigma_menu_data.buttons[i].label, LIGHT_GREY);
        enigma_menu_data.buttons[i].texture_hovered = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
    }

    // Charger sons
    hover_sound = Mix_LoadWAV("assets/audio/hover.wav");
    quiz_music = Mix_LoadWAV("assets/audio/quiz_suspense.wav");

    enigma_menu_init = 1;
}

int quiz_active = 0;
SDL_Rect quiz_band = {50,50,500,60};
SDL_Rect ans_buttons[3] = {{100,150,150,40},{225,150,150,40},{350,150,150,40}};
char *quiz_question = "*****"; // Question placeholder
char *quiz_answers[3] = {"*****","*****","*****"};

void enigma_menu(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y){
    if(!enigma_menu_init) init_enigma_menu(renderer,font);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,enigma_menu_data.background,NULL,&enigma_menu_data.position);

    SDL_Point p = {mouse_x,mouse_y};
    for(int i=0;i<2;i++){
        if(SDL_PointInRect(&p,&enigma_menu_data.buttons[i].position)){
            SDL_RenderCopy(renderer,enigma_menu_data.buttons[i].texture_hovered,NULL,&enigma_menu_data.buttons[i].position);
            Mix_PlayChannel(-1,hover_sound,0);
        } else {
            SDL_RenderCopy(renderer,enigma_menu_data.buttons[i].texture,NULL,&enigma_menu_data.buttons[i].position);
        }
    }

    // Si Quiz actif
    if(quiz_active){
        // Bande pour question
        SDL_SetRenderDrawColor(renderer,0,0,0,200);
        SDL_RenderFillRect(renderer,&quiz_band);
        SDL_Surface *q_surf = TTF_RenderText_Blended(font,quiz_question,WHITE);
        SDL_Texture *q_tex = SDL_CreateTextureFromSurface(renderer,q_surf);
        SDL_FreeSurface(q_surf);
        SDL_RenderCopy(renderer,q_tex,NULL,&quiz_band);
        SDL_DestroyTexture(q_tex);

        // Boutons réponses
        for(int i=0;i<3;i++){
            SDL_Surface *ans_surf = TTF_RenderText_Blended(font,quiz_answers[i],DARK_GREY);
            SDL_Texture *ans_tex = SDL_CreateTextureFromSurface(renderer,ans_surf);
            SDL_FreeSurface(ans_surf);

            SDL_RenderCopy(renderer,ans_tex,NULL,&ans_buttons[i]);
            SDL_DestroyTexture(ans_tex);
        }
    }

    SDL_RenderPresent(renderer);
}

void handle_enigma_events(SDL_Event *event){
    if(event->type==SDL_KEYDOWN && event->key.keysym.sym==SDLK_ESCAPE){
        running=0; // quitter le jeu
    }
    if(event->type==SDL_MOUSEBUTTONDOWN){
        int x=event->button.x;
        int y=event->button.y;
        SDL_Point p={x,y};
        // Quiz bouton
        if(SDL_PointInRect(&p,&enigma_menu_data.buttons[0].position)){
            quiz_active=1;
            Mix_PlayChannel(-1,quiz_music,0);
        }
        // Puzzle bouton
        if(SDL_PointInRect(&p,&enigma_menu_data.buttons[1].position)){
            quiz_active=0; // placeholder
        }
    }
}
