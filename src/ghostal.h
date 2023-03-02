//******************************************************************************
//  ghostal.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef GHOSTAL_H
#define GHOSTAL_H

#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;
class SDL_Renderer;
class SDL_Surface;

class Ghostal : public Entity
{
    private:
        int ticks;
        bool fire_right;
        bool fire_left;
        bool fire_up;
        bool fire_down;
        std::string path;
        SDL_Surface* sprite_surface;
        Entity_Data bullet_init;
        
    public:
        Ghostal(const Entity_Data);
        ~Ghostal();
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        Entity_Data populate_explosion_init(std::unique_ptr<Game_State>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        Entity_Data populate_bullet_init(std::unique_ptr<Game_State>&);
        void fire_bullets(std::unique_ptr<Game_State>&);
        void fire_single_bullet(std::unique_ptr<Game_State>&, const char dir);
        void die(std::unique_ptr<Game_State>&);
};

#endif