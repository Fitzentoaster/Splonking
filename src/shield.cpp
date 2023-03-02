//******************************************************************************
//  shield.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "shield.h"
#include "entity_handler.h"
#include "game_state.h"
#include "player.h"

#include <iostream>

using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;

Shield::Shield(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    ms_left = MS_SHIELD;
    set_angle(STARTING_ANGLE);
    is_flashing = false;
    
}

void Shield::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    ms_left -= delta;
    int x = game_state->current_player->get_x_origin();
    int y = game_state->current_player->get_y_origin();
    int ang = game_state->current_player->get_angle();
    
    set_x_pos(x - 14);
    set_y_pos(y - 14);
    set_angle(ang);
    update_position(delta);
    if (ms_left < 0)
    {
        die(game_state);
    }
    if (ms_left < 3000 && ms_left % 4 == 0)
    {
        is_flashing = true;
    }
    else
    {
        is_flashing = false;
    }
}

void Shield::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::BULLET:
            collidee->die(game_state);
            break;
        case Col_Class::BOUNCER:
            game_state->aud_handler->play_sfx("sfx_moonrock_death", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
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

void Shield::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture;
    if (is_flashing)
    {
        display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_shield_flash");
    }
    else
    {
        display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_shield");
    }
    SDL_RenderCopyEx(game_renderer, display_texture, NULL, &hitbox, 360 - get_angle(), NULL, SDL_FLIP_NONE);
}

void Shield::die(unique_ptr<Game_State>& game_state)
{
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::SHIELD);
    set_exists(false);
}

Shield::~Shield()
{

}