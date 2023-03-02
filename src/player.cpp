//******************************************************************************
//  player.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "player.h"
#include "entity_handler.h"
#include "game_state.h"
#include "shield.h"
#include "explosion.h"

#include <iostream>

using std::vector;
using std::string;
using std::unique_ptr;

Player::Player(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    set_angle(STARTING_ANGLE);
    fuel = STARTING_FUEL;
}

void Player::set_thrust_active(const bool state) 
{
    thrust_active = state; 
    if(thrust_active)
    {
        thrust_active = (fuel > 0) ? true: false;
    }
    
}

void Player::set_fuel(const int fl) {fuel = fl; fuel = (fuel > STARTING_FUEL) ? 200 : fuel;}
void Player::decrement_fuel() {fuel = (fuel < 1) ? 0 : fuel - 1;}
void Player::set_ms_this_fuel(const int ms) {ms_this_fuel = ms;}

int Player::get_fuel() const {return fuel;}
bool Player::get_thrust_active() const {return thrust_active;}
int Player::get_ms_this_fuel() const {return ms_this_fuel;}

void Player::rotate_left(const int delta)
{
    double delta_factor = static_cast<double>(delta) / MS_WAIT;
    set_angle(get_angle() + ROTATION_CONST * delta_factor);
}

void Player::rotate_right(const int delta)
{
    double delta_factor = static_cast<double>(delta) / MS_WAIT;
    set_angle(get_angle() - ROTATION_CONST * delta_factor);

}

void Player::rotate_analog(const int delta, const int axis)
{
    double delta_factor = static_cast<double>(delta) / MS_WAIT;
    set_angle(get_angle() - (axis / (ANALOG_ROTATION_CONST)) * delta_factor);
}

void Player::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    if (get_is_moving())
    {
        double delta_factor = static_cast<double>(delta) / MS_WAIT;
        double x_accel = 0.0;
        double y_accel = 0.0;

        double radians = (static_cast<double>(get_angle() * M_PI) / 180.0);

        x_accel = cos(radians) * ACCEL_CONST * delta_factor;
        y_accel = sin(radians) * -ACCEL_CONST * delta_factor;

        double xspd = get_x_speed();
        double yspd = get_y_speed();

        if (thrust_active && fuel > 0)
        {
            xspd += x_accel;
            if (xspd > MAX_SPEED)
                xspd = MAX_SPEED;
            else if (xspd < -MAX_SPEED)
                xspd = -MAX_SPEED;
            yspd += y_accel;
            if (yspd > MAX_SPEED)
                yspd = MAX_SPEED;
            else if (yspd < -MAX_SPEED)
                yspd = -MAX_SPEED;    
        }

        xspd = (xspd < 0) ? xspd + (AIR_RESIST_CONST * delta_factor) : xspd - (AIR_RESIST_CONST * delta_factor);
        xspd = (abs(xspd) <= AIR_RESIST_CONST) ? 0 : xspd;
        yspd = (yspd + (GRAVITY_CONST * delta_factor) > MAX_SPEED) ? MAX_SPEED : yspd + (GRAVITY_CONST * delta_factor);

        set_x_speed(xspd);
        set_y_speed(yspd);
        update_position(delta);
    }
}

void Player::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::OBSTACLE: 
        case Col_Class::LASER:
        case Col_Class::BOUNCER:
        case Col_Class::ENEMY:
            die(game_state);
            break;
        case Col_Class::BULLET:
        case Col_Class::MISSILE:
            die(game_state);
            collidee->die(game_state);
            break;
        case Col_Class::FUEL_PICKUP:
            game_state->aud_handler->play_sfx("sfx_fuel_pickup", AH_FIRST_AVAIL, AH_ONESHOT);
            set_fuel(get_fuel() + FUEL_AMOUNT);
            collidee->die(game_state);
            break;
        case Col_Class::SHIELD_PICKUP:
            game_state->aud_handler->play_sfx("sfx_fuel_pickup", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            spawn_shield(game_state);
            break;
        case Col_Class::BOMB_PICKUP:
            for (auto& ent: game_state->entities_on_screen)
            {
                game_state->aud_handler->play_sfx("sfx_screen_clear", AH_FIRST_AVAIL, AH_ONESHOT);
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
            collidee->die(game_state);
            break;
        default:
            break;
    }
}

void Player::spawn_shield(std::unique_ptr<Game_State>& game_state)
{
    game_state->ent_handler->spawn_new<Shield>(game_state, generate_shield_init(game_state));
}

Entity_Data Player::generate_shield_init(std::unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.id = game_state->current_id_pos;
    init.cwd = game_state->cwd;
    init.ent_type = Entity_Type::SHIELD;
    init.col = Col_Class::SHIELD;
    init.x = get_x_origin();
    init.y = get_y_origin();
    init.angle = get_angle();
    init.hgt = 80;
    init.wid = 80;
    init.move = false;
    init.exist = true;
    init.tied = false;
    return init;
}

void Player::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr((thrust_active) ? "gfx_sprite_player_thrust" : "gfx_sprite_player_nothrust");

    SDL_RenderCopyEx(game_renderer, display_texture, NULL, &hitbox, 360 - get_angle(), NULL, SDL_FLIP_NONE);
 
}

Entity_Data Player::populate_explosion_init(unique_ptr<Game_State>& game_state)
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


void Player::die(unique_ptr<Game_State>& game_state)
{
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::PLAYER);
    game_state->aud_handler->play_sfx("sfx_player_death", AH_FIRST_AVAIL, AH_ONESHOT);
    game_state->ent_handler->spawn_new<Explosion>(game_state, populate_explosion_init(game_state));
    set_exists(false);
}

Player::~Player()
{
 
}