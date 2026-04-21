#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "decl.h"
#include "game.h"
#include "joueur.h"
#include "ennemi.h"
#include "minimap_game.h"

typedef enum {
	SECOND_CHANCE_NONE = 0,
	SECOND_CHANCE_ENIGMA = 1,
	SECOND_CHANCE_PUZZLE = 2
} SecondChanceType;

typedef struct {
	GameLevel level;
	int level_initialized;
	Personne player;
	Enemy enemy;
	int gameplay_initialized;
	int player_dead;
	int first_death_chance_used;
	int invuln_ms;
	Uint32 last_projectile_tick;
	Uint32 last_frame_tick;
	Uint32 game_start_tick;
	MiniMapGame minimap;
	int minimap_initialized;
	SecondChanceType second_chance;
	Uint32 second_chance_deadline;
	int puzzle_initialized;
	char name_input[20];
} GameplaySession;

void gameplay_session_reset(GameplaySession *session);
int gameplay_enter(GameplaySession *session, SDL_Renderer *renderer);
int gameplay_handle_event(
	GameplaySession *session,
	SDL_Event *event,
	SDL_Renderer *renderer,
	TTF_Font *font,
	enum current_menu_state *current_menu
);
void gameplay_frame(
	GameplaySession *session,
	SDL_Renderer *renderer,
	TTF_Font *font,
	enum current_menu_state *current_menu
);
void gameplay_leave(GameplaySession *session);

#endif
