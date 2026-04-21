/**
 * @file joueur.c
 * @brief Lot 1 – Joueur : implémentation adaptée à la spritesheet réelle
 *
 * CARTE DE LA SPRITESHEET (1316 × 810 px) :
 *
 *   px    0    188   376   564   752   940  1128
 *         ┌────┬────┬────┬────┬────┬────┬────┐
 *   y=  0 │ M0 │ M1 │ M2 │ M3 │ M4 │ M5 │ M6 │  MARCHE/COURSE (7×188px, h=200)
 *         ├────┼────┼────┼────┼────┼────┼────┤
 *   y=200 │ I0 │ I1 │ I2 │ I3 │ I4 │ I5 │ I6 │  IDLE (7×188px, h=200)
 *         ├─────────┬─────────┬──────┬────┬────┬────┬────┤
 *   y=400 │  A0(280)│  A1(280)│A2(280)│Kt0│Kt1│Kt2│Kt3│  ATTAQUE(0-2,280px) + MORT(3-6,188px)
 *         ├─────────────────────────┬────┬────┬────┤
 *   y=600 │ S0(257) │ S1(257) │S2(257)│Ks0│Ks1│Ks2│  SUPER(0-2,257px) + MORT_SOL(3-5,188px)
 *         └─────────────────────────┴────┴────┴────┘
 */

#include "joueur.h"
#include <stdio.h>
#include <string.h>

/* =====================================================================
 *  TABLE DES ANIMATIONS
 *  { src_x, src_y, frame_w, frame_h, nb_frames, delai_ms, boucle }
 * ===================================================================== */
static const AnimDesc ANIMS[NB_ETATS] = {
    /* ETAT_IDLE          – ligne 1, 7 frames de 188px */
    [ETAT_IDLE]          = {0,              SS_ROW1_Y, SS_FRAME_W,    SS_ROW1_H, 7, 140, 1},

    /* ETAT_MARCHE        – ligne 0, 7 frames de 188px */
    [ETAT_MARCHE]        = {0,              SS_ROW0_Y, SS_FRAME_W,    SS_ROW0_H, 7, 110, 1},

    /* ETAT_COURSE        – même ligne que MARCHE, délai réduit */
    [ETAT_COURSE]        = {0,              SS_ROW0_Y, SS_FRAME_W,    SS_ROW0_H, 7,  70, 1},

    /* ETAT_SAUT          – réutilise les 4 premières frames IDLE */
    [ETAT_SAUT]          = {0,              SS_ROW1_Y, SS_FRAME_W,    SS_ROW1_H, 4, 120, 1},

    /* ETAT_ATTAQUE       – ligne 2, 3 frames larges (280px chacune) */
    [ETAT_ATTAQUE]       = {0,              SS_ROW2_Y, SS_ATK_FRAME_W, SS_ROW2_H, SS_ATK_NB, 90, 0},

    /* ETAT_SUPER_ATTAQUE – ligne 3, 3 frames très larges (257px chacune) */
    [ETAT_SUPER_ATTAQUE] = {0,              SS_ROW3_Y, SS_SUPER_FRAME_W, SS_ROW3_H, SS_SUPER_NB, 80, 0},

    /* ETAT_MORT          – ligne 2, 4 frames de 188px, départ x=840 */
    [ETAT_MORT]          = {SS_MORT_X,      SS_ROW2_Y, SS_FRAME_W,    SS_ROW2_H, SS_MORT_NB,   130, 0},

    /* ETAT_MORT_SOL      – ligne 3, 3 frames de 188px, départ x=770 */
    [ETAT_MORT_SOL]      = {SS_MORTsol_X,   SS_ROW3_Y, SS_FRAME_W,    SS_ROW3_H, SS_MORTsol_NB, 150, 0},
};

/* =====================================================================
 *  FONCTIONS UTILITAIRES INTERNES
 * ===================================================================== */

/**
 * @brief Crée une texture SDL depuis un texte TTF.
 *        Libère l'ancienne texture si elle existe.
 */
static SDL_Texture *creerTextureTxt(SDL_Renderer *r, TTF_Font *font,
                                    const char *txt, SDL_Color c,
                                    SDL_Texture *old)
{
    if (old) SDL_DestroyTexture(old);

    SDL_Surface *s = TTF_RenderUTF8_Blended(font, txt, c);
    if (!s) { fprintf(stderr, "[ERR] TTF: %s\n", TTF_GetError()); return NULL; }

    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}

/**
 * @brief Assigne les touches par défaut selon le numéro de joueur.
 *        J1 = flèches / J2 = WASD
 */
static void configTouchesDefaut(ConfigTouches *t, int num)
{
    if (num == 1) {
        t->gauche        = SDL_SCANCODE_LEFT;
        t->droite        = SDL_SCANCODE_RIGHT;
        t->sauter        = SDL_SCANCODE_UP;
        t->courir        = SDL_SCANCODE_RSHIFT;
        t->attaquer      = SDL_SCANCODE_RCTRL;
        t->super_attaque = SDL_SCANCODE_KP_0;
    } else {
        t->gauche        = SDL_SCANCODE_A;
        t->droite        = SDL_SCANCODE_D;
        t->sauter        = SDL_SCANCODE_W;
        t->courir        = SDL_SCANCODE_LSHIFT;
        t->attaquer      = SDL_SCANCODE_E;
        t->super_attaque = SDL_SCANCODE_Q;
    }
}

/**
 * @brief Bascule vers un nouvel état et remet la frame à 0 si l'état change.
 */
static void changerEtat(Personne *p, EtatJoueur nouvelEtat)
{
    if (p->etat != nouvelEtat) {
        p->etatPrecedent    = p->etat;
        p->etat             = nouvelEtat;
        p->frame            = 0;
        p->animFinie        = 0;
        p->dernierTempsAnim = SDL_GetTicks();
    }
}

/* =====================================================================
 *  TÂCHES OBLIGATOIRES
 * ===================================================================== */

/* ------------------------------------------------------------------ */
void initPerso(Personne *p, SDL_Renderer *r,
               const char *sprite, int numJoueur)
{
    /* Positions initiales séparées */
    p->x  = (numJoueur == 1) ? 100.0f : 600.0f;
    p->y  = (float)SOL_Y;
    p->vx = p->vy = 0.0f;

    /* Taille affichée à l'écran (peut différer de la frame source) */
    p->largeur = 96;
    p->hauteur = 96;
    p->auSol   = 1;

    /* Statistiques */
    p->score = SCORE_INIT;
    p->vies  = NB_VIES_INIT;
    p->actif = 1;

    /* Animation : démarre en IDLE */
    p->frame            = 0;
    p->dernierTempsAnim = SDL_GetTicks();
    p->etat             = ETAT_IDLE;
    p->etatPrecedent    = ETAT_IDLE;
    p->regardeDroite    = (numJoueur == 1) ? 1 : 0;
    p->animFinie        = 0;

    /* Ressources SDL */
    p->texture  = NULL;
    p->texScore = NULL;
    p->texVies  = NULL;

    /* Config joueur */
    p->numJoueur  = numJoueur;
    p->costumeIdx = 0;
    snprintf(p->nom, sizeof(p->nom), "Joueur %d", numJoueur);
    configTouchesDefaut(&p->touches, numJoueur);

    /* Chargement de la spritesheet */
    p->texture = IMG_LoadTexture(r, sprite);
    if (!p->texture)
        fprintf(stderr, "[ERR] IMG_LoadTexture('%s') : %s\n",
                sprite, IMG_GetError());
    else
        printf("[OK] Spritesheet chargée : %s (joueur %d)\n",
               sprite, numJoueur);
}

/* ------------------------------------------------------------------ */
void afficherPerso(Personne p, SDL_Renderer *r)
{
    if (!p.actif || !p.texture) return;

    const AnimDesc *a = &ANIMS[p.etat];

    /* Zone SOURCE dans la spritesheet */
    SDL_Rect src = {
        a->src_x + p.frame * a->frame_w,
        a->src_y,
        a->frame_w,
        a->frame_h
    };

    /* Zone DESTINATION à l'écran – centré sur p.x/p.y */
    SDL_Rect dst = {
        (int)p.x,
        (int)p.y,
        p.largeur,
        p.hauteur
    };

    SDL_RendererFlip flip = p.regardeDroite ? SDL_FLIP_NONE
                                            : SDL_FLIP_HORIZONTAL;

    SDL_RenderCopyEx(r, p.texture, &src, &dst, 0.0, NULL, flip);
}

/* ------------------------------------------------------------------ */
void gestionViesScores(Personne *p, SDL_Renderer *r, TTF_Font *font)
{
    /* Tableaux statiques indexés par numéro joueur (0 ou 1) */
    static int dernScore[2] = {-1, -1};
    static int dernVies[2]  = {-1, -1};
    int idx = (p->numJoueur == 1) ? 0 : 1;

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color rouge = {220,  50,  50, 255};

    if (p->score != dernScore[idx]) {
        char buf[64];
        snprintf(buf, sizeof(buf), "Score J%d : %d", p->numJoueur, p->score);
        p->texScore = creerTextureTxt(r, font, buf, blanc, p->texScore);
        dernScore[idx] = p->score;
    }
    if (p->vies != dernVies[idx]) {
        char buf[64];
        snprintf(buf, sizeof(buf), "Vies J%d : %d", p->numJoueur, p->vies);
        SDL_Color c = (p->vies <= 1) ? rouge : blanc;
        p->texVies = creerTextureTxt(r, font, buf, c, p->texVies);
        dernVies[idx] = p->vies;
    }

    int ox = (p->numJoueur == 1) ? 10 : 500;
    if (p->texScore) {
        SDL_Rect d = {ox, 10, 200, 30};
        SDL_RenderCopy(r, p->texScore, NULL, &d);
    }
    if (p->texVies) {
        SDL_Rect d = {ox, 45, 180, 30};
        SDL_RenderCopy(r, p->texVies, NULL, &d);
    }

    /* Mort si plus de vies */
    if (p->vies <= 0 && p->etat != ETAT_MORT && p->etat != ETAT_MORT_SOL) {
        changerEtat(p, ETAT_MORT_SOL);
        p->actif = 0;
    }
}

/* ------------------------------------------------------------------ */
void animerPerso(Personne *p)
{
    if (p->animFinie) return;

    const AnimDesc *a = &ANIMS[p->etat];
    Uint32 now = SDL_GetTicks();

    if (now - p->dernierTempsAnim >= (Uint32)a->delai_ms) {
        p->frame++;
        p->dernierTempsAnim = now;

        if (p->frame >= a->nb_frames) {
            if (a->boucle) {
                p->frame = 0;
            } else {
                p->frame     = a->nb_frames - 1;
                p->animFinie = 1;

                /* Retour automatique après attaque */
                if (p->etat == ETAT_ATTAQUE || p->etat == ETAT_SUPER_ATTAQUE) {
                    changerEtat(p, ETAT_IDLE);
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
void deplacerPerso(Personne *p, const Uint8 *keys)
{
    if (!p->actif) return;

    /* Bloqué si animation non-bouclée en cours */
    int enAttaque = (p->etat == ETAT_ATTAQUE || p->etat == ETAT_SUPER_ATTAQUE)
                    && !p->animFinie;
    if (enAttaque) {
        /* Applique quand même la gravité */
        p->vy += GRAVITE;
        p->y  += p->vy;
        if (p->y >= (float)SOL_Y) { p->y = (float)SOL_Y; p->vy = 0; p->auSol = 1; }
        else p->auSol = 0;
        return;
    }

    /* Déclenchement des attaques */
    if (keys[p->touches.super_attaque]) {
        changerEtat(p, ETAT_SUPER_ATTAQUE);
        return;
    }
    if (keys[p->touches.attaquer]) {
        changerEtat(p, ETAT_ATTAQUE);
        return;
    }

    /* Déplacement horizontal */
    int enCourse = keys[p->touches.courir];

    if (keys[p->touches.gauche]) {
        p->vx            = enCourse ? -(float)VITESSE_COURSE : -(float)VITESSE_MARCHE;
        p->regardeDroite = 0;
        if (p->auSol) changerEtat(p, enCourse ? ETAT_COURSE : ETAT_MARCHE);
    } else if (keys[p->touches.droite]) {
        p->vx            = enCourse ?  (float)VITESSE_COURSE :  (float)VITESSE_MARCHE;
        p->regardeDroite = 1;
        if (p->auSol) changerEtat(p, enCourse ? ETAT_COURSE : ETAT_MARCHE);
    } else {
        p->vx = 0;
        if (p->auSol && p->etat != ETAT_SAUT)
            changerEtat(p, ETAT_IDLE);
    }

    /* Saut */
    if (keys[p->touches.sauter]) saut(p);

    /* Gravité */
    p->vy += GRAVITE;

    /* Application vélocités */
    p->x += p->vx;
    p->y += p->vy;

    /* Bornes horizontales de l'écran */
    if (p->x < 0) p->x = 0;
    if (p->x > 800 - p->largeur) p->x = (float)(800 - p->largeur);

    /* Sol */
    if (p->y >= (float)SOL_Y) {
        p->y    = (float)SOL_Y;
        p->vy   = 0.0f;
        if (!p->auSol && p->etat == ETAT_SAUT)
            changerEtat(p, ETAT_IDLE);
        p->auSol = 1;
    } else {
        p->auSol = 0;
    }
}

/* ------------------------------------------------------------------ */
void saut(Personne *p)
{
    if (p->auSol) {
        p->vy    = -FORCE_SAUT;
        p->auSol = 0;
        changerEtat(p, ETAT_SAUT);
    }
}

/* ------------------------------------------------------------------ */
void detruirePerso(Personne *p)
{
    if (p->texture)  { SDL_DestroyTexture(p->texture);  p->texture  = NULL; }
    if (p->texScore) { SDL_DestroyTexture(p->texScore); p->texScore = NULL; }
    if (p->texVies)  { SDL_DestroyTexture(p->texVies);  p->texVies  = NULL; }
}

/* =====================================================================
 *  TÂCHES OPTIONNELLES
 * ===================================================================== */

/* ------------------------------------------------------------------ */
void initJoueur2(Personne *p2, SDL_Renderer *r, const char *sprite)
{
    initPerso(p2, r, sprite, 2);
    p2->x = 600.0f;
}

/* ------------------------------------------------------------------ */
int sousMenuJoueur(Personne *p, SDL_Renderer *r, TTF_Font *font)
{
    const char *costumes[] = {"Héros", "Ninja", "Robot"};
    const int   NB = 3;

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color jaune = {255, 220,   0, 255};

    int selection = p->costumeIdx;
    int quitter   = 0, valide = 0;
    SDL_Event ev;

    while (!quitter) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) { quitter = 1; break; }
            if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                        selection = (selection - 1 + NB) % NB; break;
                    case SDL_SCANCODE_DOWN:
                        selection = (selection + 1) % NB;      break;
                    case SDL_SCANCODE_RETURN:
                        p->costumeIdx = selection;
                        quitter = valide = 1; break;
                    case SDL_SCANCODE_ESCAPE:
                        quitter = 1; break;
                    default: break;
                }
            }
        }

        SDL_SetRenderDrawColor(r, 20, 20, 30, 255);
        SDL_RenderClear(r);

        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 40, 40, 40, 210);
        SDL_Rect panneau = {150, 100, 500, 350};
        SDL_RenderFillRect(r, &panneau);

        char titre[64];
        snprintf(titre, sizeof(titre), "Choisir costume - Joueur %d", p->numJoueur);
        SDL_Texture *tTitre = creerTextureTxt(r, font, titre, jaune, NULL);
        if (tTitre) {
            SDL_Rect d = {170, 120, 380, 36};
            SDL_RenderCopy(r, tTitre, NULL, &d);
            SDL_DestroyTexture(tTitre);
        }

        for (int i = 0; i < NB; i++) {
            char ligne[64];
            snprintf(ligne, sizeof(ligne), "%s %s",
                     i == selection ? ">" : " ", costumes[i]);
            SDL_Color c = (i == selection) ? jaune : blanc;
            SDL_Texture *tl = creerTextureTxt(r, font, ligne, c, NULL);
            if (tl) {
                SDL_Rect d = {220, 200 + i * 55, 260, 38};
                SDL_RenderCopy(r, tl, NULL, &d);
                SDL_DestroyTexture(tl);
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    return valide;
}
