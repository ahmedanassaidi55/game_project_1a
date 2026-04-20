#include "background.h"
game_clock clk;
//a dynamic array for number of players(or backgrounds here), because memory leaks are fun ^^
int background_init=0;
Background *bg = NULL;

Background* init_background(SDL_Renderer *renderer,int player_count){
	bg = (Background*)malloc(sizeof(Background)*player_count);
	if(!bg){
		printf("error allocating @init_background\n");
		return NULL;
	}
		SDL_Surface *s = IMG_Load("assets/images/mapbg.png");
		if(!s){
			printf("invalid path.\n");
			free(bg);
			return NULL;
		}
		SDL_Surface *surface_coll = IMG_Load("assets/images/collmapbg.png");
		if(!surface_coll){
			printf("invalid path.\n");
			free(bg);
			return NULL;
		}
	SDL_Texture *bgtexture = SDL_CreateTextureFromSurface(renderer,s);
		int w = s->w;
		int h = s->h;
		SDL_FreeSurface(s);
		if(!bgtexture){
			printf("error while creating texture.\n");
			free(bg);
			return NULL;
		}
	FILE *f = fopen("saves/elem_pos.txt","r");
	if (f == NULL){
			printf("couldn't find file");
			free(bg);
			return NULL;
		}
	for(Background* pB =bg;pB<bg+player_count;pB++){
		rewind(f);
		pB->texture = bgtexture;
		pB->offsetX=0;
		pB->offsetY=0;
		pB->position =(SDL_Rect){0,0,w,h};
		
		for(int i = 0; i < OBJ_COUNT; i++){
			char elem_path[30];
			sprintf(elem_path,"assets/images/elem%d.png",i);
			pB->objects[i].base = IMG_LoadTexture(renderer,elem_path);
			sprintf(elem_path,"assets/images/elemu%d.png",i);
			pB->objects[i].used = IMG_LoadTexture(renderer,elem_path);
			if(!pB->objects[i].base || !pB->objects[i].used){
    				printf("texture load failed @init_bg\n");
    				for(Background* p = bg; p <= pB; p++){
				    for(int j = 0; j < i; j++){
					if(p->objects[j].base)
				SDL_DestroyTexture(p->objects[j].base);
					if(p->objects[j].used)
				SDL_DestroyTexture(p->objects[j].used);
				    }
				}
				SDL_DestroyTexture(bgtexture);
				fclose(f);
				free(bg);
				return NULL;
}
			if(fscanf(f,"%d|%d|%d|%d|%d",&pB->objects[i].position.x,
			&pB->objects[i].position.y,
			&pB->objects[i].position.w,
			&pB->objects[i].position.h,
			&pB->objects[i].is_interactedwith
			)!=5){
				printf("failed to initialize object: %d .\n",i);
				break;
			}	
		}
		
	}
	fclose(f);
	background_init=1;
	clk.disp_clock[0]='\0';
	clk.start = SDL_GetTicks()/1000;
	return bg;
}

void display_background(Background* bg,SDL_Renderer *renderer,int player_count){
	if(!bg){
		printf("error initializing background@display_bg\n");
		return;
	}
	for(Background* pB =bg;pB<bg+player_count;pB++){
	int index = pB - bg;
	SDL_Rect dest = {(SCREEN_WIDTH / player_count) * index,
	    0,
	    SCREEN_WIDTH / player_count,
	    SCREEN_HEIGHT
	};
	SDL_Rect src = {pB->offsetX,
		pB->offsetY,
		SCREEN_WIDTH / player_count,
    		SCREEN_HEIGHT
	};

	SDL_RenderCopy(renderer, pB->texture, &src, &dest);
	for(int i = 0; i < OBJ_COUNT; i++){
	int screenX = pB->objects[i].position.x - pB->offsetX + dest.x;
	int screenY = pB->objects[i].position.y - pB->offsetY;
	if (screenX + pB->objects[i].position.w < dest.x ||
    screenX > dest.x + dest.w ||
    screenY + pB->objects[i].position.h < 0 ||
    screenY > SCREEN_HEIGHT) {
    continue;
}
	if(pB->objects[i].is_interactedwith){
		SDL_RenderCopy(renderer, pB->objects[i].used,NULL,&(SDL_Rect){screenX,screenY,pB->objects[i].position.w,pB->objects[i].position.h});
	}else{
	SDL_RenderCopy(renderer, pB->objects[i].base,NULL,&(SDL_Rect){screenX,screenY,pB->objects[i].position.w,pB->objects[i].position.h});}
		}
	}
}
void scroll_background(Background *bg, enum direction d, int step){
	float diag_mvt = 0.7071f;
	int x = bg->offsetX;
	int y = bg->offsetY; // to keep constant speed in diagonal directions
	switch(d){
	case up:
	y = y-step<0?0:y-step;
	break;
	case up_right:
	y = y-step*diag_mvt<0?0:y-step*diag_mvt;
	x = x+step*diag_mvt>(BACKGROUND_WIDTH-SCREEN_WIDTH)?BACKGROUND_WIDTH-SCREEN_WIDTH:x+step*diag_mvt;
	break;
	case right:
	x = x+step>(BACKGROUND_WIDTH-SCREEN_WIDTH)?BACKGROUND_WIDTH-SCREEN_WIDTH:x+step;
	break;
	case down_right:
	y = y+step*diag_mvt>(BACKGROUND_HEIGHT-SCREEN_HEIGHT)?BACKGROUND_HEIGHT-SCREEN_HEIGHT:y+ step*diag_mvt;
	x = x + step*diag_mvt > (BACKGROUND_WIDTH -SCREEN_WIDTH)
	? (BACKGROUND_WIDTH - SCREEN_WIDTH) : x + step*diag_mvt;
	break;
	case down:
	y = y+step>(BACKGROUND_HEIGHT-SCREEN_HEIGHT)?BACKGROUND_HEIGHT-SCREEN_HEIGHT:y+step;
	break;
	case down_left:
	y = y+step*diag_mvt>(BACKGROUND_HEIGHT-SCREEN_HEIGHT)?BACKGROUND_HEIGHT-SCREEN_HEIGHT:y+ step*diag_mvt;
	x = x-step*diag_mvt<0?0:x-step*diag_mvt;
	break;
	case left:
	x = x-step<0?0:x-step;
	break;
	case left_up:
	y = y-step*diag_mvt<0?0:y-step*diag_mvt;
	x = x-step*diag_mvt<0?0:x-step*diag_mvt;
	break;
	}
		bg->offsetX=x;
		bg->offsetY=y;
}
int is_blocked(SDL_Surface* collision_map,int x, int y){
    if(!collision_map) return 0;
    int w = SCREEN_WIDTH;
    int h = SCREEN_HEIGHT;
    int points[8][2] = {
        {x, y},
        {x + w/2, y},
        {x + w, y},
        {x, y + h/2},
        {x + w, y + h/2},
        {x, y + h},
        {x + w/2, y + h},
        {x + w, y + h}
    };
    SDL_LockSurface(collision_map);
    for(int i = 0; i < 8; i++){
        int px = points[i][0];
        int py = points[i][1];
        if(px < 0 || py < 0 || px >= collision_map->w || py >= collision_map->h){
            SDL_UnlockSurface(collision_map);
            return 1;
        }
        SDL_Color c = get_pixel(collision_map, px, py);
        if(c.r == 0 && c.g == 0 && c.b == 0){
            SDL_UnlockSurface(collision_map);
            return 1;
        }
    }

    SDL_UnlockSurface(collision_map);
    return 0;
}
SDL_Color get_pixel(SDL_Surface *surface, int x, int y){
	SDL_Color c ={255,255,255,255};
	if(x < 0 || y < 0 || x >= surface->w || y >= surface->h)
    		return c;
	Uint8 *pPosition = (Uint8*)surface->pixels+(y*surface->pitch)+(x*surface->format->BytesPerPixel);
	Uint32 pixel_value=0;
	memcpy(&pixel_value,pPosition,surface->format->BytesPerPixel);
	SDL_GetRGB(pixel_value,surface->format,&c.r,&c.g,&c.b);
	return c;
}
void interact(int x,int y,interactable * objects){
for( interactable* it = objects; it<objects+OBJ_COUNT;it++){
	if(!it->is_interactedwith){
		int dist = sqrt(pow(x - it->position.x, 2) + pow(y - it->position.y, 2));
		if(dist<INTERACT_RAD){
			it->is_interactedwith = 1;
			}
		}
	}
}
void manage_time(game_clock *t,SDL_Renderer *renderer,TTF_Font *font){
	t->elapsed = SDL_GetTicks()/1000-t->start;
	int h = t->elapsed / 3600;
    	int m = (t->elapsed % 3600) / 60;
    	int s = t->elapsed % 60;
    	char buff[10];
    	sprintf(buff,"%02d:%02d:%02d",h,m,s);
	if(strcmp(buff,t->disp_clock)!=0){
		strcpy(t->disp_clock,buff);
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, t->disp_clock, WHITE);
		if(!textSurface){
			printf("error creating surface @manage_time\n");
			return;
		}
		if(t->texture)
		SDL_DestroyTexture(t->texture);
		t->texture = SDL_CreateTextureFromSurface(renderer,textSurface);
		if(!t->texture){
			printf("error creating texture @manage_time\n");
			return;
		}
		SDL_FreeSurface(textSurface);
	}
}
//clear renderer and render the frame only once per frame and not on every pass
void init_tutorial(SDL_Renderer *renderer,tutorial_piece *tutorialArr, int tut_count){
	char path[30];
	for (tutorial_piece *pT = tutorialArr;pT<tutorialArr+tut_count;pT++)
	{
		sprintf(path,"assets/images/tutorial%d.png",pT-tutorialArr);
		SDL_Surface *s = IMG_Load(path);
		if(!s){
			printf("error creating surface @init_tutorial\n");
			return;
		}	
		
		pT->position = (SDL_Rect){50,30,s->w,s->h};
		pT->tut_display = SDL_CreateTextureFromSurface(renderer,s);
		SDL_FreeSurface(s);
	}
}
void display_tutorial(SDL_Renderer *renderer,tutorial_piece tut){
	SDL_RenderCopy(renderer,tut.tut_display,NULL,&tut.position);
}
void save_score(ScoreInfo s,char* time, char* score_file){
	FILE *f = fopen(score_file,"a");
	if(!f){
	printf("failed to open score file @save_scores\n");
	return;
	}
	fprintf(f,"%d|%s|%s\n",s.score,s.name,time);
	fclose(f);
	char sort_cmd[100];
	sprintf(sort_cmd,"sort -o %s %s",score_file,score_file); 
	if(system(sort_cmd)==-1){
		printf("failed to sort @save_scores\n");
		return;
	}
}
void input_name(SDL_Renderer *renderer,SDL_Event *e,char name_buff[],int len,TTF_Font *font){
	display_name(renderer,font, name_buff);
	if(e->type == SDL_TEXTINPUT){
		snprintf(name_buff +strlen(name_buff),
			len-strlen(name_buff),
			"%s",e->text.text);
	}//limiting input length to string size coz abdullah ben abdelaziz karawita might break the name thing
	else if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE && strlen(name_buff) >0)		
		{
			name_buff[strlen(name_buff)-1]='\0';
		}
	else if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN){
		ScoreInfo s;
		strcpy(s.name,name_buff);
		s.score = clk.elapsed; //will determine score calculation
		save_score(s,clk.disp_clock,"saves/highscores.txt");
		SDL_StopTextInput();
		strcpy(name_buff,"");
		current_menu = MENU_HIGHSCORES;
	}		
}
void display_name(SDL_Renderer *renderer, TTF_Font *font, char *name) {
    SDL_Color color = {255, 255, 255, 255};
    char display_text[20];
    snprintf(display_text, sizeof(display_text), "Name: %s", name);

    SDL_Surface *surface = TTF_RenderText_Blended(font, display_text, color);
    if (!surface) {
        printf("Error creating text surface @display_name\n");
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {(SCREEN_WIDTH-surface->w)/2, 50, surface->w, surface->h};
    SDL_FreeSurface(surface);

    if (!texture) {
        printf("Error creating text texture @display_name\n");
        return;
    }
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}
void clean_up_bg(Background *bg,tutorial_piece *tut,game_clock *clk,int player_count){
	if(bg){
		for(Background *pB =bg;pB<bg+player_count;pB++){
			for(int i=0;i<OBJ_COUNT;i++){
			if(pB->objects[i].base)
				SDL_DestroyTexture(pB->objects[i].base);
			if(pB->objects[i].used)
				SDL_DestroyTexture(pB->objects[i].used);
			}
		if(bg && bg[0].texture){
			SDL_DestroyTexture(bg[0].texture);
			}
		}
	}
	if(tut){
		for(tutorial_piece* pT=tut;pT<tut+3;pT++){
			SDL_DestroyTexture(pT->tut_display);
		}
	}
	if(clk){
		SDL_DestroyTexture(clk->texture);
	}
	free(bg);
}
