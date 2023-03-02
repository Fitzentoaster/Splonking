//******************************************************************************
//  selection_arrow.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef SELECTION_ARROW_H
#define SELECTION_ARROW_H

#include <memory>
#include "entity.h"
#include "constants.h"
#include "datastructs.h"

class Game_State;
class Entity_Handler;

class Main_Menu_Cursor : public Entity
{
    private:
        Main_Menu_Option current_selection;
        
    public:
        Main_Menu_Cursor(const Entity_Data);
        ~Main_Menu_Cursor();

        void move_up();
        void move_down();
        Main_Menu_Option get_current();

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void tick_process(std::unique_ptr<Game_State>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
};

#endif