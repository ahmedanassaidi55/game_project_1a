#include "gameplay.h"
#include "enigma.h"
#include "Puzzle.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

extern int quiz_active;

static void save_highscore_entry(const char *name, int score_seconds){
	FILE *f = fopen("saves/highscores.txt", "a");
	if(!f){
		printf("failed opening highscores file\n");
		return;
	}
	int h = score_seconds / 3600;
	int m = (score_seconds % 3600) / 60;
	int s = score_seconds % 60;
	char t[10];
	snprintf(t, sizeof(t), "%02d:%02d:%02d", h, m, s);
	fprintf(f, "%d|%s|%s\n", score_seconds, name, t);
	fclose(f);
}

static int handle_name_input(SDL_Event *event, char *buffer, size_t size){
	if(event->type != SDL_KEYDOWN){
		return 0;
	}
	SDL_Keycode key = event->key.keysym.sym;
	size_t len = strlen(buffer);
	if(key == SDLK_RETURN){
		return 1;
	}
	if(key == SDLK_BACKSPACE && len > 0){
		buffer[len - 1] = '\0';
		return 0;
	}
	if((isalnum((unsigned char)key) || key == SDLK_SPACE) && len + 1 < size){
		buffer[len] = (key == SDLK_SPACE) ? ' ' : (char)key;
		buffer[len + 1] = '\0';
	}
	return 0;
}

static void render_name_prompt(SDL_Renderer *renderer, TTF_Font *font, const char *name){
	SDL_Color white = {255, 255, 255, 255};
	SDL_Rect panel = {60, 90, 480, 180};
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 220);
	SDL_RenderFillRect(renderer, &panel);

	SDL_Surface *title_s = TTF_RenderText_Blended(font, "Game Over - Enter Name:", white);
	SDL_Surface *name_s = TTF_RenderText_Blended(font, name[0] ? name : "_", white);
	if(title_s && name_s){
		SDL_Texture *title_t = SDL_CreateTextureFromSurface(renderer, title_s);
		SDL_Texture *name_t = SDL_CreateTextureFromSurface(renderer, name_s);
		SDL_Rect title_r = {80, 130, title_s->w, title_s->h};
		SDL_Rect name_r = {80, 175, name_s->w, name_s->h};
		SDL_RenderCopy(renderer, title_t, NULL, &title_r);
		SDL_RenderCopy(renderer, name_t, NULL, &name_r);
		SDL_DestroyTexture(title_t);
		SDL_DestroyTexture(name_t);
	}
	if(title_s) SDL_FreeSurface(title_s);
	if(name_s) SDL_FreeSurface(name_s);
}

void gameplay_session_reset(GameplaySession *s){
	if(!s) return;
	memset(s, 0, sizeof(*s));
	s->second_chance = SECOND_CHANCE_NONE;
}

int gameplay_enter(GameplaySession *s, SDL_Renderer *renderer){
	if(!s || !renderer){
		return 1;
	}
	if(s->gameplay_initialized){
		return 0;
	}
	int player_count = character_mode > 0 ? character_mode : 1;
	if(init_level(&s->level, renderer, player_count) != 0){
		printf("failed to initialize level\n");
		return 1;
	}
	s->level_initialized = 1;

	initPerso(&s->player, renderer, "assets/images/orig_big.png", 1);
	s->player.largeur = 72;
	s->player.hauteur = 96;
	s->player.x = 60;
	s->player.y = SOL_Y;

	initEnemy(&s->enemy, renderer);
	s->enemy.x = 480;
	s->enemy.y = SCREEN_HEIGHT - s->enemy.posScreen.h;

	if(initMiniMapGame(&s->minimap, renderer, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)){
		s->minimap_initialized = 1;
	}

	s->game_start_tick = SDL_GetTicks();
	s->last_frame_tick = s->game_start_tick;
	s->last_projectile_tick = s->game_start_tick;
	s->gameplay_initialized = 1;
	return 0;
}

int gameplay_handle_event(GameplaySession *s, SDL_Event *event, SDL_Renderer *renderer, TTF_Font *font, enum current_menu_state *current_menu){
	if(!s || !event || !renderer || !font || !current_menu){
		return 0;
	}

	if(event->type == SDL_MOUSEBUTTONDOWN){
		if(s->second_chance == SECOND_CHANCE_ENIGMA){
			handle_enigma_quiz_events(event, renderer, font);
			return 1;
		}
		if(s->second_chance == SECOND_CHANCE_PUZZLE){
			int gx = 0;
			int gy = 0;
			getGridFromMouse(event->button.x, event->button.y, &gx, &gy);
			if(gx >= 0 && gx < GRID && gy >= 0 && gy < GRID){
				moveTile(gx, gy);
				if(checkWin()){
					winGame = 1;
				}
			}
			return 1;
		}
	}

	if(event->type == SDL_KEYDOWN){
		if(s->second_chance == SECOND_CHANCE_ENIGMA){
			handle_enigma_quiz_events(event, renderer, font);
			return 1;
		}
		if(s->second_chance == SECOND_CHANCE_PUZZLE){
			if(event->key.keysym.sym == SDLK_ESCAPE){
				s->second_chance = SECOND_CHANCE_NONE;
				s->player_dead = 1;
			}
			return 1;
		}
		if(s->player_dead){
			if(handle_name_input(event, s->name_input, sizeof(s->name_input))){
				const char *final_name = s->name_input[0] ? s->name_input : "player";
				int elapsed_seconds = (int)((SDL_GetTicks() - s->game_start_tick) / 1000);
				save_highscore_entry(final_name, elapsed_seconds);
				*current_menu = MENU_HIGHSCORES;
			}
			return 1;
		}
	}
	return 0;
}

void gameplay_frame(GameplaySession *s, SDL_Renderer *renderer, TTF_Font *font, enum current_menu_state *current_menu){
	if(!s || !renderer || !font || !current_menu || !s->gameplay_initialized){
		return;
	}

	Uint32 now = SDL_GetTicks();
	Uint32 delta = now - s->last_frame_tick;
	s->last_frame_tick = now;
	if(s->invuln_ms > 0){
		s->invuln_ms -= (int)delta;
	}

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if(!s->player_dead && s->second_chance == SECOND_CHANCE_NONE){
		update_level(&s->level, keys);
		deplacerPerso(&s->player, keys);
		animerPerso(&s->player);
		EnemyTarget enemy_target = {s->player.x, s->player.y};
		pathfindingSimple(&s->enemy, enemy_target);
		animateEnemy(&s->enemy);
		if(now - s->last_projectile_tick >= 1500){
			shootProjectile(&s->enemy, enemy_target);
			s->last_projectile_tick = now;
		}
		updateProjectiles(&s->enemy);

		SDL_Rect player_rect = {(int)s->player.x, (int)s->player.y, s->player.largeur, s->player.hauteur};
		if(checkCollision(s->enemy.posScreen, player_rect) && s->invuln_ms <= 0){
			s->player.vies--;
			s->invuln_ms = 800;
		}
		for(int i = 0; i < MAX_PROJECTILES; i++){
			if(s->enemy.projectiles[i].active &&
				checkCollision(s->enemy.projectiles[i].rect, player_rect) &&
				s->invuln_ms <= 0){
				s->enemy.projectiles[i].active = 0;
				s->player.vies--;
				s->invuln_ms = 800;
				break;
			}
		}

		if(s->player.vies <= 0){
			if(!s->first_death_chance_used){
				s->first_death_chance_used = 1;
				s->second_chance = (rand() % 2 == 0) ? SECOND_CHANCE_ENIGMA : SECOND_CHANCE_PUZZLE;
				s->second_chance_deadline = now + 30000;
				s->player_dead = 0;
				s->player.vies = 0;
				if(s->second_chance == SECOND_CHANCE_ENIGMA){
					enigma_result = 0;
					quiz_active = 0;
					free_current_enigme();
				}else{
					if(!s->puzzle_initialized){
						ren = renderer;
						img = loadImage("assets/images/IMG_4433.jpg");
						s->puzzle_initialized = 1;
					}
					initPuzzle();
					winGame = 0;
				}
			}else{
				s->player_dead = 1;
			}
		}
	}

	SDL_RenderClear(renderer);
	if(s->second_chance == SECOND_CHANCE_ENIGMA){
		int mouse_x = 0;
		int mouse_y = 0;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		enigma_quiz_frame_update(renderer, font);
		enigma_quiz_menu(renderer, font, mouse_x, mouse_y);
		if(enigma_result == 1){
			s->second_chance = SECOND_CHANCE_NONE;
			s->player.vies = 1;
			s->player_dead = 0;
			s->invuln_ms = 1200;
			enigma_result = 0;
		}else if(enigma_result == -1 || SDL_GetTicks() > s->second_chance_deadline){
			s->second_chance = SECOND_CHANCE_NONE;
			s->player_dead = 1;
			enigma_result = 0;
		}
	}else if(s->second_chance == SECOND_CHANCE_PUZZLE){
		renderPuzzle();
		if(winGame){
			s->second_chance = SECOND_CHANCE_NONE;
			s->player.vies = 1;
			s->player_dead = 0;
			s->invuln_ms = 1200;
		}else if(SDL_GetTicks() > s->second_chance_deadline){
			s->second_chance = SECOND_CHANCE_NONE;
			s->player_dead = 1;
		}
	}else{
		render_level(&s->level, renderer);
		afficherPerso(s->player, renderer);
		renderEnemy(&s->enemy, renderer);
		gestionViesScores(&s->player, renderer, font);
		if(s->minimap_initialized && s->level.backgrounds){
			SDL_Rect player_world = {(int)s->level.player.world_x, (int)s->level.player.world_y, 1, 1};
			SDL_Rect enemy_world = {
				s->level.backgrounds[0].offsetX + s->enemy.posScreen.x,
				s->level.backgrounds[0].offsetY + s->enemy.posScreen.y,
				1, 1
			};
			updateMiniMapGame(&s->minimap, player_world, enemy_world);
			renderMiniMapGame(&s->minimap, renderer);
		}
		if(s->player_dead){
			render_name_prompt(renderer, font, s->name_input);
		}
	}
	SDL_RenderPresent(renderer);

	if(*current_menu != MENU_PLAY){
		gameplay_leave(s);
	}
}

void gameplay_leave(GameplaySession *s){
	if(!s){
		return;
	}
	if(s->level_initialized){
		cleanup_level(&s->level);
		s->level_initialized = 0;
	}
	if(s->gameplay_initialized){
		detruirePerso(&s->player);
		if(s->enemy.sprite){
			SDL_DestroyTexture(s->enemy.sprite);
			s->enemy.sprite = NULL;
		}
		s->gameplay_initialized = 0;
	}
	if(s->minimap_initialized){
		freeMiniMapGame(&s->minimap);
		s->minimap_initialized = 0;
	}
	if(s->puzzle_initialized && img){
		SDL_DestroyTexture(img);
		img = NULL;
	}
	gameplay_session_reset(s);
}
