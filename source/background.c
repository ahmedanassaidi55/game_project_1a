#include "background.h"
game_clock clk;
void init_background(Background *bg,SDL_Renderer *renderer){
	SDL_Surface *s = IMG_Load("assets/images/mapbg.png");
	if(!s){
		printf("invalid path.\n");
		return;
	}
	bg->position =(SDL_Rect){0,0,s->w,s->h};
	bg->texture = SDL_CreateTextureFromSurface(renderer,s);
	if(!bg->texture){
		printf("error while creating texture.\n");
		return;
	}
	SDL_FreeSurface(s);
	bg->offsetX = 0;
	bg->offsetY = 0;
	FILE *f = fopen("saves/elem_pos.txt","r");
	if (f == NULL){
		printf("couldn't find file");
		return;
	}
	for(int i = 0; i < OBJ_COUNT; i++){
		char elem_path[30];
		sprintf(elem_path,"assets/images/elem%d.png",i);
		bg->objects[i].base = IMG_LoadTexture(renderer,elem_path);
		sprintf(elem_path,"assets/images/elemu%d.png",i);
		bg->objects[i].used = IMG_LoadTexture(renderer,elem_path);
		if(fscanf(f,"%d|%d|%d|%d|%d",&bg->objects[i].position.x,
		&bg->objects[i].position.y,
		&bg->objects[i].position.w,
		&bg->objects[i].position.h,
		bg->objects[i].is_interactedwith
		)!=5){
			printf("failed to initialize object: %d .\n",i);
			break; 
		}		
	}
	clk.start = SDL_GetTicks()/1000;
}

void display_background(Background* bg,SDL_Renderer *renderer){
	SDL_RenderCopy(renderer,bg->texture,&(SDL_Rect){bg->offsetX,bg->offsetY,SCREEN_WIDTH,SCREEN_HEIGHT},NULL);
	for(int i = 0; i < OBJ_COUNT; i++){
	int screenX = bg->objects[i].position.x - bg->offsetX;
	int screenY = bg->objects[i].position.y - bg->offsetY;
	if(screenX + bg->objects[i].position.w <0 || screenX> SCREEN_WIDTH ||
	screenY + bg->objects[i].position.h <0 || screenY> SCREEN_HEIGHT){
		continue;
	}
	if(bg->objects[i].is_interactedwith){
		SDL_RenderCopy(renderer, bg->objects[i].used,NULL,&(SDL_Rect){screenX,screenY,bg->objects[i].position.w,bg->objects[i].position.h});
	}else{
	SDL_RenderCopy(renderer, bg->objects[i].base,NULL,&(SDL_Rect){screenX,screenY,bg->objects[i].position.w,bg->objects[i].position.h});}
	}
}
void scroll_background(Background *bg, enum direction d, int step){
	int diag_mvt = 0.7071f; // to keep constant speed in diagonal directions
	switch(d){
	case up:
	bg->offsetY = bg->offsetY-step<0?0:bg->offsetY-step;
	break;
	case up_right:
	bg->offsetY = bg->offsetY-step*diag_mvt<0?0:bg->offsetY-step*diag_mvt;
	bg->offsetX = bg->offsetX+step*diag_mvt>(BACKGROUND_WIDTH-SCREEN_WIDTH)?BACKGROUND_WIDTH-SCREEN_WIDTH:bg->offsetX+step*diag_mvt;
	break;
	case right:
	bg->offsetX = bg->offsetX+step>(BACKGROUND_WIDTH-SCREEN_WIDTH)?BACKGROUND_WIDTH-SCREEN_WIDTH:bg->offsetX+step;
	break;
	case down_right:
	bg->offsetY = bg->offsetY+step*diag_mvt>(BACKGROUND_HEIGHT-SCREEN_HEIGHT)?BACKGROUND_HEIGHT-SCREEN_HEIGHT:bg->offsetY+ step*diag_mvt;
	bg->offsetX = bg->offsetX + step*diag_mvt > (BACKGROUND_WIDTH -SCREEN_WIDTH)
	? (BACKGROUND_WIDTH - SCREEN_WIDTH) : bg->offsetX + step*diag_mvt;
	break;
	case down:
	bg->offsetY = bg->offsetY+step>(BACKGROUND_HEIGHT-SCREEN_HEIGHT)?BACKGROUND_HEIGHT-SCREEN_HEIGHT:bg->offsetY+step;
	break;
	case down_left:
	bg->offsetY = bg->offsetY+step*diag_mvt>(BACKGROUND_HEIGHT-SCREEN_HEIGHT)?BACKGROUND_HEIGHT-SCREEN_HEIGHT:bg->offsetY+ step*diag_mvt;
	bg->offsetX = bg->offsetX-step*diag_mvt<0?0:bg->offsetX-step*diag_mvt;
	break;
	case left:
	bg->offsetX = bg->offsetX-step<0?0:bg->offsetX-step;
	break;
	case left_up:
	bg->offsetY = bg->offsetY-step*diag_mvt<0?0:bg->offsetY-step*diag_mvt;
	bg->offsetX = bg->offsetX-step*diag_mvt<0?0:bg->offsetX-step*diag_mvt;
	break;
	}
}
int is_blocked(int futureX, int futureY){
	return tilemap[(int)futureX/TILE_WIDTH][(int)futureY/TILE_HEIGHT] == 1;
}
void interact(int x,int y,interactable * objects){
for( interactable* it = objects; it<objects+OBJ_COUNT;it++){
	if(!it->is_interactedwith){
		int dist = sqrt(pow(x - it->position.x, 2) + pow(y - it->position.y, 2));
		if(dist<INTERACT_RAD){
			it->is_interactedwith = 1;
			tilemap[it->tilex][it->tiley] = 0;
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
	if(strcmp(buff,t->disp_clock)){
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
		SDL_Surface *s = SDL_IMGLoad(path);
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
	display_name(renderer,font, name_buff)
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
