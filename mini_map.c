mini_map.c
"#include "mini_map.h"
#include <stdio.h>

/* ============================================================
 *  MINIMAP - Dimensions du monde reeel (a adapter a ton projet)
 *  Ces valeurs representent la taille totale de ta carte en pixels
 * ============================================================ */
#define WORLD_WIDTH  3200
#define WORLD_HEIGHT 2000

/* Taille du point represente sur la minimap */
#define DOT_SIZE 6

/* ============================================================
 *  INITIALISATION
 * ============================================================ */
void initMiniMap(Minimap *m, SDL_Renderer *renderer) {
    m->backgroundTexture = IMG_LoadTexture(renderer, "mini_map.png");
    m->playerTexture     = IMG_LoadTexture(renderer, "point_jaune.png");
    m->enemyTexture      = IMG_LoadTexture(renderer, "point_rouge.png");

    if (!m->backgroundTexture || !m->playerTexture || !m->enemyTexture) {
        printf("Erreur textures Minimap: %s\n", IMG_GetError());
    }

    /* Position et taille de la minimap a l'ecran */
    m->minimapPosition = (SDL_Rect){620, 20, 160, 100};
}

/* ============================================================
 *  CALCUL DE LA POSITION D'UNE ENTITE SUR LA MINIMAP
 *
 *  posEntite : position de l'entite a l'ecran (coords relatives camera)
 *  camera    : decalage de la camera (origine du monde visible)
 *
 *  Position absolue dans le monde = posEntite + camera
 *  Position sur minimap = (posAbsolue / dimensionMonde) * dimensionMinimap
 * ============================================================ */
SDL_Rect calculerPosSurMap(SDL_Rect posEntite, Minimap m, SDL_Rect camera) {
    SDL_Rect posMap;
    posMap.w = DOT_SIZE;
    posMap.h = DOT_SIZE;

    /* 1. Coordonnees absolues de l'entite dans le monde */
    int x_monde = posEntite.x + camera.x + (posEntite.w / 2); /* centre horizontal */
    int y_monde = posEntite.y + camera.y + (posEntite.h / 2); /* centre vertical   */

    /* 2. Projection proportionnelle sur la minimap */
    int x_map = (int)((float)x_monde / WORLD_WIDTH  * m.minimapPosition.w);
    int y_map = (int)((float)y_monde / WORLD_HEIGHT * m.minimapPosition.h);

    /* 3. Decalage par rapport a l'origine de la minimap a l'ecran */
    posMap.x = m.minimapPosition.x + x_map - (DOT_SIZE / 2);
    posMap.y = m.minimapPosition.y + y_map - (DOT_SIZE / 2);

    /* 4. Clampage : le point reste toujours dans les bornes de la minimap */
    if (posMap.x < m.minimapPosition.x)
        posMap.x = m.minimapPosition.x;
    if (posMap.y < m.minimapPosition.y)
        posMap.y = m.minimapPosition.y;
    if (posMap.x + DOT_SIZE > m.minimapPosition.x + m.minimapPosition.w)
        posMap.x = m.minimapPosition.x + m.minimapPosition.w - DOT_SIZE;
    if (posMap.y + DOT_SIZE > m.minimapPosition.y + m.minimapPosition.h)
        posMap.y = m.minimapPosition.y + m.minimapPosition.h - DOT_SIZE;

    return posMap;
}

/* ============================================================
 *  AFFICHAGE COMPLET DE LA MINIMAP
 * ============================================================ */
void afficherMiniMap(Minimap m, SDL_Renderer *renderer,
                     SDL_Rect joueur,
                     SDL_Rect ennemis[], int nbEnnemis,
                     SDL_Rect camera) {

    /* Fond de la minimap */
    SDL_RenderCopy(renderer, m.backgroundTexture, NULL, &m.minimapPosition);

    /* Clip optionnel : empeche tout rendu hors de la minimap */
    SDL_RenderSetClipRect(renderer, &m.minimapPosition);

    /* Points rouges : ennemis */
    for (int i = 0; i < nbEnnemis; i++) {
        SDL_Rect pE = calculerPosSurMap(ennemis[i], m, camera);
        SDL_RenderCopy(renderer, m.enemyTexture, NULL, &pE);
    }

    /* Point jaune : joueur */
    SDL_Rect pJ = calculerPosSurMap(joueur, m, camera);
    SDL_RenderCopy(renderer, m.playerTexture, NULL, &pJ);

    /* Desactive le clip apres l'affichage */
    SDL_RenderSetClipRect(renderer, NULL);
}

/* ============================================================
 *  LIBERATION DES RESSOURCES
 * ============================================================ */
void LibererMinimap(Minimap *m) {
    if (m->backgroundTexture) SDL_DestroyTexture(m->backgroundTexture);
    if (m->playerTexture)     SDL_DestroyTexture(m->playerTexture);
    if (m->enemyTexture)      SDL_DestroyTexture(m->enemyTexture);
    m->backgroundTexture = NULL;
    m->playerTexture     = NULL;
    m->enemyTexture      = NULL;
}

/* ============================================================
 *  SECTION COLLISIONS
 * ============================================================ */
SDL_Color GetPixel(SDL_Surface *pSurface, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    if (pSurface == NULL) return color;

    if (x >= 0 && x < pSurface->w && y >= 0 && y < pSurface->h) {
        Uint32 *pixels = (Uint32 *)pSurface->pixels;
        SDL_GetRGB(pixels[(y * pSurface->w) + x],
                   pSurface->format,
                   &color.r, &color.g, &color.b);
    }
    return color;
}

int collisionPerfectPixel(Background b, SDL_Rect posJoueur) {
    /* Point de detection : pied du joueur, centre horizontal */
    int x_abs = posJoueur.x + b.camera.x + (posJoueur.w / 2);
    int y_abs = posJoueur.y + b.camera.y +  posJoueur.h;

    SDL_Color color = GetPixel(b.mask, x_abs, y_abs);
    return (color.r == 0 && color.g == 0 && color.b == 0);
}"
