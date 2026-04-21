#ifndef ENIGME_H
#define ENIGME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    char question[256];
    char rep[3][256];
    int bonne_reponse;

    SDL_Texture *tex_question;
    SDL_Texture *tex_rep[3];
} Enigme;

extern Enigme *current_enigme;
extern int enigma_result;
extern int quiz_correct_count;
extern int quiz_answered_count;
extern int quiz_score;

#define QUIZ_TOTAL_QUESTIONS        3
#define QUIZ_SECONDS_PER_QUESTION    15

Enigme *generer_enigme(const char *filename);
void    create_quiz_textures(SDL_Renderer *renderer, TTF_Font *font);
void    free_current_enigme(void);
int     get_quiz_score(void);
void    copy_quiz_score_to(int *score_var);
void    handle_enigma_events(SDL_Event *event, SDL_Renderer *renderer, TTF_Font *font);
void    enigma_menu(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y);
void    enigma_frame_update(SDL_Renderer *renderer, TTF_Font *font);

#endif
