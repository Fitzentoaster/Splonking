//******************************************************************************
//  laser_turret.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "laser_turret.h"
#include "game_state.h"
#include "entity_handler.h"
#include "datastructs.h"
#include "laser.h"

#include <SDL.h>

//debugging
#include <iostream>
using std::cerr;
using std::endl;
//enddebug

using std::string;
using std::unique_ptr;

Laser_Turret::Laser_Turret(const Entity_Data init) : Entity(init)
{
    ticks = rand() % (MS_PER_LASER) - 500;
    fire_left = (get_x_origin() > SCREEN_WIDTH / 2) ? true : false;
    laser_on = false;
    path = init.cwd;
}

void Laser_Turret::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    ticks += delta;
    if (ticks > MS_PER_LASER) 
    {
        toggle_laser();
        if (laser_on)
        {
            fire_laser(game_state); 
        }
        ticks = 0;
    }
}

void Laser_Turret::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    SDL_Rect border_hitbox = get_hitbox();
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            game_state->aud_handler->play_sfx("sfx_player_death", AH_FIRST_AVAIL, AH_ONESHOT);
            collidee->die(game_state);
            break;
        case Col_Class::BULLET:
            collidee->die(game_state);
            break;
        case Col_Class::BOUNCER:
        case Col_Class::FUEL_PICKUP:
        case Col_Class::SHIELD_PICKUP:
            if (collidee->check_edge_collision('U', &border_hitbox))
            {
                collidee->jump_for_reflect('D');
                collidee->set_reflected_vertical(true);
            }
            else if (collidee->check_edge_collision('D', &border_hitbox))
            {
                collidee->jump_for_reflect('U');
                collidee->set_reflected_vertical(true);
            }
            else if (collidee->check_edge_collision('L', &border_hitbox))
            {
                collidee->jump_for_reflect('R');
                collidee->set_reflected_horizontal(true);
            }
            else if (collidee->check_edge_collision('R', &border_hitbox))
            {
                collidee->jump_for_reflect('L');
                collidee->set_reflected_horizontal(true);
            }
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

void Laser_Turret::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture;
    if (fire_left)
    {
        if (ticks > (MS_PER_LASER / 4) * 3 && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_l_partial_three");
        }
        else if (ticks > (MS_PER_LASER / 4) * 2 && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_l_partial_two");
        }
        else if (ticks > (MS_PER_LASER / 4) && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_l_partial_one");
        }
        else if (ticks >= 0 && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_l");
        }
        else if (laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_l_full");
        }
    }
    else
    {
        if (ticks > (MS_PER_LASER / 4) * 3 && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_r_partial_three");
        }
        else if (ticks > (MS_PER_LASER / 4) * 2 && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_r_partial_two");
        }
        else if (ticks > (MS_PER_LASER / 4) && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_r_partial_one");
        }
        else if (ticks >= 0 && !laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_r");
        }
        else if (laser_on)
        {
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_laser_turret_r_full");
        }
    }

    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}

void Laser_Turret::fire_laser(unique_ptr<Game_State>& game_state)
{
    Entity_Data laser_init = populate_laser_init(game_state);
    laser_init.x = (fire_left) ? 0 : get_x_origin() + get_width();
    laser_init.y = get_y_origin() + get_height() / 2;
    laser_init.wid = (fire_left) ? get_x_origin() : SCREEN_WIDTH - get_x_origin() - get_width();
    game_state->aud_handler->play_sfx("sfx_laser_state", AH_FIRST_AVAIL, AH_ONESHOT);
    game_state->ent_handler->spawn_new<Laser>(game_state, laser_init);
}

Entity_Data Laser_Turret::populate_laser_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data new_init;
    new_init.id = game_state->current_id_pos++;
    new_init.z = 3;
    new_init.ent_type = Entity_Type::LASER;
    new_init.col = Col_Class::LASER;
    new_init.cwd = game_state->cwd;
    new_init.x = 0;
    new_init.y = 0;
    new_init.xspd = 0;
    new_init.yspd = 0;
    new_init.hgt = 4;
    new_init.wid = 4;
    new_init.move = true;
    new_init.exist = true;
    new_init.tied = true;
    return new_init;
}

void Laser_Turret::toggle_laser() {laser_on = (laser_on) ? false : true;}

void Laser_Turret::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::LASER_TURRET);
}

Laser_Turret::~Laser_Turret()
{

}