/* Enemy آ· H (FINAL - ADAPTأ‰ AU SPRITESHEET) */

#ifndef ENEMY_H
#define ENEMY_H
 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
 
/* ============================================================
   CONSTANTES
   ============================================================ */
#define WINDOW_W        800
#define WINDOW_H        600
 
/* ============================================================
   SPRITESHEET  (image 1024أ—1536 â†’ grille 4أ—6 de 256أ—256)
   ============================================================ */
#define SPRITE_COLS     4
#define SPRITE_ROWS     6   /* 6 lignes physiques dans l'image */

/* BUG FIX : أ©tait 64أ—64, la vraie taille est 256أ—256 */
#define FRAME_W         256
#define FRAME_H         256

#define ANIM_SPEED      100

/* ============================================================
   LIGNES (animations) â€” indices de ligne dans le spritesheet
   ============================================================ */
#define ROW_WALK_RIGHT  0
#define ROW_WALK_LEFT   1
#define ROW_IDLE        2
#define ROW_ATTACK      3
#define ROW_DEATH       4

/* ============================================================
   FRAMES PAR ANIMATION
   ============================================================ */
#define FRAMES_WALK     4
#define FRAMES_IDLE     4
#define FRAMES_ATTACK   4   /* BUG FIX : أ©tait 3, la ligne a bien 4 frames */
#define FRAMES_DEATH    4

/* ============================================================
   COMPORTEMENT
   ============================================================ */
#define ENEMY_SPEED_LVL1  2
#define ENEMY_SPEED_LVL2  4
#define ENEMY_HP_MAX      3
#define ATTACK_RANGE      80
#define PATROL_DIST       200
 
#define TRAJ_LINEAR       0
#define TRAJ_DIAGONAL     1
 
/* ============================================================
   أ‰TATS
   ============================================================ */
typedef enum {
    ALIVE   = 0,
    HURT    = 1,
    DEAD    = 2
} HealthState;
 
typedef enum {
    STATE_PATROL  = 0,
    STATE_CHASE   = 1,
    STATE_ATTACK  = 2,
    STATE_IDLE    = 3,
    STATE_DEAD    = 4
} EnemyState;

/* ============================================================
   JOUEUR
   ============================================================ */
typedef struct {
    float x, y;
    int   w, h;
    int   hp;
} Player;

/* ============================================================
   ENTITأ‰ SECONDAIRE
   ============================================================ */
typedef struct {
    float  angle;
    float  radius;
    float  speed;
    int    active;
    SDL_Rect dst;
} SecondaryEntity;

/* ============================================================
   ENNEMI
   ============================================================ */
typedef struct {
    float      x, y;
    float      vx, vy;
    int        w, h;
    int        dir;

    int        traj;
    float      originX;
    float      originY;
    int        speed;

    /* Animation */
    int        currentFrame;
    int        currentRow;
    int        maxFrames;
    int        animTimer;

    int        isAttacking;

    int        hp;
    HealthState healthState;
    int        hurtTimer;

    EnemyState state;
    int        level;

    SDL_Rect   bbox;

    SecondaryEntity secondary;

    SDL_Texture *texture;

} Enemy;

/* ============================================================
   MINI MAP
   ============================================================ */
typedef struct {
    SDL_Rect   area;
    float      scaleX;
    float      scaleY;
} MiniMap;

/* ============================================================
   PROTOTYPES
   ============================================================ */
Enemy*  enemy_create(SDL_Renderer *ren, const char *spritePath,
                     float startX, float startY, int level);

void    enemy_destroy(Enemy *e);

void    enemy_update(Enemy *e, Player *p, Uint32 deltaMs);

int     enemy_check_collision_player(Enemy *e, Player *p);
void    enemy_apply_collision(Enemy *e, Player *p);

void    enemy_take_damage(Enemy *e, int dmg);
void    enemy_draw_healthbar(Enemy *e, SDL_Renderer *ren);

void    enemy_draw(Enemy *e, SDL_Renderer *ren);
void    enemy_draw_secondary(Enemy *e, SDL_Renderer *ren);

MiniMap minimap_create(void);
void    minimap_draw(MiniMap *mm, Enemy *e, Player *p,
                     SDL_Renderer *ren);

#endif
