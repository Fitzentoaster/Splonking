//******************************************************************************
//  bg_pause.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "bg_pause.h"
#include "game_state.h"
#include "entity_handler.h"

using std::vector;
using std::string;
using std::unique_ptr;

BG_Pause::BG_Pause(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    set_visibility(false);
}

void BG_Pause::render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    if (get_is_visible())
    {
        SDL_Rect hitbox = get_hitbox();
        SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_pause");

        SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
    }
}


void BG_Pause::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{

}

void BG_Pause::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

BG_Pause::~BG_Pause()
{
    
}