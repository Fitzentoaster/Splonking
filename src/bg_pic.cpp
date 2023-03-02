//******************************************************************************
//  bg_pic.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "bg_pic.h"
#include "game_state.h"
#include "entity_handler.h"

using std::vector;
using std::string;
using std::unique_ptr;

BG_Pic::BG_Pic(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    source_rect.x = 0;
    source_rect.h = SCREEN_HEIGHT;
    source_rect.w = SCREEN_WIDTH;
    source_rect.y = 0;
}

void BG_Pic::render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_scroller");
    
    SDL_RenderCopy(game_renderer, display_texture, &source_rect, &hitbox);
}


void BG_Pic::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{

}

void BG_Pic::tick_process(unique_ptr<Game_State>&)
{
    source_rect.y += BG_SCROLL_PER_TICK;
    if (source_rect.y > 900)
    {
        source_rect.y -= 900;
    }
}

void BG_Pic::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

BG_Pic::~BG_Pic()
{
    
}