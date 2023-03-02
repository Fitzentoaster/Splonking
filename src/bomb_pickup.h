//******************************************************************************
//  bomb_pickup.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef BOMB_PICKUP_H
#define BOMB_PICKUP_H

#include <string>
#include <memory>

#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;
class SDL_Renderer;
class SDL_Surface;

class Bomb_Pickup : public Entity
{
    private:
        std::string path;
        
    public:
        Bomb_Pickup(const Entity_Data);
        ~Bomb_Pickup();

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void spawn_shield(std::unique_ptr<Game_State>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif