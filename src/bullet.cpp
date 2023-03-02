//******************************************************************************
//  bullet.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "bullet.h"
#include "game_state.h"
#include "entity_handler.h"
#include "freefunctions.h"

#include <SDL.h>

using std::string;
using std::unique_ptr;

Bullet::Bullet(const Entity_Data init, const bool frnd) : Entity(init)
{
    path = init.cwd;
    friendly = frnd;
}

Bullet::Bullet(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    friendly = false;
}

void Bullet::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    update_position(delta);
}

void Bullet::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            collidee->die(game_state);
            break;
        case Col_Class::OBSTACLE:
        case Col_Class::BULLET:
        case Col_Class::LASER:
        case Col_Class::BOUNCER:
        case Col_Class::SHIELD:
            die(game_state);
            break;
        case Col_Class::ENEMY:
            collidee->die(game_state);
            break;
        case Col_Class::MISSILE:
            collidee->die(game_state);
            die(game_state);
            break;
        default:
            break;
    }
}

void Bullet::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hitbox = get_hitbox();
    SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("gfx_sprite_bullet");

    SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
}

void Bullet::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

Bullet::~Bullet()
{

}