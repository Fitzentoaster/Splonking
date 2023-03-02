//******************************************************************************
//  bg_pause.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef BG_PAUSE_H
#define BG_PAUSE_H

#include <vector>
#include <SDL.h>
#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;

class BG_Pause : public Entity
{
    private:
        std::string path;
        std::string pic_name;
        
    public:
        BG_Pause(const Entity_Data);
        ~BG_Pause();
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void despawn(std::unique_ptr<Game_State>&);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif