//******************************************************************************
//  bomb_pickup.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "bomb_pickup.h"
#include "game_state.h"
#include "entity_handler.h"

#include <SDL.h>

using std::string;
using std::unique_ptr;

Bomb_Pickup::Bomb_Pickup(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
}

void Bomb_Pickup::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{

    update_position(delta);

}

void Bomb_Pickup::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    SDL_Rect collidee_hitbox = collidee->get_hitbox();
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            game_state->aud_handler->play_sfx("sfx_screen_clear", AH_FIRST_AVAIL, AH_ONESHOT);
            for (auto& ent: game_state->entities_on_screen)
            {
                if (ent->get_type() == Entity_Type::GHOSTAL ||
                    ent->get_type() == Entity_Type::MOONROCK ||
                    ent->get_type() == Entity_Type::PLATFORM ||
                    ent->get_type() == Entity_Type::HOMING_MISSILE ||
                    ent->get_type() == Entity_Type::BEE ||
                    ent->get_type() == Entity_Type::BULLET ||
                    ent->get_type() == Entity_Type::LASER ||
                    ent->get_type() == Entity_Type::LASER_TURRET)
                    {
                        ent->die(game_state);
                    }
            }
            die(game_state);
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

void Bomb_Pickup::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_bomb_pickup");

    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox); 
}

void Bomb_Pickup::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::BOMB_PICKUP);
}

Bomb_Pickup::~Bomb_Pickup()
{

}