#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include <SDL_image.h>

#define MAX_PROJECTILES 20

/*================ STRUCTURES =================*/

typedef struct {
    float x, y;
    float vx, vy;
    int active;
    SDL_Rect rect;
} Projectile;

typedef struct {
    float x, y;
} Player;

typedef struct {
    float x, y;
    float speed;
    int health;

    SDL_Texture *sprite;

    SDL_Rect posScreen;
    SDL_Rect posSprite;

    int frame;

    Projectile projectiles[MAX_PROJECTILES];

} Enemy;

/*================ FONCTIONS =================*/

void initEnemy(Enemy *e, SDL_Renderer *renderer);
void pathfindingSimple(Enemy *e, Player p);
void animateEnemy(Enemy *e);
void shootProjectile(Enemy *e, Player p);
void updateProjectiles(Enemy *e);
int checkCollision(SDL_Rect a, SDL_Rect b);
void renderEnemy(Enemy *e, SDL_Renderer *renderer);

#endif
