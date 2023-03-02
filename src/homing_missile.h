//******************************************************************************
//  homing_missile.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef HOMING_MISSILE_H
#define HOMING_MISSILE_H

#include "entity.h"
#include "constants.h"
#include <memory>

class SDL_Renderer;
class SDL_Surface;
class Entity_Handler;
struct Game_State;

class Homing_Missile : public Entity
{
    private:  
        std::string path;

    public:
        Homing_Missile(const Entity_Data);
        ~Homing_Missile();
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        Entity_Data populate_explosion_init(std::unique_ptr<Game_State>& game_state);

        void update_rotation(std::unique_ptr<Game_State>&, const int);
};

#endif
