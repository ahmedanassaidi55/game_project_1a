/**
 * @file joueur.h
 * @brief Lot 1 – Joueur : adapté à la spritesheet fournie (1316×810 px)
 *
 * CARTE DE LA SPRITESHEET (mesures réelles) :
 *
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ Ligne 0 (y=  0, h=200) : 7 frames  → MARCHE / COURSE   │
 *   │ Ligne 1 (y=200, h=200) : 7 frames  → IDLE              │
 *   │ Ligne 2 (y=400, h=200) : [0-2] ATTAQUE  + [3-6] MORT   │
 *   │ Ligne 3 (y=600, h=210) : [0-2] SUPER    + [3-5] MORT_SOL│
 *   └─────────────────────────────────────────────────────────┘
 *
 *  Largeur d'une frame standard  : 188 px
 *  Ligne 2 : ATTAQUE = 3 frames larges (~280px chacune),
 *            MORT    = 4 frames (188px chacune) à partir de x=840
 *  Ligne 3 : SUPER   = 3 frames très larges (tout l'espace gauche),
 *            MORT_SOL= 3 frames à partir de x=770
 */

#ifndef JOUEUR_H
#define JOUEUR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* =====================================================================
 *  DIMENSIONS SPRITESHEET (1316 × 810 px)
 * ===================================================================== */
#define SS_FRAME_W       188   /**< Largeur standard d'une frame (px)      */
#define SS_ROW0_Y          0   /**< Y ligne 0 – MARCHE/COURSE              */
#define SS_ROW0_H        200   /**< Hauteur ligne 0                        */
#define SS_ROW1_Y        200   /**< Y ligne 1 – IDLE                       */
#define SS_ROW1_H        200   /**< Hauteur ligne 1                        */
#define SS_ROW2_Y        400   /**< Y ligne 2 – ATTAQUE + MORT             */
#define SS_ROW2_H        200   /**< Hauteur ligne 2                        */
#define SS_ROW3_Y        600   /**< Y ligne 3 – SUPER + MORT_SOL           */
#define SS_ROW3_H        210   /**< Hauteur ligne 3                        */

/* Attaque (ligne 2) : 3 frames de largeur ~280 px */
#define SS_ATK_FRAME_W   280
#define SS_ATK_NB          3

/* Mort (ligne 2) : 4 frames de 188 px, départ x=840 */
#define SS_MORT_X        840
#define SS_MORT_NB         4

/* Super-attaque (ligne 3) : 3 frames de ~257 px chacune */
#define SS_SUPER_FRAME_W 257
#define SS_SUPER_NB        3

/* Mort-sol (ligne 3) : 3 frames de 188 px, départ x=770 */
#define SS_MORTsol_X     770
#define SS_MORTsol_NB      3

/* =====================================================================
 *  PHYSIQUE & GAMEPLAY
 * ===================================================================== */
#define GRAVITE          0.5f
#define FORCE_SAUT       12.0f
#define VITESSE_MARCHE   4
#define VITESSE_COURSE   8
#define NB_VIES_INIT     3
#define SCORE_INIT       0
#define SOL_Y            304   /**< Y du sol (en pixels, haut du sprite)   */

/* =====================================================================
 *  ÉTATS D'ANIMATION
 * ===================================================================== */
typedef enum {
    ETAT_IDLE          = 0,
    ETAT_MARCHE        = 1,
    ETAT_COURSE        = 2,
    ETAT_SAUT          = 3,
    ETAT_ATTAQUE       = 4,
    ETAT_SUPER_ATTAQUE = 5,
    ETAT_MORT          = 6,
    ETAT_MORT_SOL      = 7,
    NB_ETATS
} EtatJoueur;

/* =====================================================================
 *  DESCRIPTEUR D'ANIMATION
 * ===================================================================== */
typedef struct {
    int src_x;      /**< X pixel de départ dans la spritesheet  */
    int src_y;      /**< Y pixel de départ dans la spritesheet  */
    int frame_w;    /**< Largeur d'une frame (px)               */
    int frame_h;    /**< Hauteur d'une frame (px)               */
    int nb_frames;  /**< Nombre de frames de l'animation        */
    int delai_ms;   /**< Millisecondes entre deux frames        */
    int boucle;     /**< 1 = boucle, 0 = joue une seule fois   */
} AnimDesc;

/* =====================================================================
 *  CONFIGURATION TOUCHES
 * ===================================================================== */
typedef struct {
    SDL_Scancode gauche;
    SDL_Scancode droite;
    SDL_Scancode sauter;
    SDL_Scancode courir;
    SDL_Scancode attaquer;
    SDL_Scancode super_attaque;
} ConfigTouches;

/* =====================================================================
 *  STRUCTURE JOUEUR
 * ===================================================================== */
typedef struct {
    /* --- Position & physique --- */
    float x, y;
    float vx, vy;
    int   largeur, hauteur;
    int   auSol;

    /* --- Statistiques --- */
    int score;
    int vies;
    int actif;

    /* --- Animation --- */
    int        frame;
    Uint32     dernierTempsAnim;
    EtatJoueur etat;
    EtatJoueur etatPrecedent;
    int        regardeDroite;
    int        animFinie;

    /* --- Ressources SDL --- */
    SDL_Texture *texture;
    SDL_Texture *texScore;
    SDL_Texture *texVies;

    /* --- Config joueur --- */
    int           numJoueur;
    ConfigTouches touches;
    char          nom[32];
    int           costumeIdx;
} Personne;

/* =====================================================================
 *  PROTOTYPES – OBLIGATOIRES
 * ===================================================================== */
void initPerso        (Personne *p, SDL_Renderer *r,
                       const char *sprite, int numJoueur);
void afficherPerso    (Personne p,  SDL_Renderer *r);
void gestionViesScores(Personne *p, SDL_Renderer *r, TTF_Font *font);
void animerPerso      (Personne *p);
void deplacerPerso    (Personne *p, const Uint8 *keys);
void saut             (Personne *p);
void detruirePerso    (Personne *p);

/* =====================================================================
 *  PROTOTYPES – OPTIONNELS
 * ===================================================================== */
void initJoueur2   (Personne *p2, SDL_Renderer *r, const char *sprite);
int  sousMenuJoueur(Personne *p,  SDL_Renderer *r, TTF_Font *font);

#endif /* JOUEUR_H */
