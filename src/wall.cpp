//******************************************************************************
//  wall.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "wall.h"
#include "game_state.h"
#include "entity_handler.h"

#include <SDL.h>
#include <iostream>

using std::unique_ptr;

Wall::Wall(const Entity_Data init) : Entity(init) {}

void Wall::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    if (get_y_pos() < HUD_HEIGHT)
    {
        die(game_state);
    }
    update_position(delta);
}

void Wall::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    SDL_Rect wall_hitbox = get_hitbox();
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            collidee->die(game_state);
            break;
        case Col_Class::BULLET:
            collidee->die(game_state);
            break;
        case Col_Class::ENEMY:
            collidee->die(game_state);
            break;
        case Col_Class::MISSILE:
            collidee->die(game_state);
            break;
        case Col_Class::BOUNCER:
        case Col_Class::FUEL_PICKUP:
        case Col_Class::SHIELD_PICKUP:
        case Col_Class::OBSTACLE:
            if (collidee->check_edge_collision('U', &wall_hitbox))
            {
                collidee->jump_for_reflect('D');
                collidee->set_reflected_horizontal(true);
            }
            else if (collidee->check_edge_collision('D', &wall_hitbox))
            {
                collidee->jump_for_reflect('U');
                collidee->set_reflected_horizontal(true);
            }
            else if (collidee->check_edge_collision('L', &wall_hitbox))
            {
                collidee->jump_for_reflect('R');
                collidee->set_reflected_horizontal(true);
            }
            else if (collidee->check_edge_collision('R', &wall_hitbox))
            {
                collidee->jump_for_reflect('L');
                collidee->set_reflected_horizontal(true);
            }
            break;
        default:
            break;
    }
}

void Wall::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_SetRenderDrawColor(game_renderer, get_color().red, get_color().green, get_color().blue, SDL_ALPHA_OPAQUE);
    SDL_Rect hitbox = get_hitbox();
    const SDL_Rect* wall_hitbox = &hitbox;
    SDL_RenderFillRect(game_renderer, wall_hitbox);
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void Wall::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

Wall::~Wall()
{

}