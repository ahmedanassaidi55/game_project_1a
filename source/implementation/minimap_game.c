#include "minimap_game.h"

static int clampi(int value, int minv, int maxv){
    if(value < minv) return minv;
    if(value > maxv) return maxv;
    return value;
}

bool initMiniMapGame(MiniMapGame *m, SDL_Renderer *renderer, int world_w, int world_h){
    int tex_w = 0;
    int tex_h = 0;

    if(!m || !renderer || world_w <= 0 || world_h <= 0){
        SDL_Log("MiniMap init failed: invalid arguments");
        return false;
    }

    m->map_bg = NULL;
    m->player_icon = NULL;
    m->enemy_icon = NULL;
    m->world_w = world_w;
    m->world_h = world_h;

    m->map_bg = IMG_LoadTexture(renderer, "assets/images/mapbg.png");
    m->player_icon = IMG_LoadTexture(renderer, "assets/images/IMG_4433.jpg");
    m->enemy_icon = IMG_LoadTexture(renderer, "assets/images/IMG_4446.jpg");
    if(!m->map_bg || !m->player_icon || !m->enemy_icon){
        SDL_Log("MiniMap texture load failed: %s", IMG_GetError());
        freeMiniMapGame(m);
        return false;
    }

    m->pos_minimap.x = 410;
    m->pos_minimap.y = 10;

    if(SDL_QueryTexture(m->map_bg, NULL, NULL, &tex_w, &tex_h) != 0){
        SDL_Log("SDL_QueryTexture failed: %s", SDL_GetError());
        freeMiniMapGame(m);
        return false;
    }

    m->pos_minimap.w = clampi(tex_w / 10, 160, 180);
    m->pos_minimap.h = clampi(tex_h / 10, 100, 120);
    m->scale_x = (float)m->pos_minimap.w / (float)world_w;
    m->scale_y = (float)m->pos_minimap.h / (float)world_h;
    m->pos_player.w = 8;
    m->pos_player.h = 8;
    m->pos_enemy.w = 8;
    m->pos_enemy.h = 8;
    return true;
}

void updateMiniMapGame(MiniMapGame *m, SDL_Rect pos_joueur_monde, SDL_Rect pos_enemy_monde){
    int px = m->pos_minimap.x + (int)(pos_joueur_monde.x * m->scale_x);
    int py = m->pos_minimap.y + (int)(pos_joueur_monde.y * m->scale_y);
    int ex = m->pos_minimap.x + (int)(pos_enemy_monde.x * m->scale_x);
    int ey = m->pos_minimap.y + (int)(pos_enemy_monde.y * m->scale_y);
    int max_x = m->pos_minimap.x + m->pos_minimap.w - m->pos_player.w;
    int max_y = m->pos_minimap.y + m->pos_minimap.h - m->pos_player.h;

    m->pos_player.x = clampi(px, m->pos_minimap.x, max_x);
    m->pos_player.y = clampi(py, m->pos_minimap.y, max_y);
    m->pos_enemy.x = clampi(ex, m->pos_minimap.x, max_x);
    m->pos_enemy.y = clampi(ey, m->pos_minimap.y, max_y);
}

void renderMiniMapGame(MiniMapGame *m, SDL_Renderer *renderer){
    if(!m || !renderer || !m->map_bg || !m->player_icon || !m->enemy_icon){
        return;
    }
    SDL_RenderCopy(renderer, m->map_bg, NULL, &m->pos_minimap);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &m->pos_minimap);
    SDL_RenderCopy(renderer, m->player_icon, NULL, &m->pos_player);
    SDL_RenderCopy(renderer, m->enemy_icon, NULL, &m->pos_enemy);
}

void freeMiniMapGame(MiniMapGame *m){
    if(!m) return;
    if(m->map_bg) SDL_DestroyTexture(m->map_bg);
    if(m->player_icon) SDL_DestroyTexture(m->player_icon);
    if(m->enemy_icon) SDL_DestroyTexture(m->enemy_icon);
    m->map_bg = NULL;
    m->player_icon = NULL;
    m->enemy_icon = NULL;
}
