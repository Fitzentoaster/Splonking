//******************************************************************************
//  laser_turret.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef LASER_TURRET_H
#define LASER_TURRET_H

#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"
#include "datastructs.h"

class Game_State;
class Entity_Handler;
class SDL_Renderer;
class SDL_Surface;

class Laser_Turret : public Entity
{
    private:
        int ticks;
        bool fire_left;
        bool laser_on;
        std::string path;
        SDL_Surface* sprite_surface_l;
        SDL_Surface* sprite_surface_r;
        
    public:
        Laser_Turret(const Entity_Data);
        ~Laser_Turret();

        void fire_laser(std::unique_ptr<Game_State>&);
        Entity_Data populate_laser_init(std::unique_ptr<Game_State>&);
        void toggle_laser();

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
};

#endif