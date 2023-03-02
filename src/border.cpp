//******************************************************************************
//  border.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "border.h"
#include "game_state.h"
#include "entity_handler.h"
#include <SDL.h>

using std::unique_ptr;

Border::Border(const Entity_Data init) : Entity(init) {}

void Border::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    
}

void Border::resolve_collision(std::unique_ptr<Game_State>& game_state, std::shared_ptr<Entity>& collidee)
{
    SDL_Rect border_hitbox = get_hitbox();
    switch (collidee->get_collision_class())
    {
        case Col_Class::PLAYER:
            collidee->die(game_state);
            break;
        case Col_Class::BULLET:
        case Col_Class::LASER:
            collidee->die(game_state);
            break;
        case Col_Class::BOUNCER:
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
            collidee->die(game_state);
            break;
        case Col_Class::MISSILE:
            collidee->die(game_state);
            break;
        default:
            break;
    }
}

void Border::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_SetRenderDrawColor(game_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect hitbox = get_hitbox();
    const SDL_Rect* border_hitbox = &hitbox;
    SDL_RenderFillRect(game_renderer, border_hitbox);
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void Border::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

Border::~Border()
{

}