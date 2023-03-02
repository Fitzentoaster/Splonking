//******************************************************************************
//  entity_handler.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "entity_handler.h"
#include "game_state.h"

using std::string;
using std::unique_ptr;
using std::move;

Entity_Handler::Entity_Handler()
{
    add_type(Entity_Type::ATTRACT_SCREEN, 0, 1, 0);
    add_type(Entity_Type::BG_PIC, 0, 1, 0);
    add_type(Entity_Type::BG_PAUSE, 0, 1, 0);
    add_type(Entity_Type::BULLET, 0, 50, 0);
    add_type(Entity_Type::FUEL_PICKUP, TICKS_MIN_PER_FUEL, 2, 0);
    add_type(Entity_Type::GHOSTAL, TICKS_MIN_PER_GHOSTAL, 5, 3);
    add_type(Entity_Type::MOONROCK, TICKS_MIN_PER_MOONROCK, 1, 2);
    add_type(Entity_Type::PLATFORM, TICKS_MIN_PER_PLATFORM, 4, 1);
    add_type(Entity_Type::PLAYER, 0, 1, 0);
    add_type(Entity_Type::LASER_TURRET, TICKS_MIN_PER_TURRET, 3, 8);
    add_type(Entity_Type::HOMING_MISSILE, TICKS_MIN_PER_MISSILE, 2, 6);
    add_type(Entity_Type::SHIELD , 0, 1, 0);
    add_type(Entity_Type::SHIELD_PICKUP, TICKS_MIN_PER_SHIELD, 1, 1);
    add_type(Entity_Type::BOMB_PICKUP, TICKS_MIN_PER_BOMB, 1, 1);
    add_type(Entity_Type::BORDER, 0, 2, 0);
    add_type(Entity_Type::BEE, TICKS_MIN_PER_BEE, 1, 5);
}

void Entity_Handler::reset_all()
{
    for (auto const& type : tick_map)
    {
        reset_ticks(type.first);
        tick_map[type.first].current_on_screen = 0;
    }
    set_max_on_screen(Entity_Type::MOONROCK, 1);
}

void Entity_Handler::add_type(Entity_Type type, const int tick_max, const int screen_max, const int min_diff)
{
    Entity_Stats new_stats;
    new_stats.current_ticks = 0;
    new_stats.max_ticks = tick_max;
    new_stats.current_on_screen = 0;
    new_stats.max_on_screen = screen_max;
    new_stats.min_difficulty = min_diff;
    tick_map[type] = new_stats;
}

void Entity_Handler::clear_buffer(unique_ptr<Game_State>& game_state)
{
    for (auto& ent : spawn_buffer)
    {
        game_state->entities_on_screen.push_back(move(ent));
    }
    spawn_buffer.clear();
}

void Entity_Handler::increment_ticks(Entity_Type type)
{
    tick_map[type].current_ticks++;
}

std::map<Entity_Type, Entity_Stats> Entity_Handler::get_map()
{
    return tick_map;
}

int Entity_Handler::get_current_ticks(Entity_Type type)
{
    return tick_map[type].current_ticks;
}

int Entity_Handler::get_max_ticks(Entity_Type type)
{
    return tick_map[type].max_ticks;
}

int Entity_Handler::get_current_on_screen(Entity_Type type)
{
    return tick_map[type].current_on_screen;
}

int Entity_Handler::get_max_on_screen(Entity_Type type)
{
    return tick_map[type].max_on_screen;
}

int Entity_Handler::get_min_difficulty(Entity_Type type)
{
    return tick_map[type].min_difficulty;
}

void Entity_Handler::increment_current_on_screen(Entity_Type type)
{
    tick_map[type].current_on_screen++;
}

void Entity_Handler::decrement_current_on_screen(Entity_Type type)
{
    tick_map[type].current_on_screen--;
}
 
void Entity_Handler::reset_ticks(Entity_Type type)
{
    tick_map[type].current_ticks = 0;
}

void Entity_Handler::decrement_ticks(Entity_Type type)
{
    tick_map[type].current_ticks -= 30;
}

void Entity_Handler::ticks_minus()
{
    for (auto& type : tick_map)
    {
        decrement_ticks(type.first);
    }
}

void Entity_Handler::increment_max_on_screen(Entity_Type type)
{
    tick_map[type].max_on_screen++;
}

void Entity_Handler::decrement_max_on_screen(Entity_Type type)
{
    tick_map[type].max_on_screen--;
}

void Entity_Handler::set_max_on_screen(Entity_Type type, const int value)
{
    tick_map[type].max_on_screen = value;
}
void Entity_Handler::increment_max_ticks(Entity_Type type)
{
    tick_map[type].max_ticks += 15;
}

void Entity_Handler::decrement_max_ticks(Entity_Type type)
{
    tick_map[type].max_on_screen -= 15;
}
