#ifndef HEAD_H
#define HEAD_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Texture *map_bg;      // Optional background texture
    SDL_Texture *player_icon; // Optional player icon texture
    SDL_Texture *enemy_icon;  // Optional enemy icon texture
    SDL_Rect pos_minimap;     // Position of minimap on screen
    SDL_Rect pos_player;      // Player position relative to minimap
    SDL_Rect pos_enemy;       // Enemy position relative to minimap
    float scale_x;            // Horizontal scaling factor
    float scale_y;            // Vertical scaling factor
    int world_w;              // World width in pixels
    int world_h;              // World height in pixels
} MiniMap;

// Prototypes based on your Cahier des Charges
bool initMiniMap(MiniMap *m, SDL_Renderer *renderer, int world_w, int world_h);
void majMinimap(MiniMap *m, SDL_Rect pos_joueur_monde, SDL_Rect pos_enemy_monde);
void afficherMinimap(MiniMap m, SDL_Renderer *renderer);
void libererMinimap(MiniMap *m);

#endif
