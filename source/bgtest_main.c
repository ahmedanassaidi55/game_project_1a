#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "background.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG init failed: %s\n", IMG_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF init failed: %s\n", TTF_GetError());
        return 1;
    }
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    printf("SDL_Init: %s\n", SDL_GetError());

if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    printf("Mix_OpenAudio: %s\n", Mix_GetError());

    SDL_Window *window = SDL_CreateWindow(
        "Full System Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("../assets/dogicapixel.ttf", 20);
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
        return 1;
    }
    Background *bg = init_background(renderer, 1);
    if (!bg) {
        printf("Background init failed\n");
        return 1;
    }
    SDL_Surface *collision_map = IMG_Load("../assets/images/collmapbg.png");
    if (!collision_map) {
        printf("Collision map load failed: %s\n", IMG_GetError());
        return 1;
    }
    Mix_Music * game_music = Mix_LoadMUS("../assets/audio/level1.mp3");
    if(!game_music){
    	printf("failed to load game music: %s\n",Mix_GetError());
    	return 1;
    }
    Mix_Music * save_score_music = Mix_LoadMUS("../assets/audio/name_input_music.mp3");
    if(!save_score_music){
    	printf("failed to load game music\n");
    	return 1;
    }
    Mix_PlayMusic(game_music,-1);
    int typing = 0;
    int running = 1;
    SDL_Event e;
    int step = 12;
    char name[20]="";
    while (running) {

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = 0;
            if(typing)
            typing = input_name(renderer,&e,name,20,font);
            if (e.type == SDL_KEYDOWN) {

                int newX = bg[0].offsetX;
                int newY = bg[0].offsetY;
		if (e.type == SDL_TEXTINPUT) {
			printf("TEXT EVENT: %s\n", e.text.text);
		}
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;

                    case SDLK_UP:
                        newY -= step;
                        break;

                    case SDLK_DOWN:
                        newY += step;
                        break;

                    case SDLK_LEFT:
                        newX -= step;
                        break;

                    case SDLK_RIGHT:
                        newX += step;
                        break;
                    case SDLK_s:
                    	typing = 1;
                    	Mix_HaltMusic();
                    	Mix_PlayMusic(save_score_music,-1);
                    	break;
                    	
                }
                if (!is_blocked(collision_map, newX, newY)) {
                    bg[0].offsetX = newX;
                    bg[0].offsetY = newY;
                }
            }
        }
        SDL_RenderClear(renderer);

        display_background(bg, renderer, 1);
        if(!typing){
            manage_time(&clk, renderer, font);
            if (clk.texture) {
        	SDL_Rect clock_pos = {10, 10, 120, 30};
        	SDL_RenderCopy(renderer, clk.texture, NULL, &clock_pos);
            }
        }
        else{
        	display_name(renderer,font,name);
        }

        SDL_RenderPresent(renderer);
    }

    // ---------------- CLEANUP ----------------
    SDL_FreeSurface(collision_map);
    TTF_CloseFont(font);

    clean_up_bg(bg, NULL, &clk, 1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
