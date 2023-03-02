//******************************************************************************
//  bullet.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef BULLET_H
#define BULLET_H

#include "entity.h"
#include "constants.h"
#include <vector>
#include <memory>

class SDL_Renderer;
class SDL_Surface;
class Entity_Handler;
struct Game_State;

class Bullet : public Entity
{
    private:
        bool friendly;
        std::string path;
        SDL_Surface* sprite_surface;

    public:
        Bullet(const Entity_Data, const bool);
        Bullet(const Entity_Data);
        ~Bullet();
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
};

#endif
