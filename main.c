main.c
"#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "mini_map.h"

int main(int argc, char *argv[]) {
    // 1. Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur init SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Test Minimap SDL2", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 2. Initialisation de la Minimap
    Minimap myMinimap;
    initMiniMap(&myMinimap, renderer);

    // 3. Setup des entités (Joueur, Ennemis, Caméra)
    // Position du joueur dans le monde (au centre au début)
    SDL_Rect joueur = {100, 100, 50, 80}; 
    
    // Un tableau d'ennemis pour l'exemple
    SDL_Rect ennemis[1];
    ennemis[0] = (SDL_Rect){1500, 1000, 40, 40}; // Un ennemi loin dans le monde

    // La caméra (on simule un décalage si nécessaire)
    SDL_Rect camera = {0, 0, 800, 600};

    // 4. Boucle de jeu
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Gestion des déplacements simples (Z,Q,S,D ou Flèches)
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_UP])    joueur.y -= 5;
        if (state[SDL_SCANCODE_DOWN])  joueur.y += 5;
        if (state[SDL_SCANCODE_LEFT])  joueur.x -= 5;
        if (state[SDL_SCANCODE_RIGHT]) joueur.x += 5;

        // --- RENDU ---
        // Effacer l'écran (fond bleu pour le ciel/monde)
        SDL_SetRenderDrawColor(renderer, 50, 100, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner le joueur dans le "monde" (en blanc)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &joueur);

        // Afficher la minimap par-dessus
        // Note : On passe le joueur, le tableau d'ennemis et la caméra
        afficherMiniMap(myMinimap, renderer, joueur, ennemis, 1, camera);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // 5. Nettoyage
    LibererMinimap(&myMinimap);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}"
