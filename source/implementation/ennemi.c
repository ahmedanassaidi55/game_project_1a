#include "ennemi.h"
#include <stdio.h>

/*================ INIT =================*/
void initEnemy(Enemy *e, SDL_Renderer *renderer)
{
    e->x = 400;
    e->y = 200;
    e->speed = 1.5;
    e->health = 100;

    SDL_Surface *surf = IMG_Load("assets/images/IMG_4446.jpg");
    if(!surf){
        printf("Erreur chargement assets/images/IMG_4446.jpg\n");
        return;
    }

    e->sprite = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    e->posScreen.w = 64;
    e->posScreen.h = 64;

    e->posSprite.x = 0;
    e->posSprite.y = 0;
    e->posSprite.w = 64;
    e->posSprite.h = 64;

    e->frame = 0;

    for(int i=0;i<MAX_PROJECTILES;i++)
        e->projectiles[i].active = 0;
}

/*================ IA =================*/
void pathfindingSimple(Enemy *e, EnemyTarget p)
{
    if(p.x > e->x) e->x += e->speed;
    else e->x -= e->speed;

    if(p.y > e->y) e->y += e->speed;
    else e->y -= e->speed;

    e->posScreen.x = (int)e->x;
    e->posScreen.y = (int)e->y;
}

/*================ ANIMATION =================*/
void animateEnemy(Enemy *e)
{
    e->frame++;
    if(e->frame >= 4)
        e->frame = 0;

    e->posSprite.x = e->frame * e->posSprite.w;
}

/*================ TIR =================*/
void shootProjectile(Enemy *e, EnemyTarget p)
{
    for(int i=0;i<MAX_PROJECTILES;i++)
    {
        if(!e->projectiles[i].active)
        {
            Projectile *proj = &e->projectiles[i];

            proj->x = e->x + (e->posScreen.w / 2.0f);
            proj->y = e->y + (e->posScreen.h / 2.0f);

            float dx = p.x - e->x;
            float dy = p.y - e->y;
            float dist = sqrt(dx*dx + dy*dy);
            if(dist == 0) dist = 1;

            proj->vx = (dx/dist) * PROJECTILE_SPEED;
            proj->vy = (dy/dist) * PROJECTILE_SPEED;

            proj->rect.w = 12;
            proj->rect.h = 12;
            proj->rect.x = (int)proj->x;
            proj->rect.y = (int)proj->y;

            proj->active = 1;
            break;
        }
    }
}

/*================ UPDATE PROJECTILES =================*/
void updateProjectiles(Enemy *e)
{
    for(int i=0;i<MAX_PROJECTILES;i++)
    {
        if(e->projectiles[i].active)
        {
            e->projectiles[i].x += e->projectiles[i].vx;
            e->projectiles[i].y += e->projectiles[i].vy;

            e->projectiles[i].rect.x = (int)e->projectiles[i].x;
            e->projectiles[i].rect.y = (int)e->projectiles[i].y;

            if(e->projectiles[i].x < 0 || e->projectiles[i].x > ENEMY_WORLD_W ||
               e->projectiles[i].y < 0 || e->projectiles[i].y > ENEMY_WORLD_H)
            {
                e->projectiles[i].active = 0;
            }
        }
    }
}

/*================ COLLISION =================*/
int checkCollision(SDL_Rect a, SDL_Rect b)
{
    if (a.x + a.w < b.x) return 0;
    if (a.x > b.x + b.w) return 0;
    if (a.y + a.h < b.y) return 0;
    if (a.y > b.y + b.h) return 0;
    return 1;
}

/*================ RENDER =================*/
void renderEnemy(Enemy *e, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, e->sprite, &e->posSprite, &e->posScreen);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for(int i=0;i<MAX_PROJECTILES;i++)
    {
        if(e->projectiles[i].active)
        {
            SDL_RenderFillRect(renderer, &e->projectiles[i].rect);
        }
    }
}
