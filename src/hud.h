//******************************************************************************
//  hud.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef HUD_H
#define HUD_H

#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;
class SDL_Renderer;
class SDL_Surface;

class HUD : public Entity
{
    private:
        TTF_Font* plain_font;
        TTF_Font* logo_font;
        SDL_Color text_color = {255, 255, 255};
        int level, fuel;
        double depth, max_depth;
        std::string path;

    public:

        HUD(const Entity_Data);
        ~HUD();
        void render_depth(SDL_Renderer*);
        void render_max_depth(SDL_Renderer*);
        void render_level(SDL_Renderer*);
        void render_logo(SDL_Renderer*);
        void render_fuel(SDL_Renderer*);
        double round_double(const double);

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
};

#endif