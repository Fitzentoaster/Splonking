//******************************************************************************
//  entity_handler.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef ENTITY_HANDLER_H
#define ENTITY_HANDLER_H

#include <string>
#include <map>
#include <memory>

#include <iostream>

#include "constants.h"
#include "entity.h"
#include "datastructs.h"

struct Game_State;

struct Entity_Stats
{
    int current_ticks;
    int max_ticks;
    int current_on_screen;
    int max_on_screen;
    int min_difficulty;
};

class Entity_Handler
{
    private:
        std::map<Entity_Type, Entity_Stats> tick_map;
        std::vector<std::shared_ptr<Entity>> spawn_buffer;

    public:
        Entity_Handler();
        void add_type(Entity_Type, const int, const int, const int);
        std::map<Entity_Type, Entity_Stats> get_map();
        int get_current_ticks(Entity_Type);
        int get_max_ticks(Entity_Type);
        int get_current_on_screen(Entity_Type);
        int get_max_on_screen(Entity_Type);
        int get_min_difficulty(Entity_Type);

        void increment_current_on_screen(Entity_Type);
        void decrement_current_on_screen(Entity_Type);
        void increment_max_on_screen(Entity_Type);
        void decrement_max_on_screen(Entity_Type);
        void set_max_on_screen(Entity_Type, const int);
        void increment_max_ticks(Entity_Type);
        void decrement_max_ticks(Entity_Type);
        void increment_ticks(Entity_Type);
        void decrement_ticks(Entity_Type);
        void reset_ticks(Entity_Type);
        void ticks_minus();
        void clear_buffer(std::unique_ptr<Game_State>&);
        void reset_all();

        template <class T>
        void spawn_new(std::unique_ptr<Game_State>& game_state, Entity_Data init)
        {
            std::shared_ptr<T> ptr(new T(init));
            spawn_buffer.push_back(std::move(ptr));
        }
};

#endif