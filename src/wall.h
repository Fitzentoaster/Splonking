//******************************************************************************
//  wall.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef WALL_H
#define WALL_H

#include <memory>
#include "constants.h"
#include "entity.h"

class Game_State;
class SDL_Renderer;
class Entity_Handler;

class Wall : public Entity
{
    private:
    public:
        Wall(const Entity_Data);
        ~Wall();

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif