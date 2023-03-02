//******************************************************************************
//  level.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "level.h"
#include "moonrock.h"
#include "fuel_pickup.h"
#include "platform.h"
#include "game_state.h"
#include "wall.h"

#include <vector>
#include <iostream>

using std::vector;
using std::unique_ptr;

Level::Level()
{
   
}


int Level::get_min_wall_width() const {return min_wall_width;}
int Level::get_max_wall_width() const {return max_wall_width;}
int Level::get_left_wall_coord() const {return left_wall_coord;}
int Level::get_right_wall_coord() const {return right_wall_coord;}
int Level::get_ms_this_level() const {return ms_this_level;}
int Level::get_difficulty_level() const {return difficulty_level;}
int Level::get_tick_counter() const {return tick_counter;}
double Level::get_depth_this_level() const {return depth_this_level;}
RGB_Color Level::get_wall_color() const {return wall_color;}

void Level::set_min_wall_width(const int wid) {min_wall_width = wid;}
void Level::set_max_wall_width(const int wid) {max_wall_width = wid;}
void Level::set_left_wall_coord(const int left) {left_wall_coord = left;}
void Level::set_right_wall_coord(const int right) {right_wall_coord = right;}
void Level::set_ms_this_level(const int ms) {ms_this_level = ms;}
void Level::set_difficulty_level(const int lev) {difficulty_level = lev;}
void Level::set_tick_counter(const int tix) {tick_counter = tix;}
void Level::set_depth_this_level(const double dep) {depth_this_level = dep;}
void Level::increment_depth_this_level() {++depth_this_level;}
void Level::reset_depth_this_level() {depth_this_level = 0;}
void Level::set_wall_color(const RGB_Color col) {wall_color = col;}

SDL_Rect Level::get_render_rect(const int x, const int y)
{
    const SDL_Rect rect = {x*GRID_WIDTH, y*GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
    return rect;
}

void Level::new_walls(unique_ptr<Game_State>& game_state)
{
    int width = right_wall_coord - left_wall_coord;
    if ((rand() % 2) == 1)
        right_wall_coord++;
    else
        right_wall_coord--;

    if ((rand() % 2) == 1)
        left_wall_coord++;
    else
        left_wall_coord--;
    
    if (width > max_wall_width)
    {
        left_wall_coord++;
        right_wall_coord--;
    }

    if (width < min_wall_width)
    {
        left_wall_coord--;
        right_wall_coord++;
    }

    if (left_wall_coord < 1)
    {
        left_wall_coord = 2;
    }
    if (right_wall_coord > X_COORDS - 1)
    {
        right_wall_coord = X_COORDS - 2;
    }

    game_state->ent_handler->spawn_new<Wall>(game_state, generate_wall_init(game_state, left_wall_coord));
    game_state->ent_handler->spawn_new<Wall>(game_state, generate_wall_init(game_state, right_wall_coord));
}

Entity_Data Level::generate_wall_init(std::unique_ptr<Game_State>& game_state, const int wall_coord)
{
    Entity_Data init;
    int x_coord = wall_coord * GRID_WIDTH;
    if (x_coord < SCREEN_WIDTH / 2)
    {
        init.x = 0;
        init.wid = x_coord;
    }
    else
    {
        init.x = x_coord;
        init.wid = SCREEN_WIDTH - x_coord;
    }
    init.id = game_state->current_id_pos;
    init.z = 5;
    init.cwd = game_state->cwd;
    init.ent_type = Entity_Type::WALL;
    init.col = Col_Class::OBSTACLE;
    init.color = RGB_Color {game_state->wall_color->red, game_state->wall_color->green, game_state->wall_color->blue};
    init.y = SCREEN_HEIGHT - GRID_HEIGHT - 4;
    init.angle = 0;
    init.hgt = GRID_HEIGHT;
    init.move = true;
    init.exist = true;
    init.tied = true;
    return init;
}
