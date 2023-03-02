//******************************************************************************
//  bee.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef BEE_H
#define BEE_H

#include "entity.h"
#include "constants.h"
#include <memory>

class SDL_Renderer;
class SDL_Surface;
struct Game_State;

class Bee : public Entity
{
    private:  
        std::string path;
        bool go_left;
        
    public:
        Bee(const Entity_Data);
        ~Bee();
        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        void die(std::unique_ptr<Game_State>&);
        Entity_Data populate_explosion_init(std::unique_ptr<Game_State>&);
        void update_rotation(std::unique_ptr<Game_State>&, const int);
};

#endif
