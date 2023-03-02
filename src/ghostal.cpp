//******************************************************************************
//  ghostal.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "ghostal.h"
#include "game_state.h"
#include "bullet.h"
#include "entity_handler.h"
#include "explosion.h"
#include "generate_init.h"
#include "freefunctions.h"

#include <SDL.h>

#include <iostream>

using std::string;
using std::unique_ptr;

Ghostal::Ghostal(const Entity_Data init) : Entity(init)
{
    ticks = 0;
    fire_right = (rand() % 2 == 0) ? true : false;
    fire_left = (rand() % 2 == 0) ? true : false;
    fire_up = (rand() % 2 == 0) ? true : false;
    fire_down = (rand() % 2 == 0) ? true : false;
    path = init.cwd;
}

void Ghostal::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    update_position(delta);
    ticks += delta;
    if (ticks > GHOSTAL_MOVE_DELAY)
    {
        set_is_moving((get_is_moving()) ? false : true);
        if (!get_is_moving())
        {
            set_x_speed(0);
            set_y_speed(0);
            fire_bullets(game_state);
        }
        else
        {
            set_x_speed((rand() % 6) - 3);
            set_y_speed((rand() % 6) - 3);
        }
        ticks = 0;
    }
}

void Ghostal::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::OBSTACLE: 
        case Col_Class::LASER:
        case Col_Class::BOUNCER:
        case Col_Class::SHIELD:
            die(game_state);
            break;
        case Col_Class::ENEMY:
        case Col_Class::BULLET:
            die(game_state);
            collidee->die(game_state);
            break;
        case Col_Class::MISSILE:
            game_state->ent_handler->spawn_new<Explosion>(game_state, populate_explosion_init(game_state));
            collidee->die(game_state);
            die(game_state);
            break;
        default:
            break;
    }
}

void Ghostal::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_ghostal");

    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}

Entity_Data Ghostal::populate_explosion_init(unique_ptr<Game_State>& game_state)
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

void Ghostal::fire_bullets(unique_ptr<Game_State>& game_state)
{
    bool fired = false;
    if (fire_right) 
    {
        fire_single_bullet(game_state, 'R'); 
        fired = true;
    }
    if (fire_left) 
    {
        fire_single_bullet(game_state, 'L'); 
        fired = true;
    }
    if (fire_up)
    {
        fire_single_bullet(game_state, 'U');
        fired = true;
    }
    if (fire_down) 
    {
        fire_single_bullet(game_state, 'D'); 
        fired = true;
    }
    if (fired) 
    {
        game_state->aud_handler->play_sfx("sfx_enemy_shoot", AH_FIRST_AVAIL, 0);
    }
}

void Ghostal::fire_single_bullet(unique_ptr<Game_State>& game_state, const char dir)
{
    Entity_Data bullet_init = generate_bullet_init(game_state);
    switch (dir)
    {
        case 'U':
            bullet_init.xspd = 0;
            bullet_init.yspd = -4;
            bullet_init.x = get_x_origin() + get_width() / 2;
            bullet_init.y = get_y_origin() - (bullet_init.hgt + 4);
            game_state->ent_handler->spawn_new<Bullet>(game_state, bullet_init);
            break;
        case 'D':
            bullet_init.xspd = 0;
            bullet_init.yspd = 4;
            bullet_init.x = get_x_origin() + get_width() / 2;
            bullet_init.y = get_y_origin() + get_height() + (bullet_init.hgt + 4);
            game_state->ent_handler->spawn_new<Bullet>(game_state, bullet_init);
            break;
        case 'L':
            bullet_init.xspd = -4;
            bullet_init.yspd = 0;
            bullet_init.x = get_x_origin() - (bullet_init.wid + 4);
            bullet_init.y = get_y_origin() + get_height() / 2;
            bullet_init.tied = true;
            game_state->ent_handler->spawn_new<Bullet>(game_state, bullet_init);
            break;
        case 'R':
            bullet_init.xspd = 4;
            bullet_init.yspd = 0;
            bullet_init.x = get_x_origin() + get_width() + (bullet_init.wid + 4);
            bullet_init.y = get_y_origin() + get_height() / 2;
            bullet_init.tied = true;
            game_state->ent_handler->spawn_new<Bullet>(game_state, bullet_init);
            break;
    }
}

void Ghostal::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->aud_handler->play_sfx("sfx_enemy_death", AH_FIRST_AVAIL, AH_ONESHOT);
    game_state->ent_handler->spawn_new<Explosion>(game_state, populate_explosion_init(game_state));        
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::GHOSTAL);
}

Ghostal::~Ghostal()
{

}