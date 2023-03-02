//******************************************************************************
//  moonrock.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "moonrock.h"
#include "game_state.h"
#include "entity_handler.h"
#include "explosion.h"

#include <SDL.h>

using std::string;
using std::unique_ptr;

Moonrock::Moonrock(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    set_angle(STARTING_ANGLE);
    rotation_speed = (rand() % 200) / 200.0;
    type = rand() % 3;
}

int Moonrock::get_rotation_speed() const {return rotation_speed;}

void Moonrock::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{

    update_position(delta);

    set_angle((get_angle() + rotation_speed >= 360) ? 0 : get_angle() + rotation_speed);

}

void Moonrock::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    SDL_Rect obstacle_hitbox = collidee->get_hitbox();
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            collidee->die(game_state);
            break;
        case Col_Class::BULLET:
            collidee->die(game_state);
            break;
        case Col_Class::BOUNCER:
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::ENEMY:
            collidee->die(game_state);
            break;
        case Col_Class::MISSILE:
            collidee->die(game_state);
            die(game_state);
            break;
        case Col_Class::LASER:
        case Col_Class::SHIELD:
            die(game_state);
            break;
        case Col_Class::OBSTACLE:
            if (check_edge_collision('U', &obstacle_hitbox))
            {
                jump_for_reflect('D');
                set_reflected_vertical(true);
            }
            else if (check_edge_collision('D', &obstacle_hitbox))
            {
                jump_for_reflect('U');
                set_reflected_vertical(true);
                if (get_y_pos() < HUD_HEIGHT)
                {
                    die(game_state);
                }
            }
            else if (check_edge_collision('L', &obstacle_hitbox))
            {
                jump_for_reflect('R');
                set_reflected_horizontal(true);
            }
            else if (check_edge_collision('R', &obstacle_hitbox))
            {
                jump_for_reflect('L');
                set_reflected_horizontal(true);
            }
            break;
        default:
            break;
    }
}

void Moonrock::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture;
    switch (type)
    {
        case 0:
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_moonrock");
            break;
        case 1:
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_moonrock_2");
            break;
        case 2:
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_moonrock_3");
            break;
        default:
            display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_moonrock");
            break;
    }
    SDL_RenderCopyEx(game_renderer, display_texture, NULL, &hitbox, 360 - get_angle(), NULL, SDL_FLIP_NONE);
    
}

void Moonrock::die(std::unique_ptr<Game_State>& game_state)
{
    set_exists(false);
    game_state->aud_handler->play_sfx("sfx_moonrock_death", AH_FIRST_AVAIL, AH_ONESHOT);
    game_state->ent_handler->spawn_new<Explosion>(game_state, populate_explosion_init(game_state));
    game_state->ent_handler->decrement_current_on_screen(Entity_Type::MOONROCK);
}

Entity_Data Moonrock::populate_explosion_init(unique_ptr<Game_State>& game_state)
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

Moonrock::~Moonrock()
{

}