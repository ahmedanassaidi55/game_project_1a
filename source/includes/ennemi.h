#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include <SDL_image.h>

#define MAX_PROJECTILES 20
#define PROJECTILE_SPEED 1.4f
#define ENEMY_WORLD_W 600
#define ENEMY_WORLD_H 400


/*================ STRUCTURES =================*/

/* Structure representant un projectile (balle de l’ennemi) */
typedef struct {
    float x, y;       // position du projectile
    float vx, vy;     // vitesse 
    int active;       //
    SDL_Rect rect;    // rectangle pour affichage
} Projectile;

/* Structure représentant le joueur */
typedef struct {
    float x, y;      // position du joueur dans le game
} EnemyTarget;

/* Structure représentant l’ennemi */
typedef struct {
    float x, y;          // position 
    float speed;         // vitesse
    int health;          //  vie 

    SDL_Texture *sprite; // image 

    SDL_Rect posScreen;  // position affichee
    SDL_Rect posSprite;  // decoupage du sprite pour animation

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
void pathfindingSimple(Enemy *e, EnemyTarget p);

/* Animation :
   Change les frames du sprite pour créer une animation */
void animateEnemy(Enemy *e);

void shootProjectile(Enemy *e, EnemyTarget p);
void updateProjectiles(Enemy *e);


/* Détection de collision entre deux rectangles SDL */
int checkCollision(SDL_Rect a, SDL_Rect b);

/* Affichage :
   - dessine l’ennemi
   - dessine les projectiles */
void renderEnemy(Enemy *e, SDL_Renderer *renderer);

#endif /* ENNEMI_H */
