//******************************************************************************
//  bg_gameover.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "bg_gameover.h"
#include "game_state.h"
#include "entity_handler.h"
#include "datastructs.h"
#include <sstream>
#include <iomanip>

using std::vector;
using std::string;
using std::unique_ptr;
using std::to_string;
using std::stringstream;
using std::setw;
using std::setfill;

const string PLAIN_FONT = "assets\\fonts\\coolvetica.ttf";


BG_Gameover::BG_Gameover(const Entity_Data init) : Entity(init)
{
    path = init.cwd;
    set_visibility(false);
    plain_font = TTF_OpenFont((path + PLAIN_FONT).c_str(), 96);

}

void BG_Gameover::render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    if (get_is_visible())
    {
        SDL_Rect hitbox = get_hitbox();
        SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_gameover");
        render_timer(game_state, game_renderer);
        SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
    }
}

void BG_Gameover::generate_timer_string(Timer& tmr, string& str)
{
    stringstream ss;
    ss << "In-Game Timer - " << setw(2) << setfill('0') << tmr.minutes << ":" << setw(2) << tmr.seconds << "." << setw(3) << tmr.ms;
    str = ss.str();

}

/**
 * Convert ms into a time data struct.
 * 
 * @param timer The timer struct to work with.
 * @param ms The milliseconds elapsed.
 * 
 ******************************************************************************/
void BG_Gameover::fill_timer_from_ms(Timer& tmr, uint64_t ms)
{
    tmr.ms = ms;

    tmr.seconds = tmr.ms / 1000;
    tmr.ms %= 1000;

    tmr.minutes = tmr.seconds / 60;
    tmr.seconds %= 60;
}

void BG_Gameover::render_timer(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    Timer tmr;
    fill_timer_from_ms(tmr, game_state->current_game_ms);
    string tmr_string;
    generate_timer_string(tmr, tmr_string);
    SDL_Rect tmr_string_rect;
    SDL_Surface* tmr_string_surface = TTF_RenderText_Solid(plain_font, tmr_string.c_str(), text_color);
    SDL_Texture* tmr_string_texture = SDL_CreateTextureFromSurface(game_renderer, tmr_string_surface);
    tmr_string_rect.x = SCREEN_WIDTH / 2 - 200;
    tmr_string_rect.y = 100;
    tmr_string_rect.w = 400;
    tmr_string_rect.h = 96;
    SDL_RenderCopy(game_renderer, tmr_string_texture, NULL, &tmr_string_rect);
    SDL_DestroyTexture(tmr_string_texture);
    SDL_FreeSurface(tmr_string_surface);
}

void BG_Gameover::frame_process(unique_ptr<Game_State>& game_state, const int delta)
{

}

void BG_Gameover::die(unique_ptr<Game_State>& game_state)
{
    set_exists(false);
}

BG_Gameover::~BG_Gameover()
{
    
}