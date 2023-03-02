//******************************************************************************
//  moonrock.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef MOONROCK_H
#define MOONROCK_H

#include <string>
#include <memory>
#include "constants.h"
#include "entity.h"

class Game_State;
class SDL_Renderer;
class SDL_Surface;

class Moonrock : public Entity
{
    private:
        int durability;
        double rotation_speed;
        std::string path;
        SDL_Surface* sprite_surface;
        uint8_t type;
        
    public:
        Moonrock(const Entity_Data);
        ~Moonrock();

        void set_durability(const int);
        void set_rotation_speed(const int);

        int get_rotation_speed() const;
        int get_durability() const;

        void frame_process(std::unique_ptr<Game_State>&, const int);
        void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        void render(std::unique_ptr<Game_State>&, SDL_Renderer*);
        Entity_Data populate_explosion_init(std::unique_ptr<Game_State>&);

        void die(std::unique_ptr<Game_State>&);
        
};

#endif