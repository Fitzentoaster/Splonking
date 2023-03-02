//******************************************************************************
//  hud.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "hud.h"
#include "game_state.h"
#include "entity_handler.h"

#include <string>
#include <iostream>
#include <cmath>

using std::string;
using std::to_string;
using std::unique_ptr;

const string PLAIN_FONT = "assets\\fonts\\coolvetica.ttf";
const string LOGO_FONT = "assets\\fonts\\sca.ttf";

HUD::HUD(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    plain_font = TTF_OpenFont((path + PLAIN_FONT).c_str(), 96);
    level = STARTING_DIFFICULTY_LEVEL;
    fuel = STARTING_FUEL;
    depth = 0;
    max_depth = 1000;
    logo_font = TTF_OpenFont((path + LOGO_FONT).c_str(), 96);
}

void HUD::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{
    level = game_state->current_level->get_difficulty_level();
    fuel = game_state->current_player->get_fuel();
    depth = game_state->depth;
    max_depth = game_state->max_depth;
}

void HUD::render(std::unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    SDL_Rect hud_rect = {0, 0, SCREEN_WIDTH, HUD_HEIGHT};
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(game_renderer, &hud_rect);

    SDL_SetRenderDrawColor(game_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_RenderDrawLine(game_renderer, 250, 0, 250, HUD_HEIGHT);
    SDL_RenderDrawLine(game_renderer, 1350, 0, 1350, HUD_HEIGHT);
    SDL_RenderDrawLine(game_renderer, 525, 0, 525, HUD_HEIGHT);
    SDL_RenderDrawLine(game_renderer, 1075, 0, 1075, HUD_HEIGHT);

    SDL_RenderDrawLine(game_renderer, 0, HUD_HEIGHT, SCREEN_WIDTH, HUD_HEIGHT);
    SDL_RenderDrawLine(game_renderer, 0, SCREEN_HEIGHT-1, SCREEN_WIDTH, SCREEN_HEIGHT-1);
    SDL_RenderDrawLine(game_renderer, 0, 0, 0, SCREEN_HEIGHT-1);
    SDL_RenderDrawLine(game_renderer, SCREEN_WIDTH-1, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);
    SDL_RenderDrawLine(game_renderer, 0, 0, SCREEN_WIDTH-1, 0);
    render_depth(game_renderer);
    render_max_depth(game_renderer);
    render_logo(game_renderer);
    render_fuel(game_renderer);
    render_level(game_renderer);
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
   
}
 
void HUD::render_depth(SDL_Renderer* game_renderer)
{
    string depth_string = "Depth: " + to_string(static_cast<int>(round_double(depth))) + "m";
    SDL_Rect depth_string_rect;
    SDL_Surface* depth_string_surface = TTF_RenderText_Solid(plain_font, depth_string.c_str(), text_color);
    SDL_Texture* depth_string_texture = SDL_CreateTextureFromSurface(game_renderer, depth_string_surface);
    depth_string_rect.x = 30;
    depth_string_rect.y = 5;
    depth_string_rect.w = 192;
    depth_string_rect.h = 96;
    SDL_RenderCopy(game_renderer, depth_string_texture, NULL, &depth_string_rect);
    SDL_DestroyTexture(depth_string_texture);
    SDL_FreeSurface(depth_string_surface);
}

void HUD::render_max_depth(SDL_Renderer* game_renderer)
{
    string max_string = (depth > max_depth) ? "Best: " + to_string(static_cast<int>(depth)) : "Best: " + to_string(static_cast<int>(max_depth));
    max_string += "m";
    SDL_Rect max_string_rect;
    SDL_Surface* max_string_surface = TTF_RenderText_Solid(plain_font, max_string.c_str(), text_color);
    SDL_Texture* max_string_texture = SDL_CreateTextureFromSurface(game_renderer, max_string_surface);
    max_string_rect.x = 280;
    max_string_rect.y = 5;
    max_string_rect.w = 192;
    max_string_rect.h = 96;
    SDL_RenderCopy(game_renderer, max_string_texture, NULL, &max_string_rect);
    SDL_DestroyTexture(max_string_texture);
    SDL_FreeSurface(max_string_surface);
}

void HUD::render_level(SDL_Renderer* game_renderer)
{
    string level_string = "Level: " + to_string(level);
    SDL_Rect level_string_rect;
    SDL_Surface* level_string_surface = TTF_RenderText_Solid(plain_font, level_string.c_str(), text_color);
    SDL_Texture* level_string_texture = SDL_CreateTextureFromSurface(game_renderer, level_string_surface);
    level_string_rect.x = SCREEN_WIDTH - 230;
    level_string_rect.y = 5;
    level_string_rect.w = 192;
    level_string_rect.h = 96;
    SDL_RenderCopy(game_renderer, level_string_texture, NULL, &level_string_rect);
    SDL_DestroyTexture(level_string_texture);
    SDL_FreeSurface(level_string_surface);
}

void HUD::render_logo(SDL_Renderer* game_renderer)
{
    string logo_string = "SPLONKING";
    SDL_Rect logo_string_rect;
    SDL_Surface* logo_string_surface = TTF_RenderText_Solid(logo_font, logo_string.c_str(), text_color);
    SDL_Texture* logo_string_texture = SDL_CreateTextureFromSurface(game_renderer, logo_string_surface);
    logo_string_rect.x = 550;
    logo_string_rect.y = 5;
    logo_string_rect.w = 500;
    logo_string_rect.h = 96;
    SDL_RenderCopy(game_renderer, logo_string_texture, NULL, &logo_string_rect);
    SDL_DestroyTexture(logo_string_texture);
    SDL_FreeSurface(logo_string_surface);
}

void HUD::render_fuel(SDL_Renderer* game_renderer)
{
    string fuel_string = "Fuel: " + to_string(fuel);
    SDL_Rect fuel_string_rect;
    SDL_Surface* fuel_string_surface = TTF_RenderText_Solid(plain_font, fuel_string.c_str(), text_color);
    SDL_Texture* fuel_string_texture = SDL_CreateTextureFromSurface(game_renderer, fuel_string_surface);
    fuel_string_rect.x = 1105;
    fuel_string_rect.y = 5;
    fuel_string_rect.w = 192;
    fuel_string_rect.h = 96;
    SDL_RenderCopy(game_renderer, fuel_string_texture, NULL, &fuel_string_rect);
    SDL_DestroyTexture(fuel_string_texture);
    SDL_FreeSurface(fuel_string_surface);
}

double HUD::round_double(const double input)
{
    return static_cast<double>(static_cast<int>(input * 100 + 0.5) / 100);
}

void HUD::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

HUD::~HUD() 
{
    TTF_CloseFont(plain_font);
    TTF_CloseFont(logo_font);
}
