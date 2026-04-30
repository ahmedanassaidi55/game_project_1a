mini_map.h
"#ifndef MINI_MAP_H
#define MINI_MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Structure pour représenter une minimap
typedef struct {
    SDL_Texture *backgroundTexture;  // Texture de fond de la minimap
    SDL_Texture *playerTexture;      // Texture du joueur sur la minimap
    SDL_Texture *enemyTexture;       // Texture des ennemis sur la minimap
    SDL_Rect minimapPosition;        // Position et taille de la minimap à l'écran
} Minimap;

// Fonctions associées à la minimap
void initMiniMap(Minimap *m, SDL_Renderer *renderer);
SDL_Rect calculerPosSurMap(SDL_Rect posEntite, Minimap m, SDL_Rect camera);
void afficherMiniMap(Minimap m, SDL_Renderer *renderer, SDL_Rect joueur, 
                     SDL_Rect ennemis[], int nbEnnemis, SDL_Rect camera);
void LibererMinimap(Minimap *m);

// Section pour la gestion des collisions
typedef struct {
    SDL_Surface *mask;   // Surface de masque pour les collisions
    SDL_Rect camera;     // Position de la caméra
} Background;

SDL_Color GetPixel(SDL_Surface *pSurface, int x, int y);
int collisionPerfectPixel(Background b, SDL_Rect posJoueur);

#endif // MINI_MAP_H"
