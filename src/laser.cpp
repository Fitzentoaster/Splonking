//******************************************************************************
//  laser.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "laser.h"
#include "game_state.h"
#include "entity_handler.h"

#include <SDL.h>

using std::unique_ptr;
using std::string;

Laser::Laser(const Entity_Data init) : Entity(init)
{
    ticks = 0;
}

void Laser::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    ticks += delta;
    if (ticks > MS_PER_LASER || get_y_origin() < HUD_HEIGHT)
    {
        die(game_state);
    }
    update_position(delta);
}

void Laser::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            game_state->aud_handler->play_sfx("sfx_player_death", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::BULLET:
            collidee->die(game_state);
            break;
        case Col_Class::BOUNCER:
            game_state->aud_handler->play_sfx("sfx_moonrock_death", AH_FIRST_AVAIL, AH_ONESHOT);
            break;
        case Col_Class::ENEMY:
            game_state->aud_handler->play_sfx("sfx_enemy_death", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            break;
        case Col_Class::MISSILE:
            game_state->aud_handler->play_sfx("sfx_missile_explosion", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            break;
        default:
            break;
    }
}

void Laser::render(std::unique_ptr<Game_State>&, SDL_Renderer* game_renderer)
{
    SDL_SetRenderDrawColor(game_renderer, 200, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_Rect hitbox = get_hitbox();
    SDL_RenderFillRect(game_renderer, &hitbox);
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void Laser::die(unique_ptr<Game_State>& ent_handler)
{
    set_exists(false);
}

Laser::~Laser()
{

}