//******************************************************************************
//  bg_gameover.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef BG_GAMEOVER_H
#define BG_GAMEOVER_H

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"
#include "datastructs.h"

class Game_State;
class Entity_Handler;

class BG_Gameover : public Entity
{
    private:
        std::string path;
        std::string pic_name;
        TTF_Font* plain_font;
        SDL_Color text_color = {255, 255, 255};
        
    public:
        BG_Gameover(const Entity_Data);
        ~BG_Gameover();
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void despawn(std::unique_ptr<Game_State>&);
        void die(std::unique_ptr<Game_State>&);
        
        void render_timer(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void fill_timer_from_ms(Timer&, uint64_t);
        void generate_timer_string(Timer&, std::string&);

};

#endif