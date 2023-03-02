//******************************************************************************
//  fuel_pickup.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "fuel_pickup.h"
#include "game_state.h"
#include "entity_handler.h"

#include <SDL.h>

using std::string;
using std::unique_ptr;
const string FUEL_PICKUP_SPRITE_GFX = "assets\\graphics\\sprite_fuel_pickup.bmp";

Fuel_Pickup::Fuel_Pickup(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
}

void Fuel_Pickup::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    update_position(delta);
}

void Fuel_Pickup::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    SDL_Rect collidee_hitbox = collidee->get_hitbox();
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            game_state->aud_handler->play_sfx("sfx_fuel_pickup", AH_FIRST_AVAIL, AH_ONESHOT);
            die(game_state);
            game_state->current_player->set_fuel(game_state->current_player->get_fuel() + FUEL_AMOUNT);
            break;
        case Col_Class::OBSTACLE:
            if (check_edge_collision('U', &collidee_hitbox))
            {
                jump_for_reflect('D');
                set_reflected_vertical(true);
            }
            else if (check_edge_collision('D', &collidee_hitbox))
            {
                jump_for_reflect('U');
                set_reflected_vertical(true);
            }
            else if (check_edge_collision('L', &collidee_hitbox))
            {
                jump_for_reflect('R');
                set_reflected_horizontal(true);
            }
            else if (check_edge_collision('R', &collidee_hitbox))
            {
                jump_for_reflect('L');
                set_reflected_horizontal(true);
            }
            break;
        case Col_Class::BULLET:
        case Col_Class::LASER:
        case Col_Class::BOUNCER:
        case Col_Class::ENEMY:
        case Col_Class::MISSILE:
            break;
        default:
            break;
    }
}

void Fuel_Pickup::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_fuel_pickup");

    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}

void Fuel_Pickup::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::FUEL_PICKUP);
}

Fuel_Pickup::~Fuel_Pickup()
{

}