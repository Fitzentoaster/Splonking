//******************************************************************************
//  level.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef LEVEL_H
#define LEVEL_H

#include "entity.h"
#include "datastructs.h"
#include "constants.h"
#include <array>
#include <vector>
#include <memory>
#include <SDL.h>

struct Game_State;
struct Entity_Data;

class Level
{
    private:
        int min_wall_width = STARTING_MIN_WALL_WIDTH;
        int max_wall_width = STARTING_MAX_WALL_WIDTH;
        int left_wall_coord = 2;
        int right_wall_coord = X_COORDS - 2;
        double depth_this_level = 0.0;
        int ms_this_level = 0;
        int difficulty_level = STARTING_DIFFICULTY_LEVEL;
        int tick_counter = 0;
        RGB_Color wall_color = {50, 50, 50};
        
    public:
        Level();
        bool get_solid(const int, const int);
        int get_red(const int, const int);
        int get_green(const int, const int);
        int get_blue(const int, const int);
        SDL_Rect get_render_rect(const int, const int);

        int get_min_wall_width() const;
        int get_max_wall_width() const;
        int get_left_wall_coord() const;
        int get_right_wall_coord() const;
        RGB_Color get_wall_color() const;
        int get_ms_this_level() const;
        int get_difficulty_level() const;
        int get_tick_counter() const;
        double get_depth_this_level() const;

        void set_min_wall_width(const int);
        void set_max_wall_width(const int);
        void set_left_wall_coord(const int);
        void set_right_wall_coord(const int);
        void set_wall_color(const RGB_Color);
        void set_ms_this_level(const int);
        void set_difficulty_level(const int);
        void set_tick_counter(const int);
        void set_depth_this_level(const double);
        void increment_depth_this_level();
        void reset_depth_this_level();

        void set_solid(const int, const int, bool);
        void new_walls(std::unique_ptr<Game_State>&);
        Entity_Data generate_wall_init(std::unique_ptr<Game_State>&, const int);

        
};

#endif