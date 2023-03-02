//******************************************************************************
//  entity.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "entity.h"
#include "game_state.h"
#include "entity_handler.h"

#include <cmath>

using std::string;

Entity::Entity(const Entity_Data initializer)
{
    id = initializer.id;
    type = initializer.ent_type;
    collision_class = initializer.col;
    z_index = initializer.z;
    x_origin = initializer.x;
    y_origin = initializer.y;
    x_speed = initializer.xspd;
    y_speed = initializer.yspd;
    x_adj_speed = initializer.xspd;
    y_adj_speed = initializer.yspd;
    angle = initializer.angle;
    color = initializer.color;
    height = initializer.hgt;
    width = initializer.wid;
    is_moving = initializer.move;
    exists = initializer.exist;
    tied_to_screen = initializer.tied;
    x_pos = x_origin;
    y_pos = y_origin;
    reflected_horizontal = false;
    reflected_vertical = false;
    init_hitboxes();
}

Entity::~Entity() {}

void Entity::init_hitboxes()
{
    hitbox.x = x_origin;
    hitbox.y = y_origin;
    hitbox.h = height;
    hitbox.w = width;

    left_hitbox.x = x_origin;
    left_hitbox.y = y_origin;
    left_hitbox.h = height;
    left_hitbox.w = GRID_WIDTH;

    right_hitbox.x = x_origin + width - GRID_WIDTH;
    right_hitbox.y = y_origin;
    right_hitbox.h = height;
    right_hitbox.w = GRID_WIDTH;

    top_hitbox.x = x_origin;
    top_hitbox.y = y_origin;
    top_hitbox.h = GRID_HEIGHT;
    top_hitbox.w = width;

    bottom_hitbox.x = x_origin;
    bottom_hitbox.y = y_origin + height - GRID_HEIGHT;
    bottom_hitbox.h = GRID_HEIGHT;
    bottom_hitbox.w = width;
}

int Entity::get_id() const {return id;}
int Entity::get_z_index() const {return z_index;}
int Entity::get_x_origin() const {return x_origin;}
int Entity::get_y_origin() const {return y_origin;}
int Entity::get_height() const {return height;}
int Entity::get_width() const {return width;}
bool Entity::get_reflected_horizontal() const {return reflected_horizontal;}
bool Entity::get_reflected_vertical() const {return reflected_vertical;}
bool Entity::get_is_moving() const {return is_moving;}
bool Entity::get_is_visible() const {return is_visible;}
bool Entity::get_exists() const {return exists;}
bool Entity::get_tied_to_screen() const {return tied_to_screen;}
Col_Class Entity::get_collision_class() const {return collision_class;}
RGB_Color Entity::get_color() const {return color;}
double Entity::get_x_speed() const {return x_speed;}
double Entity::get_y_speed() const {return y_speed;}
double Entity::get_x_pos() const {return x_pos;}
double Entity::get_y_pos() const {return y_pos;}
double Entity::get_angle() const {return angle;}
SDL_Rect Entity::get_hitbox() const {return hitbox;}
Entity_Type Entity::get_type() const {return type;}

void Entity::set_z_index(const int z) {z_index = z;}
void Entity::set_x_origin(const int x) {x_origin = x; init_hitboxes();}
void Entity::set_y_origin(const int y) {y_origin = y; init_hitboxes();}
void Entity::set_height(const int h) {height = h; init_hitboxes();}
void Entity::set_width(const int w) {width = w; init_hitboxes();}
void Entity::set_x_pos(const double x) {x_pos = x; x_origin = static_cast<int>(round(x)); init_hitboxes();}
void Entity::set_y_pos(const double y) {y_pos = y; y_origin = static_cast<int>(round(y)); init_hitboxes();}
void Entity::set_angle(const double ang) {angle = ang; angle += (angle < 0) ? 360: 0; angle -= (angle > 360) ? 360: 0;}
void Entity::set_reflected_horizontal(const bool rflect) {reflected_horizontal = rflect;}
void Entity::set_reflected_vertical(const bool rflect) {reflected_vertical = rflect;}
void Entity::set_is_moving(const bool move) {is_moving = move;}
void Entity::set_exists(const bool exist) {exists = exist;}
void Entity::set_tied_to_screen(const bool tied) {tied_to_screen = tied;}
void Entity::set_x_speed(const double xspd) {x_speed = xspd;}
void Entity::set_y_speed(const double yspd) {y_speed = yspd;}
void Entity::set_visibility(const bool vis) {is_visible = vis;}
void Entity::reflect_horizontal() {jump_for_reflect((x_speed < 0) ? 'R' : 'L'); x_speed *= -1; x_adj_speed *= -1;}
void Entity::reflect_vertical() {jump_for_reflect((y_speed < 0) ? 'D' : 'U'); y_speed *= -1; y_adj_speed *= -1;}
void Entity::resolve_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>&) {}
void Entity::toggle_move_state() {is_moving = (is_moving) ? false : true;}
void Entity::toggle_exists() {exists = (exists) ? false : true;}
void Entity::toggle_tied_to_screen() {tied_to_screen = (tied_to_screen) ? false : true;}
void Entity::toggle_visibility() {is_visible = (is_visible) ? false : true;}

void Entity::tick_process(std::unique_ptr<Game_State>& game_state)
{
    if (get_is_moving())
    {
        set_y_pos((get_tied_to_screen()) ? get_y_pos() - GRID_HEIGHT : get_y_pos());
    }
}

SDL_Rect Entity::get_side_hitbox(const char dir) const
{
    switch (dir) 
    {
        case 'U':
            return top_hitbox;
            break;
        case 'D':
            return bottom_hitbox;
            break;
        case 'L':
            return left_hitbox;
            break;
        case 'R':
            return right_hitbox;
            break;
    }
    return hitbox;
}

void Entity::jump_for_reflect(const char dir)
{
    switch (dir)
    {
        case 'U':
            y_pos -= abs(y_speed) + 1;
            y_origin = static_cast<int>(round(y_pos));
            break;
        case 'D':
            y_pos += abs(y_speed) + 1;
            y_origin = static_cast<int>(round(y_pos));
            break;
        case 'L':
            x_pos -= abs(x_speed) + 1;
            x_origin = static_cast<int>(round(x_pos));
            break;
        case 'R':
            x_pos += abs(x_speed) + 1;
            x_origin = static_cast<int>(round(x_pos));
            break;
    }
    init_hitboxes();
}

void Entity::update_position(const int delta)
{
    if (is_moving)
    {
        if (reflected_horizontal)
        {
            reflect_horizontal();
            reflected_horizontal = false;
        }
        if (reflected_vertical)
        {
            reflect_vertical();
            reflected_vertical = false;
        }

        x_pos += (get_x_speed() * (static_cast<double>(delta) / MS_WAIT));
        y_pos += (get_y_speed() * (static_cast<double>(delta) / MS_WAIT));
        
        x_origin = static_cast<int>(round(x_pos));
        y_origin = static_cast<int>(round(y_pos));
        init_hitboxes();
    }
}

bool Entity::check_collision(SDL_Rect* box2) const
{
    const SDL_Rect* box = &hitbox;
    return (SDL_HasIntersection(box, box2) == SDL_TRUE) ? true : false;
}

bool Entity::check_edge_collision(const char direction, SDL_Rect* box2) const
{
    SDL_Rect hitbox = get_side_hitbox(direction);
    const SDL_Rect* box = &hitbox;
    return (SDL_HasIntersection(box, box2) == SDL_TRUE) ? true : false;
}

bool Entity::is_oob() const
{
    if ((x_origin < -get_width()) || (x_origin + get_width() > SCREEN_WIDTH + 50) || (y_origin + get_height() < 0) || (y_origin > SCREEN_HEIGHT))
        return true;
    return false;
}

void Entity::despawn()
{
    set_exists(false);
}