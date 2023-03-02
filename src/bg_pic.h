//******************************************************************************
//  bg_pic.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef BG_PIC_H
#define BG_PIC_H

#include <vector>
#include <SDL.h>
#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;

class BG_Pic : public Entity
{
    private:
        std::string path;
        std::string pic_name;
        SDL_Rect source_rect;
        
    public:
        BG_Pic(const Entity_Data);
        ~BG_Pic();
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);

        void tick_process(std::unique_ptr<Game_State>&);
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void despawn(std::unique_ptr<Game_State>&);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif