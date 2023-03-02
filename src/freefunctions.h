//******************************************************************************
//  freefunctions.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef FREEFUNCTIONS_H
#define FREEFUNCTIONS_h

#include <memory>
#include <string>

class SDL_Renderer;
class Entity;
struct Game_State;
struct Entity_Data;

void log(std::string);
void log_SDL_Error(std::string);
void load_highscore(std::unique_ptr<Game_State>&);
void load_from_ini(std::unique_ptr<Game_State>&);
void title_screen_logic(std::unique_ptr<Game_State>&);
void intro_screen_logic(std::unique_ptr<Game_State>&, SDL_Renderer*);
void main_game_logic(std::unique_ptr<Game_State>&);
void pause_mode_logic(std::unique_ptr<Game_State>&);
void check_for_highscore(std::unique_ptr<Game_State>&);
void game_over_logic(std::unique_ptr<Game_State>&);
void restart_screen_logic(std::unique_ptr<Game_State>&);
void game_initialize(std::unique_ptr<Game_State>&);
void reset_game_state(std::unique_ptr<Game_State>&);
void close(std::unique_ptr<Game_State>&);
const char* get_path(std::string, std::string);
void frame_update(std::unique_ptr<Game_State>&, const int);
void frame_render(std::unique_ptr<Game_State>&, SDL_Renderer*);
void cleanup(std::unique_ptr<Game_State>&);
void tick_update(std::unique_ptr<Game_State>&, const int);
void in_game_polling(std::unique_ptr<Game_State>&, const int);
void attract_mode_polling(std::unique_ptr<Game_State>&);
void game_over_polling(std::unique_ptr<Game_State>&);
void pause_mode_polling(std::unique_ptr<Game_State>&);
void process_level_up(std::unique_ptr<Game_State>&, const int);
void process_depth(std::unique_ptr<Game_State>&);
void process_fuel(std::unique_ptr<Game_State>&, const int);
void randomize_target_color(std::unique_ptr<Game_State>&);
void randomize_music(std::unique_ptr<Game_State>&);
void update_wall_color(std::unique_ptr<Game_State>&);
void process_collision(std::unique_ptr<Game_State>&, std::shared_ptr<Entity>, std::shared_ptr<Entity>);
void render_scanlines(SDL_Renderer*, std::unique_ptr<Game_State>&);
void save_screenshot(std::unique_ptr<Game_State>&);
Entity_Data generate_init(std::unique_ptr<Game_State>&, const std::string);
template <class T> void check_for_spawn(std::unique_ptr<Game_State>&, Entity_Data init);

#endif