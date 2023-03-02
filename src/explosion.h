//******************************************************************************
//  explosion.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"

class Game_State;
class Entity_Handler;
class SDL_Renderer;
class SDL_Rect;

class Explosion : public Entity
{
    private:
        int ticks;
        int anim_frames_max;
        int anim_frame_cur;
        std::string path;
        SDL_Rect src_rect;
        
    public:
        Explosion(const Entity_Data);
        ~Explosion();
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
};

#endif