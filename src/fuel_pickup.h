//******************************************************************************
//  fuel_pickup.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef FUEL_PICKUP_H
#define FUEL_PICKUP_H

#include <string>
#include <memory>

#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;
class SDL_Renderer;
class SDL_Surface;

class Fuel_Pickup : public Entity
{
    private:
        std::string path;
        
    public:
        Fuel_Pickup(const Entity_Data);
        ~Fuel_Pickup();
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif