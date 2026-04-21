#include "game.h"
#include <math.h>

static float clamp_f(float value, float min_value, float max_value){
	if(value < min_value) return min_value;
	if(value > max_value) return max_value;
	return value;
}

int init_level(GameLevel *level, SDL_Renderer *renderer, int player_count){
	if(!level || !renderer || player_count <= 0){
		return 1;
	}

	level->player_count = player_count;
	level->backgrounds = init_background(renderer, player_count);
	if(!level->backgrounds){
		return 1;
	}

	level->player.world_x = BACKGROUND_WIDTH / 2;
	level->player.world_y = BACKGROUND_HEIGHT / 2;
	level->player.velocity_y = 0.0f;
	level->player.gravity = 0.35f;
	level->player.speed = 6;

	for(int i = 0; i < player_count; i++){
		center_background_on_player(
			&level->backgrounds[i],
			level->player.world_x,
			level->player.world_y,
			SCREEN_WIDTH / player_count,
			SCREEN_HEIGHT
		);
	}

	return 0;
}

void update_level(GameLevel *level, const Uint8 *keyboard_state){
	if(!level || !level->backgrounds || !keyboard_state){
		return;
	}

	float move_x = 0.0f;
	float move_y = 0.0f;
	float max_speed = level->player.speed;
	int view_width = SCREEN_WIDTH / level->player_count;
	int view_height = SCREEN_HEIGHT;

	if(keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_UP]) move_y -= max_speed;
	if(keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDL_SCANCODE_DOWN]) move_y += max_speed;
	if(keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT]) move_x -= max_speed;
	if(keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT]) move_x += max_speed;

	/* Gravity contributes vertical movement every frame. */
	level->player.velocity_y += level->player.gravity;
	if(level->player.velocity_y > max_speed){
		level->player.velocity_y = max_speed;
	}
	move_y += level->player.velocity_y;

	/* Normalize so diagonal/combined (input + gravity) speed stays constant. */
	float magnitude = sqrtf((move_x * move_x) + (move_y * move_y));
	if(magnitude > max_speed && magnitude > 0.0f){
		float scale = max_speed / magnitude;
		move_x *= scale;
		move_y *= scale;
	}

	float next_world_x = clamp_f(level->player.world_x + move_x, 0.0f, (float)BACKGROUND_WIDTH);
	float next_world_y = clamp_f(level->player.world_y + move_y, 0.0f, (float)BACKGROUND_HEIGHT);

	/* Convert centered player target to camera offsets for collision-map checks. */
	int current_offset_y = (int)clamp_f(level->player.world_y - (view_height / 2.0f), 0.0f, (float)(BACKGROUND_HEIGHT - view_height));
	int next_offset_x = (int)clamp_f(next_world_x - (view_width / 2.0f), 0.0f, (float)(BACKGROUND_WIDTH - view_width));
	int next_offset_y = (int)clamp_f(next_world_y - (view_height / 2.0f), 0.0f, (float)(BACKGROUND_HEIGHT - view_height));

	/* Use collision map to validate horizontal and vertical movement independently. */
	SDL_Surface *collision_map = level->backgrounds[0].bg_collision_map;
	if(!is_blocked(collision_map, next_offset_x, current_offset_y)){
		level->player.world_x = next_world_x;
	}
	if(!is_blocked(collision_map, (int)clamp_f(level->player.world_x - (view_width / 2.0f), 0.0f, (float)(BACKGROUND_WIDTH - view_width)), next_offset_y)){
		level->player.world_y = next_world_y;
	}else{
		/* Stop vertical drift into blocked zones (e.g., ground). */
		level->player.velocity_y = 0.0f;
	}

	/* Stop gravity accumulation when standing on world bottom. */
	if(level->player.world_y >= BACKGROUND_HEIGHT){
		level->player.velocity_y = 0.0f;
	}

	for(int i = 0; i < level->player_count; i++){
		center_background_on_player(
			&level->backgrounds[i],
			(int)level->player.world_x,
			(int)level->player.world_y,
			view_width,
			view_height
		);
	}
}

void render_level(GameLevel *level, SDL_Renderer *renderer){
	if(!level || !level->backgrounds || !renderer){
		return;
	}
	display_background(level->backgrounds, renderer, level->player_count);
}

void cleanup_level(GameLevel *level){
	if(!level || !level->backgrounds){
		return;
	}
	clean_up_bg(level->backgrounds, NULL, NULL, level->player_count);
	level->backgrounds = NULL;
}
