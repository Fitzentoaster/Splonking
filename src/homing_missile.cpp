//******************************************************************************
//  homing_missile.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "homing_missile.h"
#include "game_state.h"
#include "entity_handler.h"
#include "explosion.h"

#include <SDL.h>
#include <cmath>

#include <iostream>

using std::string;
using std::unique_ptr;
using std::shared_ptr;

Homing_Missile::Homing_Missile(const Entity_Data init) : Entity(init)
{
    set_angle(270);
}

void Homing_Missile::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    update_rotation(game_state, delta);

    double radians = (get_angle() * M_PI) / 180.0;

    double xspd = cos(radians) * 3;
    double yspd = sin(radians) * -3;

    set_x_speed(xspd);
    set_y_speed(yspd);

    update_position(delta);
}

void Homing_Missile::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            game_state->aud_handler->play_sfx("sfx_missile_explosion", AH_FIRST_AVAIL, AH_ONESHOT);
            game_state->aud_handler->play_sfx("sfx_player_death", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::OBSTACLE:
        case Col_Class::LASER:
        case Col_Class::SHIELD:
            game_state->aud_handler->play_sfx("sfx_missile_explosion", AH_FIRST_AVAIL, AH_ONESHOT);
            die(game_state);
            break;
        case Col_Class::BULLET:
        case Col_Class::BOUNCER:
        case Col_Class::ENEMY:
        case Col_Class::MISSILE:
            game_state->aud_handler->play_sfx("sfx_missile_explosion", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            die(game_state);
            break;
        default:
            break;
    }
}

void Homing_Missile::update_rotation(unique_ptr<Game_State>& game_state, const int delta)
{
    int player_x, player_y;
    double delta_factor = static_cast<double>(delta) / MS_WAIT;
    
    player_x = game_state->current_player->get_x_origin() + game_state->current_player->get_width() / 2; 
    player_y = game_state->current_player->get_y_origin() + game_state->current_player->get_height() / 2;
    
    int missile_x = get_x_origin() + get_width() / 2;
    int missile_y = get_y_origin() + get_height() / 2;

    double angle_to_player = atan2(-(player_y - missile_y), (player_x - missile_x));

    angle_to_player *= (180 / M_PI);
    angle_to_player += (angle_to_player < 0) ? 360 : 0;
    double diff = (angle_to_player - get_angle());
    while (diff < 0) {diff += 360;}
    if (round(get_angle()) != round(angle_to_player))
    {
        if (diff < 180) {set_angle(get_angle() + delta_factor * 0.75);}
        else {set_angle(get_angle() - delta_factor * 0.75);}
    }


}

void Homing_Missile::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_homing_missile");

    SDL_RenderCopyEx(game_renderer, display_texture, NULL, &hitbox, 360 - get_angle(), NULL, SDL_FLIP_NONE);
}

void Homing_Missile::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->aud_handler->play_sfx("sfx_missile_explosion", AH_FIRST_AVAIL, AH_ONESHOT);
    game_state->ent_handler->spawn_new<Explosion>(game_state, populate_explosion_init(game_state));
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::HOMING_MISSILE);
}

Entity_Data Homing_Missile::populate_explosion_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data new_explosion_init;

    new_explosion_init.id = game_state->current_id_pos++;
    new_explosion_init.ent_type = Entity_Type::EXPLOSION;
    new_explosion_init.col = Col_Class::NONE;
    new_explosion_init.cwd = game_state->cwd;
    new_explosion_init.x = get_x_origin();
    new_explosion_init.y = get_y_origin();
    new_explosion_init.xspd = 0;
    new_explosion_init.yspd = 0;
    new_explosion_init.hgt = get_height();
    new_explosion_init.wid = get_width();
    new_explosion_init.move = true;
    new_explosion_init.exist = true;
    new_explosion_init.tied = true;
    return new_explosion_init;
}


Homing_Missile::~Homing_Missile()
{
    
}