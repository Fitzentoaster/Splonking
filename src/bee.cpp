//******************************************************************************
//  bee.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "bee.h"
#include "game_state.h"
#include "entity_handler.h"
#include "explosion.h"

#include <SDL.h>
#include <cmath>

#include <iostream>

using std::string;
using std::unique_ptr;
using std::shared_ptr;

Bee::Bee(const Entity_Data init) : Entity(init)
{
    set_angle(270);
    go_left = (get_x_origin() > SCREEN_WIDTH / 2) ? true : false;
}

void Bee::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    double delta_factor = static_cast<double>(delta) / MS_WAIT;

    update_rotation(game_state, delta);

    double radians = (get_angle() * M_PI) / 180.0;

    double xspd = cos(radians) * 3.0;
    double yspd = sin(radians) * -3.0;

    set_x_speed(xspd);
    set_y_speed(yspd);
    if (go_left)
    {
        set_x_pos(get_x_pos() - (3.0 * delta_factor));
    }
    else
    {
        set_x_pos(get_x_pos() + (3.0 * delta_factor));
    }
    set_x_origin(round(get_x_pos()));
    set_y_pos(get_y_pos() + (3.0 * delta_factor));
    set_y_origin(round(get_y_pos()));

    update_position(delta);

}

void Bee::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::OBSTACLE:
        case Col_Class::BOUNCER:
        case Col_Class::LASER:
        case Col_Class::SHIELD:
            die(game_state);
            break;
        case Col_Class::ENEMY:
        case Col_Class::BULLET:
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::MISSILE:
            collidee->die(game_state);
            die(game_state);
            break;
        default:
            break;
    }
}

void Bee::update_rotation(unique_ptr<Game_State>& game_state, const int delta)
{
    set_angle(get_angle() + 3);
}

void Bee::render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture;
    if (go_left)
    {
        display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_bee_l");
    }
    else
    {
        display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_bee_r");
    }
    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}

Entity_Data Bee::populate_explosion_init(unique_ptr<Game_State>& game_state)
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

void Bee::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->aud_handler->play_sfx("sfx_enemy_death", AH_FIRST_AVAIL, AH_ONESHOT);
    game_state->ent_handler->spawn_new<Explosion>(game_state, populate_explosion_init(game_state));
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::BEE);
}

Bee::~Bee()
{
    
}