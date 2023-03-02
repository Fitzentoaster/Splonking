//******************************************************************************
//  player.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include <cmath>
#include <string>
#include <memory>

#include "constants.h"
#include "entity.h"

class Entity_Handler;
struct Game_State;

class Player : public Entity
{
    private:
        int fuel = STARTING_FUEL;
        int ms_this_fuel = 0;
        bool thrust_active = false;
        std::string path;
        
    public:
        Player(const Entity_Data);
        ~Player();

        void set_thrust_active(const bool);
        void set_fuel(const int);
        void set_ms_this_fuel(const int);
        void decrement_fuel();

        int get_fuel() const;
        int get_ms_this_fuel() const;
        bool get_thrust_active() const;

        void rotate_left(const int);
        void rotate_right(const int);
        void rotate_analog(const int delta, const int axis);

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void spawn_shield(std::unique_ptr<Game_State>&);
        Entity_Data generate_shield_init(std::unique_ptr<Game_State>&);

        Entity_Data populate_explosion_init(std::unique_ptr<Game_State>&);

        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif