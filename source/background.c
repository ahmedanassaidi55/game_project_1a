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
}
int tilemap[TILE_HEIGHT][TILE_WIDTH] =0;
void load_tilemap(char *tm_file){
	FILE *f = fopen(tm_file,"r");
	if(f == NULL){
		printf("invalid path.\n");
		return;
	}
	for(int i =0; i< TILE_HEIGHT;i++){
		for(int j=0; k<TILE_WIDTH;j++){
			if(fscanf(f,"%d",&tilemap[i][j]!=1))
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
		for(int j=0; k<TILE_WIDTH;j++){
			fprintf(f,"%d ",tilemap[i][j])
		}
		fprintf(fp,"\n");
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
		SDL_RenderCopy(renderer, bg->objects[i].used,NULL,&(SDL_Rect){ScreenX,ScreenY,bg->objects[i].position.w,bg->objects[i].position.h});
	}else{
	SDL_RenderCopy(renderer, bg->objects[i].base,NULL,&(SDL_Rect){ScreenX,ScreenY,bg->objects[i].position.w,bg->objects[i].position.h});}
	}
}

