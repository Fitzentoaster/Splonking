//******************************************************************************
//  explosion.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "explosion.h"
#include "game_state.h"
#include "bullet.h"
#include "entity_handler.h"

#include <SDL.h>

#include <iostream>

using std::string;
using std::unique_ptr;

Explosion::Explosion(const Entity_Data init) : Entity(init)
{
    ticks = 0;
    anim_frame_cur = 0;
    anim_frames_max = 79;
    path = init.cwd;
    src_rect = {0 , 0, 100, 100};
}

void Explosion::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    ticks += delta;
    if (ticks > MS_WAIT)
    {
        src_rect.x = anim_frame_cur % 8 * 100;
        src_rect.y = anim_frame_cur / 8 * 100;
        anim_frame_cur++;
        ticks = 0;
    }
    if (anim_frame_cur > anim_frames_max)
    {
        die(game_state);
    }
}

void Explosion::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    
}

void Explosion::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("anim_explosion1");

    SDL_RenderCopy(game_renderer, display_texture, &src_rect, &hitbox);
}

void Explosion::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

Explosion::~Explosion()
{

}