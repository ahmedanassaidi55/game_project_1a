#ifndef MINIMAP_GAME_H
#define MINIMAP_GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Texture *map_bg;
    SDL_Texture *player_icon;
    SDL_Texture *enemy_icon;
    SDL_Rect pos_minimap;
    SDL_Rect pos_player;
    SDL_Rect pos_enemy;
    float scale_x;
    float scale_y;
    int world_w;
    int world_h;
} MiniMapGame;

bool initMiniMapGame(MiniMapGame *m, SDL_Renderer *renderer, int world_w, int world_h);
void updateMiniMapGame(MiniMapGame *m, SDL_Rect pos_joueur_monde, SDL_Rect pos_enemy_monde);
void renderMiniMapGame(MiniMapGame *m, SDL_Renderer *renderer);
void freeMiniMapGame(MiniMapGame *m);

#endif
