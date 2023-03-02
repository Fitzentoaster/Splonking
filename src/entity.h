//******************************************************************************
//  entity.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <vector>
#include <cmath>
#include <string>
#include <memory>
#include "constants.h"
#include "datastructs.h"

class Entity_Handler;
struct Game_State;

struct Entity_Data
{
    int id;
    int z = 5;
    std::string cwd;
    Entity_Type ent_type;
    Col_Class col;
    RGB_Color color;
    double x = 0;
    double y = 0;
    double xspd = 0;
    double yspd = 0;
    double angle = 0;
    int hgt = 0;
    int wid = 0;
    bool move = true;
    bool exist = true;
    bool tied = false;
};

class Entity
{
    private:
        int id;
        int z_index;
        Entity_Type type;
        Col_Class collision_class;
        double x_pos;
        double y_pos;
        int x_origin;
        int y_origin;
        int height;
        int width;
        double angle;
        SDL_Rect hitbox;
        SDL_Rect left_hitbox;
        SDL_Rect right_hitbox;
        SDL_Rect top_hitbox;
        SDL_Rect bottom_hitbox;
        RGB_Color color;

        bool is_reflectable;
        bool reflected_horizontal;
        bool reflected_vertical;
        bool is_moving;
        bool is_visible;
        bool exists;
        bool tied_to_screen;
        double x_speed;
        double y_speed;
        double x_adj_speed;
        double y_adj_speed;

    public:
        Entity(const Entity_Data);
        void init_hitboxes();

        int get_id() const;
        int get_z_index() const;
        int get_x_origin() const;
        int get_y_origin() const;
        double get_x_pos() const;
        double get_y_pos() const;
        int get_height() const;
        int get_width() const;
        RGB_Color get_color() const;
        bool get_reflected_horizontal() const;
        bool get_reflected_vertical() const;
        bool get_is_moving() const;
        bool get_is_visible() const;
        bool get_exists() const;
        bool get_tied_to_screen() const;
        Col_Class get_collision_class() const;
        double get_x_speed() const;
        double get_y_speed() const;
        double get_angle() const;
        int get_ticks() const;
        SDL_Rect get_hitbox() const;
        SDL_Rect get_side_hitbox(const char) const;
        Entity_Type get_type() const;

        void set_z_index(const int);
        void set_x_origin(const int);
        void set_y_origin(const int);
        void set_x_pos(const double);
        void set_y_pos(const double);
        void set_height(const int);
        void set_width(const int);
        void set_reflected_horizontal(const bool);
        void set_reflected_vertical(const bool);
        void set_is_moving(const bool);
        void set_exists(const bool);
        void set_tied_to_screen(const bool);
        void set_x_speed(const double);
        void set_y_speed(const double);
        void set_angle(const double);
        void set_ticks(const int);
        void set_visibility(const bool);
        void toggle_move_state();
        void toggle_exists();
        void toggle_tied_to_screen();
        void toggle_visibility();
        void reset_ticks();
        void reflect_horizontal();
        void reflect_vertical();

        void jump_for_reflect(const char);

        void update_position(const int);
        bool check_collision(SDL_Rect*) const;
        bool check_edge_collision(const char, SDL_Rect*) const;
        bool is_oob() const;
        void despawn();
        virtual void frame_process(std::unique_ptr<Game_State>&, const int) = 0;
        virtual void tick_process(std::unique_ptr<Game_State>&);
        virtual void resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&);
        virtual void render(std::unique_ptr<Game_State>&, SDL_Renderer*) = 0;
        virtual void die(std::unique_ptr<Game_State>&) = 0;
        virtual ~Entity();

};

#endif