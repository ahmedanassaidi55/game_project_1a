/* enigme.c - menu + quiz arrondi, timer, musique vrai/faux */
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enigme.h"

extern int quiz_active;

Enigme *current_enigme = NULL;
int enigma_result = 0;
int quiz_correct_count = 0;
int quiz_answered_count = 0;
int quiz_score = 0;

static Uint32 s_question_deadline_ms = 0;

static Mix_Music *s_music_true = NULL;
static Mix_Music *s_music_false = NULL;
static int s_music_loaded = 0;

/* --- petit utilitaire: rectangle arrondi rempli (niveau cours) --- */
static void set_draw_color(SDL_Renderer *r, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

static void fill_quarter_tl(SDL_Renderer *rend, int cx, int cy, int r, SDL_Color fill) {
    set_draw_color(rend, fill);
    for (int yp = cy - r; yp <= cy; yp++) {
        int dy = yp - cy;
        int dx = (int)sqrt((double)(r * r - dy * dy));
        if (dx > 0) {
            SDL_Rect seg = {cx - dx, yp, dx, 1};
            SDL_RenderFillRect(rend, &seg);
        }
    }
}

static void fill_quarter_tr(SDL_Renderer *rend, int cx, int cy, int r, SDL_Color fill) {
    set_draw_color(rend, fill);
    for (int yp = cy - r; yp <= cy; yp++) {
        int dy = yp - cy;
        int dx = (int)sqrt((double)(r * r - dy * dy));
        if (dx > 0) {
            SDL_Rect seg = {cx, yp, dx, 1};
            SDL_RenderFillRect(rend, &seg);
        }
    }
}

static void fill_quarter_bl(SDL_Renderer *rend, int cx, int cy, int r, SDL_Color fill) {
    set_draw_color(rend, fill);
    for (int yp = cy; yp <= cy + r; yp++) {
        int dy = yp - cy;
        int dx = (int)sqrt((double)(r * r - dy * dy));
        if (dx > 0) {
            SDL_Rect seg = {cx - dx, yp, dx, 1};
            SDL_RenderFillRect(rend, &seg);
        }
    }
}

static void fill_quarter_br(SDL_Renderer *rend, int cx, int cy, int r, SDL_Color fill) {
    set_draw_color(rend, fill);
    for (int yp = cy; yp <= cy + r; yp++) {
        int dy = yp - cy;
        int dx = (int)sqrt((double)(r * r - dy * dy));
        if (dx > 0) {
            SDL_Rect seg = {cx, yp, dx, 1};
            SDL_RenderFillRect(rend, &seg);
        }
    }
}

static void fill_round_rect(SDL_Renderer *rend, SDL_Rect rect, int rad, SDL_Color fill) {
    int x = rect.x;
    int y = rect.y;
    int w = rect.w;
    int h = rect.h;
    int r = rad;
    if (r > w / 2) {
        r = w / 2;
    }
    if (r > h / 2) {
        r = h / 2;
    }

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    set_draw_color(rend, fill);

    SDL_Rect mid = {x + r, y, w - 2 * r, h};
    SDL_RenderFillRect(rend, &mid);
    SDL_Rect left = {x, y + r, r, h - 2 * r};
    SDL_RenderFillRect(rend, &left);
    SDL_Rect right = {x + w - r, y + r, r, h - 2 * r};
    SDL_RenderFillRect(rend, &right);

    fill_quarter_tl(rend, x + r, y + r, r, fill);
    fill_quarter_tr(rend, x + w - r, y + r, r, fill);
    fill_quarter_bl(rend, x + r, y + h - r, r, fill);
    fill_quarter_br(rend, x + w - r, y + h - r, r, fill);
}

static void stroke_round_rect(SDL_Renderer *rend, SDL_Rect rect, int rad, SDL_Color border) {
    (void)rad;
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    set_draw_color(rend, border);
    SDL_RenderDrawRect(rend, &rect);
}

Enigme *generer_enigme(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Erreur ouverture %s\n", filename);
        return NULL;
    }

    int nb;
    if (fscanf(f, "%d\n", &nb) != 1 || nb <= 0) {
        fclose(f);
        return NULL;
    }
    int chosen = rand() % nb;

    Enigme *e = (Enigme *)calloc(1, sizeof(Enigme));
    if (!e) {
        fclose(f);
        return NULL;
    }

    char line[256];
    for (int i = 0; i < chosen; i++) {
        for (int j = 0; j < 5; j++) {
            if (!fgets(line, sizeof(line), f)) {
                free(e);
                fclose(f);
                return NULL;
            }
        }
    }

    if (!fgets(e->question, sizeof(e->question), f)) {
        free(e);
        fclose(f);
        return NULL;
    }
    e->question[strcspn(e->question, "\n")] = 0;
    for (int i = 0; i < 3; i++) {
        if (!fgets(e->rep[i], sizeof(e->rep[i]), f)) {
            free(e);
            fclose(f);
            return NULL;
        }
        e->rep[i][strcspn(e->rep[i], "\n")] = 0;
    }
    int correct;
    if (fscanf(f, "%d", &correct) != 1) {
        free(e);
        fclose(f);
        return NULL;
    }
    e->bonne_reponse = correct - 1;

    fclose(f);
    return e;
}

void create_quiz_textures(SDL_Renderer *renderer, TTF_Font *font) {
    if (!current_enigme || !font) {
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color dark = {30, 32, 40, 255};

    if (current_enigme->tex_question) {
        SDL_DestroyTexture(current_enigme->tex_question);
    }
    for (int i = 0; i < 3; i++) {
        if (current_enigme->tex_rep[i]) {
            SDL_DestroyTexture(current_enigme->tex_rep[i]);
            current_enigme->tex_rep[i] = NULL;
        }
    }

    SDL_Surface *qsurf = TTF_RenderUTF8_Blended_Wrapped(
        font, current_enigme->question, white, 520);
    if (!qsurf) {
        qsurf = TTF_RenderUTF8_Blended(font, current_enigme->question, white);
    }
    if (qsurf) {
        current_enigme->tex_question = SDL_CreateTextureFromSurface(renderer, qsurf);
        SDL_FreeSurface(qsurf);
    }

    for (int i = 0; i < 3; i++) {
        SDL_Surface *asurf = TTF_RenderUTF8_Blended(font, current_enigme->rep[i], dark);
        if (asurf) {
            current_enigme->tex_rep[i] = SDL_CreateTextureFromSurface(renderer, asurf);
            SDL_FreeSurface(asurf);
        }
    }
}

void free_current_enigme(void) {
    if (!current_enigme) {
        return;
    }
    if (current_enigme->tex_question) {
        SDL_DestroyTexture(current_enigme->tex_question);
    }
    for (int i = 0; i < 3; i++) {
        if (current_enigme->tex_rep[i]) {
            SDL_DestroyTexture(current_enigme->tex_rep[i]);
        }
    }
    free(current_enigme);
    current_enigme = NULL;
}

int get_quiz_score(void) {
    return quiz_score;
}

void copy_quiz_score_to(int *score_var) {
    if (score_var) {
        *score_var = quiz_score;
    }
}

static void load_answer_music(void) {
    if (s_music_loaded) {
        return;
    }
    s_music_loaded = 1;

    s_music_true = Mix_LoadMUS("asset/true.weba");
    if (!s_music_true) {
        s_music_true = Mix_LoadMUS("assets/true.weba");
    }
    if (!s_music_true) {
        s_music_true = Mix_LoadMUS("asset/true.mp3");
    }
    if (!s_music_true) {
        s_music_true = Mix_LoadMUS("assets/true.mp3");
    }

    s_music_false = Mix_LoadMUS("asset/false.weba");
    if (!s_music_false) {
        s_music_false = Mix_LoadMUS("assets/false.weba");
    }
    if (!s_music_false) {
        s_music_false = Mix_LoadMUS("asset/false.mp3");
    }
    if (!s_music_false) {
        s_music_false = Mix_LoadMUS("assets/false.mp3");
    }

    if (!s_music_true) {
        printf("Note: assets/music_true.* manquant (optionnel).\n");
    }
    if (!s_music_false) {
        printf("Note: assets/music_false.* manquant (optionnel).\n");
    }
}

static void play_feedback_music(int is_correct) {
    load_answer_music();
    Mix_Music *m = is_correct ? s_music_true : s_music_false;
    if (m) {
        Mix_HaltMusic();
        Mix_PlayMusic(m, 0);
    }
}

static void reset_question_timer(void) {
    s_question_deadline_ms = SDL_GetTicks() + (Uint32)(QUIZ_SECONDS_PER_QUESTION * 1000);
}

static void quiz_finish_session(SDL_Renderer *renderer, TTF_Font *font) {
    (void)renderer;
    (void)font;
    quiz_score = quiz_correct_count;
    if (quiz_correct_count >= QUIZ_TOTAL_QUESTIONS) {
        enigma_result = 1;
        printf("Fin: %d/%d bonnes — succes.\n", quiz_correct_count, QUIZ_TOTAL_QUESTIONS);
        play_feedback_music(1);
    } else {
        enigma_result = -1;
        printf("Fin: %d/%d bonnes — echec.\n", quiz_correct_count, QUIZ_TOTAL_QUESTIONS);
        play_feedback_music(0);
    }
    quiz_active = 0;
    free_current_enigme();
}

static void quiz_next_question(SDL_Renderer *renderer, TTF_Font *font) {
    current_enigme = generer_enigme("assets/enigmes.txt");
    if (current_enigme) {
        create_quiz_textures(renderer, font);
        reset_question_timer();
    } else {
        enigma_result = -1;
        quiz_active = 0;
    }
}

/* choice:0..2 = reponse, -1 = temps ecoule */
static void quiz_after_choice(SDL_Renderer *renderer, TTF_Font *font, int choice) {
    if (!current_enigme) {
        return;
    }

    int ok = (choice >= 0 && choice == current_enigme->bonne_reponse);
    if (choice == -1) {
        ok = 0;
        printf("Temps ecoule !\n");
    }

    if (ok) {
        quiz_correct_count++;
        printf("Bonne reponse.\n");
    } else if (choice >= 0) {
        printf("Mauvaise reponse.\n");
    }

    free_current_enigme();
    quiz_answered_count++;

    if (quiz_answered_count < QUIZ_TOTAL_QUESTIONS) {
        quiz_next_question(renderer, font);
    } else {
        quiz_finish_session(renderer, font);
    }
}

void enigma_frame_update(SDL_Renderer *renderer, TTF_Font *font) {
    if (!quiz_active || !current_enigme) {
        return;
    }
    Uint32 now = SDL_GetTicks();
    if (now >= s_question_deadline_ms) {
        quiz_after_choice(renderer, font, -1);
    }
}

/* Panneau quiz: meme valeurs que le rendu */
#define PANEL_X 50
#define PANEL_Y 70
#define PANEL_W 700
#define PANEL_H 460
#define PANEL_R 28

static void get_answer_rects(SDL_Rect out[3]) {
    int bw = 200;
    int bh = 46;
    int gap = 18;
    int total = 3 * bw + 2 * gap;
    int start_x = (800 - total) / 2;
    int y = PANEL_Y + 300;
    for (int i = 0; i < 3; i++) {
        out[i] = (SDL_Rect){start_x + i * (bw + gap), y, bw, bh};
    }
}

static int point_in_round_rect(int px, int py, SDL_Rect r) {
    return px >= r.x && px < r.x + r.w && py >= r.y && py < r.y + r.h;
}

void handle_enigma_events(SDL_Event *event, SDL_Renderer *renderer, TTF_Font *font) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int x = event->button.x;
        int y = event->button.y;

        if (quiz_active && current_enigme) {
            SDL_Rect ans[3];
            get_answer_rects(ans);
            for (int i = 0; i < 3; i++) {
                if (point_in_round_rect(x, y, ans[i])) {
                    quiz_after_choice(renderer, font, i);
                    return;
                }
            }
            return;
        }

        /* menu: bouton lancer */
        SDL_Rect start = {300, 268, 200, 52};
        if (point_in_round_rect(x, y, start)) {
            quiz_correct_count = 0;
            quiz_answered_count = 0;
            quiz_score = 0;
            enigma_result = 0;
            free_current_enigme();
            quiz_active = 1;
            quiz_next_question(renderer, font);
        }
    }
}

static void draw_text_center(SDL_Renderer *r, TTF_Font *font, const char *txt, int cx, int cy, SDL_Color col) {
    if (!font || !txt) {
        return;
    }
    SDL_Surface *s = TTF_RenderUTF8_Blended(font, txt, col);
    if (!s) {
        return;
    }
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    if (!t) {
        return;
    }
    int tw, th;
    SDL_QueryTexture(t, NULL, NULL, &tw, &th);
    SDL_Rect dst = {cx - tw / 2, cy - th / 2, tw, th};
    SDL_RenderCopy(r, t, NULL, &dst);
    SDL_DestroyTexture(t);
}

void enigma_menu(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!quiz_active) {
        SDL_Color panel_bg = {25, 28, 35, 210};
        SDL_Color panel_bd = {120, 180, 255, 255};
        SDL_Rect panel = {120, 120, 560, 320};
        fill_round_rect(renderer, panel, 24, panel_bg);
        stroke_round_rect(renderer, panel, 24, panel_bd);

        draw_text_center(renderer, font, "Mini-quiz (projet SDL)", 400, 175, (SDL_Color){240, 245, 255, 255});
        draw_text_center(renderer, font, "2 bonnes reponses sur 3 pour gagner", 400, 215, (SDL_Color){200, 205, 220, 255});
        draw_text_center(renderer, font, "Chrono par question", 400, 245, (SDL_Color){200, 205, 220, 255});

        SDL_Rect start = {300, 268, 200, 52};
        int hover = point_in_round_rect(mouse_x, mouse_y, start);
        SDL_Color btn_fill = hover ? (SDL_Color){90, 140, 220, 255} : (SDL_Color){60, 110, 190, 255};
        fill_round_rect(renderer, start, 18, btn_fill);
        stroke_round_rect(renderer, start, 18, (SDL_Color){200, 230, 255, 255});
        draw_text_center(renderer, font, "Lancer le quiz", start.x + start.w / 2, start.y + start.h / 2,
                         (SDL_Color){255, 255, 255, 255});
        return;
    }

    if (!current_enigme) {
        return;
    }

    /* carte quiz arrondie */
    SDL_Rect panel = {PANEL_X, PANEL_Y, PANEL_W, PANEL_H};
    fill_round_rect(renderer, panel, PANEL_R, (SDL_Color){20, 22, 30, 235});
    stroke_round_rect(renderer, panel, PANEL_R, (SDL_Color){100, 170, 255, 255});

    /* timer */
    Uint32 now = SDL_GetTicks();
    int left = (int)((s_question_deadline_ms > now) ? (s_question_deadline_ms - now + 999) / 1000 : 0);
    char tbuf[64];
    snprintf(tbuf, sizeof(tbuf), "Temps: %d s", left);
    draw_text_center(renderer, font, tbuf, PANEL_X + PANEL_W - 90, PANEL_Y + 36, (SDL_Color){255, 220, 120, 255});

    char prog[48];
    snprintf(prog, sizeof(prog), "Question %d / %d", quiz_answered_count + 1, QUIZ_TOTAL_QUESTIONS);
    draw_text_center(renderer, font, prog, 400, PANEL_Y + 36, (SDL_Color){200, 210, 230, 255});

    /* question */
    int qx = PANEL_X + 40;
    int qy = PANEL_Y + 85;
    if (current_enigme->tex_question) {
        int tw, th;
        SDL_QueryTexture(current_enigme->tex_question, NULL, NULL, &tw, &th);
        if (th > 140) {
            th = 140;
        }
        SDL_Rect dq = {qx, qy, tw, th};
        SDL_RenderCopy(renderer, current_enigme->tex_question, NULL, &dq);
    }

    SDL_Rect ans[3];
    get_answer_rects(ans);
    for (int i = 0; i < 3; i++) {
        int hov = point_in_round_rect(mouse_x, mouse_y, ans[i]);
        SDL_Color base = hov ? (SDL_Color){230, 240, 255, 255} : (SDL_Color){245, 248, 252, 255};
        fill_round_rect(renderer, ans[i], 16, base);
        stroke_round_rect(renderer, ans[i], 16, (SDL_Color){70, 120, 200, 255});
        if (current_enigme->tex_rep[i]) {
            int tw, th;
            SDL_QueryTexture(current_enigme->tex_rep[i], NULL, NULL, &tw, &th);
            int pad = 12;
            int maxw = ans[i].w -2 * pad;
            if (tw > maxw) {
                th = th * maxw / tw;
                tw = maxw;
            }
            SDL_Rect dst = {ans[i].x + (ans[i].w - tw) / 2, ans[i].y + (ans[i].h - th) / 2, tw, th};
            SDL_RenderCopy(renderer, current_enigme->tex_rep[i], NULL, &dst);
        }
    }
}
