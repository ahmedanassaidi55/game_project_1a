#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include <SDL_image.h>

#define MAX_PROJECTILES 20
/* Nombre maximum de projectiles que l’ennemi peut gérer en même temps */

/*================ STRUCTURES =================*/

/* Structure représentant un projectile (balle de l’ennemi) */
typedef struct {
    float x, y;       // position du projectile
    float vx, vy;     // vitesse (direction + déplacement)
    int active;       // 1 = actif, 0 = inactif (non utilisé)
    SDL_Rect rect;    // rectangle pour affichage et collision
} Projectile;

/* Structure représentant le joueur */
typedef struct {
    float x, y;      // position du joueur dans le monde
} Player;

/* Structure représentant l’ennemi */
typedef struct {
    float x, y;          // position de l’ennemi
    float speed;         // vitesse de déplacement
    int health;          // points de vie de l’ennemi

    SDL_Texture *sprite; // image (texture SDL) de l’ennemi

    SDL_Rect posScreen;  // position affichée à l’écran
    SDL_Rect posSprite;  // découpage du sprite pour animation

    int frame;           // frame actuelle de l’animation

    Projectile projectiles[MAX_PROJECTILES]; 
    /* tableau de projectiles tirés par l’ennemi */

} Enemy;

/*================ FONCTIONS =================*/

/* Initialise l’ennemi :
   - position de départ
   - chargement de l’image
   - initialisation des projectiles */
void initEnemy(Enemy *e, SDL_Renderer *renderer);

/* IA simple :
   L’ennemi suit automatiquement la position du joueur */
void pathfindingSimple(Enemy *e, Player p);

/* Animation :
   Change les frames du sprite pour créer une animation */
void animateEnemy(Enemy *e);

/* Tir :
   Crée un projectile dirigé vers le joueur */
void shootProjectile(Enemy *e, Player p);

/* Mise à jour des projectiles :
   - déplacement
   - désactivation si hors écran */
void updateProjectiles(Enemy *e);

/* Détection de collision entre deux rectangles SDL */
int checkCollision(SDL_Rect a, SDL_Rect b);

/* Affichage :
   - dessine l’ennemi
   - dessine les projectiles */
void renderEnemy(Enemy *e, SDL_Renderer *renderer);

#endif /* ENNEMI_H */
