#ifndef GAME_H
#define GAME_H

#include "background.h"

typedef struct {
	float world_x;
	float world_y;
	float velocity_y;
	float gravity;
	float speed;
} Player;

typedef struct {
	Background *backgrounds;
	int player_count;
	Player player;
} GameLevel;

int init_level(GameLevel *level, SDL_Renderer *renderer, int player_count);
void update_level(GameLevel *level, const Uint8 *keyboard_state);
void render_level(GameLevel *level, SDL_Renderer *renderer);
void cleanup_level(GameLevel *level);

#endif
