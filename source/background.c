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
		if(fscanf(f,"%d|%d|%d|%d|%d|%d",&bg->objects[i].position.x,
		&bg->objects[i].position.y,
		&bg->objects[i].position.w,
		&bg->objects[i].position.h,
		&bg->objects[i].tilex,
		&bg->objects[i].tiley
		)!=1){
			printf("failed to initialize object: %d .\n",i);
			break; 
		}
		
		bg->objects[i].is_interactedwith = tilemap[bg->objects[i].tilex][bg->objects[i].tiley] == 0;
		
		
	}
	clk.start = SDL_GetTicks()/1000;
}
int tilemap[TILE_HEIGHT][TILE_WIDTH];
void load_tilemap(char *tm_file){
	FILE *f = fopen(tm_file,"r");
	if(f == NULL){
		printf("invalid path.\n");
		return;
	}
	for(int i =0; i< TILE_HEIGHT;i++){
		for(int j=0; j<TILE_WIDTH;j++){
			if(fscanf(f,"%d",&tilemap[i][j])!=1)
			break;	
		}
	}
	fclose(f);
}
void save_curr_tilemap(char *tm_file){
	FILE *f = fopen(tm_file,"w");
	if(f == NULL){
		printf("couldn't open save file");
		return;
	}
	for(int i =0; i< TILE_HEIGHT;i++){
		for(int j=0; j<TILE_WIDTH;j++){
			fprintf(f,"%d ",tilemap[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
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
	int diag_mvt = 0.7071; // to keep constant speed in diagonal directions
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
	bg->offsetX = bg->offsetX+step*diag_mvt<(BACKGROUND_WIDTH-SCREEN_WIDTH)?BACKGROUND_WIDTH-SCREEN_WIDTH:bg->offsetX+step*diag_mvt;
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
//already implemented in helper.c, visit helper.c @455 to 533 for detailed implementation, we were already evaluated for this menu so idt you guys care much
void display_highscores(char * score_file,SDL_Renderer *renderer){
	
}
