#include "enemy.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

/* ============================================================
   CREATE
   ============================================================ */
Enemy* enemy_create(SDL_Renderer *ren, const char *spritePath,
                    float startX, float startY, int level)
{
    Enemy *e = malloc(sizeof(Enemy));
    if (!e) return NULL;

    e->x = startX;
    e->y = startY;
    e->w = FRAME_W;
    e->h = FRAME_H;
    e->vx = 0;
    e->vy = 0;
    e->dir = 1;
    e->level = level;
    e->speed = (level == 1) ? ENEMY_SPEED_LVL1 : ENEMY_SPEED_LVL2;
    e->currentFrame = 0;
    e->currentRow   = ROW_IDLE;
    e->maxFrames    = FRAMES_IDLE;
    e->animTimer    = 0;
    e->isAttacking  = 0;
    e->hurtTimer    = 0;
    e->state        = STATE_PATROL;
    e->hp           = ENEMY_HP_MAX;
    e->healthState  = ALIVE;

    e->traj    = TRAJ_LINEAR;
    e->originX = startX;
    e->originY = startY;

    /* Secondary entity */
    e->secondary.active = 0;
    e->secondary.angle  = 0.0f;
    e->secondary.radius = 0.0f;
    e->secondary.speed  = 0.0f;

    SDL_Surface *surf = IMG_Load(spritePath);
    if (!surf) {
        fprintf(stderr, "IMG_Load error: %s\n", IMG_GetError());
        free(e);
        return NULL;
    }
    e->texture = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);

    if (!e->texture) {
        fprintf(stderr, "SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        free(e);
        return NULL;
    }

    return e;
}

/* ============================================================
   DESTROY
   ============================================================ */
void enemy_destroy(Enemy *e)
{
    if (!e) return;
    if (e->texture) SDL_DestroyTexture(e->texture);
    free(e);
}

/* ============================================================
   UPDATE
   ============================================================ */
void enemy_update(Enemy *e, Player *p, Uint32 deltaMs)
{
    if (e->state == STATE_DEAD) {
        /* Continuer l'animation de mort mais ne plus bouger */
        e->currentRow  = ROW_DEATH;
        e->maxFrames   = FRAMES_DEATH;
        e->animTimer  += deltaMs;
        if (e->animTimer >= ANIM_SPEED) {
            e->animTimer = 0;
            if (e->currentFrame < e->maxFrames - 1)
                e->currentFrame++;  /* Ne boucle pas, reste sur la derniĆØre frame */
        }
        return;
    }

    /* --- IA SIMPLE --- */
    float dx = p->x - e->x;
    if (fabs(dx) < ATTACK_RANGE) {
        e->state = STATE_ATTACK;
    } else {
        e->state = STATE_PATROL;
    }

    /* --- MOUVEMENT --- */
    if (e->state == STATE_PATROL) {
        e->vx  = (dx > 0) ? (float)e->speed : -(float)e->speed;
        e->x  += e->vx;
        e->dir = (e->vx > 0) ? 1 : -1;
    }

    /* Garder l'ennemi dans la fenĆŖtre */
    if (e->x < 0)                  e->x = 0;
    if (e->x > WINDOW_W - e->w)   e->x = (float)(WINDOW_W - e->w);

    /* --- ANIMATION --- */
    switch (e->state)
    {
        case STATE_PATROL:
            e->currentRow = (e->dir == 1) ? ROW_WALK_RIGHT : ROW_WALK_LEFT;
            e->maxFrames  = FRAMES_WALK;
            break;
        case STATE_ATTACK:
            e->currentRow = ROW_ATTACK;
            e->maxFrames  = FRAMES_ATTACK;
            break;
        case STATE_DEAD:
            e->currentRow = ROW_DEATH;
            e->maxFrames  = FRAMES_DEATH;
            break;
        default:
            e->currentRow = ROW_IDLE;
            e->maxFrames  = FRAMES_IDLE;
            break;
    }

    /* --- TIMER ANIMATION --- */
    e->animTimer += deltaMs;
    if (e->animTimer >= ANIM_SPEED) {
        e->animTimer   = 0;
        e->currentFrame = (e->currentFrame + 1) % e->maxFrames;
    }
}

/* ============================================================
   COLLISION
   ============================================================ */
int enemy_check_collision_player(Enemy *e, Player *p)
{
    SDL_Rect re = { (int)e->x, (int)e->y, e->w, e->h };
    SDL_Rect rp = { (int)p->x, (int)p->y, p->w, p->h };
    return SDL_HasIntersection(&re, &rp);
}

void enemy_apply_collision(Enemy *e, Player *p)
{
    /* Simple repulsion horizontale */
    if (e->x < p->x)
        e->x -= 2.0f;
    else
        e->x += 2.0f;
}

/* ============================================================
   DĆ‰GĆ‚TS / SANTĆ‰
   ============================================================ */
void enemy_take_damage(Enemy *e, int dmg)
{
    if (e->state == STATE_DEAD) return;
    e->hp -= dmg;
    if (e->hp <= 0) {
        e->hp          = 0;
        e->state       = STATE_DEAD;
        e->healthState = DEAD;
        e->currentFrame = 0;
    } else {
        e->healthState = HURT;
        e->hurtTimer   = 300; /* ms de clignotement */
    }
}

void enemy_draw_healthbar(Enemy *e, SDL_Renderer *ren)
{
    int barW = e->w;
    int barH = 6;
    int bx   = (int)e->x;
    int by   = (int)e->y - barH - 2;

    /* Fond rouge */
    SDL_SetRenderDrawColor(ren, 200, 0, 0, 255);
    SDL_Rect bg = { bx, by, barW, barH };
    SDL_RenderFillRect(ren, &bg);

    /* Vie restante en vert */
    int filled = (e->hp * barW) / ENEMY_HP_MAX;
    SDL_SetRenderDrawColor(ren, 0, 200, 0, 255);
    SDL_Rect fg = { bx, by, filled, barH };
    SDL_RenderFillRect(ren, &fg);
}

/* ============================================================
   DRAW
   ============================================================ */
void enemy_draw(Enemy *e, SDL_Renderer *ren)
{
    SDL_Rect src;
    src.x = e->currentFrame * FRAME_W;
    src.y = e->currentRow   * FRAME_H;
    src.w = FRAME_W;
    src.h = FRAME_H;

    SDL_Rect dst;
    dst.x = (int)e->x;
    dst.y = (int)e->y;
    dst.w = e->w;
    dst.h = e->h;

    SDL_RenderCopy(ren, e->texture, &src, &dst);

    enemy_draw_healthbar(e, ren);
}

void enemy_draw_secondary(Enemy *e, SDL_Renderer *ren)
{
    (void)e; (void)ren; /* Ć  implĆ©menter selon les besoins */
}

/* ============================================================
   MINI MAP
   ============================================================ */
MiniMap minimap_create(void)
{
    MiniMap m;
    m.area   = (SDL_Rect){ WINDOW_W - 110, 10, 100, 75 };
    m.scaleX = 100.0f / WINDOW_W;
    m.scaleY =  75.0f / WINDOW_H;
    return m;
}

/* BUG FIX : le corps se terminait par `})` au lieu de `}` */
void minimap_draw(MiniMap *mm, Enemy *e, Player *p, SDL_Renderer *ren)
{
    /* Fond */
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 200);
    SDL_RenderFillRect(ren, &mm->area);

    /* Contour */
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawRect(ren, &mm->area);

    /* Ennemi (rouge) */
    SDL_SetRenderDrawColor(ren, 220, 50, 50, 255);
    SDL_Rect re = {
        mm->area.x + (int)(e->x * mm->scaleX),
        mm->area.y + (int)(e->y * mm->scaleY),
        4, 4
    };
    SDL_RenderFillRect(ren, &re);

    /* Joueur (vert) */
    SDL_SetRenderDrawColor(ren, 50, 220, 50, 255);
    SDL_Rect rp = {
        mm->area.x + (int)(p->x * mm->scaleX),
        mm->area.y + (int)(p->y * mm->scaleY),
        4, 4
    };
    SDL_RenderFillRect(ren, &rp);
}
