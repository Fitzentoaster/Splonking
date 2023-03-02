//******************************************************************************
//  attractscreen.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef ATTRACTSCREEN_H
#define ATTRACTSCREEN_H

#include <vector>
#include <SDL.h>
#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"
#include "main_menu_cursor.h"

class Game_State;
class Entity_Handler;

class Attract_Screen : public Entity
{
    private:
        std::string path;
        
    public:
        Attract_Screen(const Entity_Data);
        ~Attract_Screen();
        void render(std::unique_ptr<Game_State>&, SDL_Renderer* game_renderer);

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void tick_process(std::unique_ptr<Game_State>&);
        Entity_Data generate_cursor_init();
        void despawn(std::unique_ptr<Game_State>&);
        void die(std::unique_ptr<Game_State>&);
        
};

#endif