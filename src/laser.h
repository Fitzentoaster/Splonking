//******************************************************************************
//  laser.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef LASER_H
#define LASER_H

#include "entity.h"
#include "constants.h"
#include "datastructs.h"
#include <vector>
#include <memory>

class SDL_Renderer;
class SDL_Surface;
class Entity_Handler;
struct Game_State;

class Laser : public Entity
{
    private:
        int ticks;

    public:
        Laser(const Entity_Data);
        ~Laser();

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
};

#endif
