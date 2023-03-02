//******************************************************************************
//  shield.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef SHIELD_H
#define SHIELD_H

#include <vector>
#include <SDL.h>
#include <string>
#include <memory>

#include "constants.h"
#include "entity.h"

class Entity_Handler;
struct Game_State;

class Shield : public Entity
{
    private:
        int ms_left;
        std::string path;
        bool is_flashing;
        
    public:
        Shield(const Entity_Data);
        ~Shield();

        void decrement_ms_left(const int);

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif