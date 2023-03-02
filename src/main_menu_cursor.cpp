//******************************************************************************
//  selection_arrow.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "main_menu_cursor.h"
#include "game_state.h"
#include "entity_handler.h"
#include "datastructs.h"

#include <SDL.h>

#include <iostream>


using std::unique_ptr;

Main_Menu_Cursor::Main_Menu_Cursor(const Entity_Data init) : Entity(init)
{
    current_selection = Main_Menu_Option::START;
}


void Main_Menu_Cursor::render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture;
    display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_select_arrow");
    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}

Main_Menu_Option Main_Menu_Cursor::get_current()
{
    return current_selection;
}

void Main_Menu_Cursor::move_up()
{
    switch (current_selection)
    {
        case Main_Menu_Option::START:
            break;
        case Main_Menu_Option::QUIT:
            current_selection = Main_Menu_Option::START;
            set_y_origin(get_y_origin() - 105);
            break;
    }
}

void Main_Menu_Cursor::move_down()
{
    switch (current_selection)
    {
        case Main_Menu_Option::START:
            current_selection = Main_Menu_Option::QUIT;
            set_y_origin(get_y_origin() + 105);
            break;
        case Main_Menu_Option::QUIT:
            break;
    }
}

void Main_Menu_Cursor::frame_process(unique_ptr<Game_State>& game_state, const int delta) {}
void Main_Menu_Cursor::tick_process(unique_ptr<Game_State>& game_state) {}

void Main_Menu_Cursor::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

Main_Menu_Cursor::~Main_Menu_Cursor()
{

}