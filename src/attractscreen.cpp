//******************************************************************************
//  attractscreen.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "attractscreen.h"
#include "game_state.h"
#include "entity_handler.h"

using std::vector;
using std::string;
using std::unique_ptr;

Attract_Screen::Attract_Screen(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
}

void Attract_Screen::render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_attract_screen");

    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}


void Attract_Screen::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{

}

void Attract_Screen::tick_process(unique_ptr<Game_State>& game_state)
{

}

void Attract_Screen::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}



Attract_Screen::~Attract_Screen()
{
    
}