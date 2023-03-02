//******************************************************************************
//  freefunctions.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "constants.h"
#include "attractscreen.h"
#include "entity.h"
#include "player.h"
#include "game_state.h"
#include "level.h"
#include "audio_handler.h"
#include "fuel_pickup.h"
#include "laser_turret.h"
#include "bullet.h"
#include "ghostal.h"
#include "platform.h"
#include "moonrock.h"
#include "homing_missile.h"
#include "shield.h"
#include "shield_pickup.h"
#include "bomb_pickup.h"
#include "border.h"
#include "datastructs.h"
#include "hud.h"
#include "bee.h"
#include "bg_pic.h"
#include "bg_pause.h"
#include "bg_gameover.h"
#include "main_menu_cursor.h"
#include "generate_init.h"
#include "freefunctions.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_gamecontroller.h>
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <memory>
#include <fstream>

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::string;
using std::stringstream;
using std::fstream;
using std::ofstream;
using std::getline;

/**
 * Log the current time and a message, msg, to stderr.
 * 
 * @param msg A message to log to stderr.
 * 
 ******************************************************************************/
void log(string msg)
{
    std::time_t log_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cerr << std::ctime(&log_time) << ": " << msg << std::endl;
}

/**
 * Log the 'msg' string to stderr, and log the SDL_GetError() latest error message.
 * 
 * @param msg A message to log to stderr.
 * 
 ******************************************************************************/
void log_SDL_error(string msg)
{
    log(msg);
    log(SDL_GetError());
    SDL_ClearError();
}

/**
 * Convert ms into a time data struct.
 * 
 * @param timer The timer struct to work with.
 * @param ms The milliseconds elapsed.
 * 
 ******************************************************************************/
void fill_timer_from_ms(Timer& tmr, uint64_t ms)
{
    tmr.ms = ms;

    tmr.seconds = tmr.ms / 1000;
    tmr.ms %= 1000;

    tmr.minutes = tmr.seconds / 60;
    tmr.seconds %= 60;
}

/**
 * Check if current score is a new high score. If so, write it to the highscore file.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void check_for_highscore(unique_ptr<Game_State>& game_state)
{
    if (game_state->depth >= game_state->max_depth)
    {
        ofstream max_depth_file("data\\highscore");
        max_depth_file << game_state->depth;
        max_depth_file.close();
    }
}

/**
 * Saves screenshot to /screenshots folder with random filename.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void save_screenshot(unique_ptr<Game_State>& game_state)
{
    const Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    const int width = 1600;
    const int height = 900;
    auto renderer = game_state->game_renderer;
    int filenum = rand() % 99999999;
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format);
    SDL_RenderReadPixels(renderer, NULL, format, surface->pixels, surface->pitch);
    string filenamestring = "screenshots\\" + std::to_string(filenum);
    filenamestring += ".bmp";
    SDL_SaveBMP(surface, filenamestring.c_str());
    SDL_FreeSurface(surface);
}

/**
 * Load settings from .ini file
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void load_from_ini(unique_ptr<Game_State>& game_state)
{
    fstream ini_file(get_path(game_state->cwd, "splonking.ini"));
    string ini_file_buffer_string;
    string waste;

    getline(ini_file, ini_file_buffer_string);
    game_state->fullscreen = stoi(ini_file_buffer_string.substr(ini_file_buffer_string.find("=") + 2));
    getline(ini_file, ini_file_buffer_string);
    game_state->scanlines = stoi(ini_file_buffer_string.substr(ini_file_buffer_string.find("=") + 2));
    getline(ini_file, ini_file_buffer_string);
    game_state->mus_volume = stoi(ini_file_buffer_string.substr(ini_file_buffer_string.find("=") + 2));
    getline(ini_file, ini_file_buffer_string);
    game_state->sfx_volume = stoi(ini_file_buffer_string.substr(ini_file_buffer_string.find("=") + 2));

    ini_file.close();

}

/**
 * Free resources and uninitialize SDL and related modules.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void close(unique_ptr<Game_State>& game_state)
{
    SDL_RenderClear(game_state->game_renderer);
    SDL_DestroyRenderer(game_state->game_renderer);
    SDL_DestroyWindow(game_state->game_window);

    game_state->aud_handler->clear_all_sounds();
    game_state->gfx_handler->clear_all_graphics();
    Mix_Quit();
    Mix_CloseAudio();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/**
 * Main logic/loop for the game-over phase.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void game_over_logic(unique_ptr<Game_State>& game_state)
{
    if (game_state->game_phase != Phase::QUIT)
    {
        game_state->game_phase = Phase::GAME_OVER;
    }

    game_state->aud_handler->stop_music();
    game_state->aud_handler->switch_music("mus_gameover");
    game_state->aud_handler->start_music();

    int game_over_delay = 0;
    while (game_state->game_phase == Phase::GAME_OVER)
    {
        game_state->delta->prev_ms = SDL_GetTicks();
        SDL_SetRenderDrawColor(game_state->game_renderer, 0, 0, 0, 255);
        SDL_RenderClear(game_state->game_renderer);

        frame_update(game_state, game_state->delta->delta_ms);
                    
        frame_render(game_state, game_state->game_renderer);
        
        game_state->delta->current_ms = SDL_GetTicks();
        game_state->delta->delta_ms = game_state->delta->current_ms - game_state->delta->prev_ms;

        game_over_delay += game_state->delta->delta_ms;

        if (game_over_delay > 1500)
        {
            for (auto& ent : game_state->entities_on_screen)
            {
                if (ent->get_type() == Entity_Type::BG_GAMEOVER && ent->get_is_visible() == false)
                {
                    ent->toggle_visibility();
                }
            }
            SDL_PumpEvents();
            SDL_FlushEvent(SDL_KEYDOWN);
            SDL_FlushEvent(SDL_CONTROLLERBUTTONDOWN);
            game_state->game_phase = Phase::RESTART_SCREEN;
        }
    }    
}

/**
 * Logic for the restart screeen.
 * 
 * @param game_state The current game state.
 * 
 ******************************************************************************/
void restart_screen_logic(unique_ptr<Game_State>& game_state)
{
    while (game_state->game_phase == Phase::RESTART_SCREEN)
    {
        SDL_RenderClear(game_state->game_renderer);
        
        game_state->ent_handler->clear_buffer(game_state);
        
        frame_render(game_state, game_state->game_renderer);

        game_over_polling(game_state);
        
        SDL_Delay(MS_WAIT);
    }
}

/**
 * Main logic for the main game phase.
 * 
 * @param game_state The current game state.
 * 
 ******************************************************************************/
void main_game_logic(unique_ptr<Game_State>& game_state)
{
    
    if (game_state->game_phase == Phase::MAIN_GAME)
    {
        for (auto& ent : game_state->entities_on_screen)
        {
            ent->despawn();
        }
        game_state->aud_handler->init_volume(game_state);
        game_state->aud_handler->stop_music();
        randomize_music(game_state);
        game_state->aud_handler->start_music();
        game_state->aud_handler->play_sfx("sfx_splonking", AH_FIRST_AVAIL, 0);
        game_state->current_player = make_shared<Player>(generate_player_init(game_state));
        game_state->entities_on_screen.push_back(game_state->current_player);
        game_state->ent_handler->spawn_new<Border>(game_state, generate_top_border_init(game_state));
        game_state->ent_handler->spawn_new<Border>(game_state, generate_bottom_border_init(game_state));
        game_state->ent_handler->spawn_new<HUD>(game_state, generate_hud_init(game_state));
        game_state->ent_handler->spawn_new<BG_Pause>(game_state, generate_bg_pause_init(game_state));
        game_state->ent_handler->spawn_new<BG_Pic>(game_state, generate_bg_pic_init(game_state));
        game_state->ent_handler->spawn_new<BG_Gameover>(game_state, generate_bg_gameover_init(game_state));
        game_state->aud_handler->play_sfx("sfx_player_thrust", 1, AH_INFINITE_LOOP);
    }

    while (game_state->game_phase == Phase::MAIN_GAME)
    {
        in_game_polling(game_state, game_state->delta->delta_ms);
     
        while (game_state->is_paused)
        {
            pause_mode_logic(game_state);
        }

        if (!game_state->current_player->get_exists())
        {
            game_state->game_phase = Phase::GAME_OVER;
        }

        game_state->delta->prev_ms = SDL_GetTicks();

        process_level_up(game_state, game_state->delta->delta_ms);
        process_fuel(game_state, game_state->delta->delta_ms);

        game_state->current_game_ms += game_state->delta->delta_ms;

        SDL_SetRenderDrawColor(game_state->game_renderer, 0, 0, 0, 255);
        SDL_RenderClear(game_state->game_renderer);
        
        frame_update(game_state, game_state->delta->delta_ms);
        
        tick_update(game_state, game_state->delta->delta_ms);
        
        frame_render(game_state, game_state->game_renderer);
        
        game_state->delta->current_ms = SDL_GetTicks();
        game_state->delta->delta_ms = game_state->delta->current_ms - game_state->delta->prev_ms;
    }

    game_state->aud_handler->stop_sfx(1);
}

/**
 * Pause logic/loop.
 * 
 * @param game_state The current game_state.
 ******************************************************************************/
void pause_mode_logic(unique_ptr<Game_State>& game_state)
{

    game_state->aud_handler->pause_music();
    game_state->aud_handler->play_sfx("sfx_pause", AH_FIRST_AVAIL, 0);

    for (auto& ent : game_state->entities_on_screen)
    {
        if (ent->get_type() == Entity_Type::BG_PAUSE)
        {
            ent->toggle_visibility();
        }
    }

    while (game_state->is_paused)
    {
        SDL_RenderClear(game_state->game_renderer);
                
        game_state->ent_handler->clear_buffer(game_state);
        
        frame_render(game_state, game_state->game_renderer);
        
        pause_mode_polling(game_state);

        SDL_Delay(MS_WAIT);

    }    

    for (auto& ent : game_state->entities_on_screen)
    {
        if (ent->get_type() == Entity_Type::BG_PAUSE)
        {
            ent->toggle_visibility();
        }
    }
    game_state->aud_handler->play_sfx("sfx_unpause", AH_FIRST_AVAIL, 0);
    game_state->aud_handler->start_music();

}

/**
 * Main logic for the intro
 * 
 * @param game_state The current game_state.
 * @param game_renderer The SDL renderer pointer
 * 
 ******************************************************************************/
void intro_screen_logic(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    while(game_state->game_phase == Phase::INTRO)
    {
        SDL_Rect hitbox;
        hitbox.h = 900;
        hitbox.w = 1600;
        hitbox.x = 0;
        hitbox.y = 0;

        int opacity = 15;
        while (opacity < 255)
        {
            SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_intro_one");
            SDL_SetTextureAlphaMod(display_texture, opacity);
            SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
            SDL_RenderPresent(game_renderer);
            SDL_Delay(MS_WAIT);
            SDL_RenderClear(game_renderer);
            opacity += 4;
        }
        SDL_Delay(2000);
        while (opacity > 15)
        {
            SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_intro_one");
            SDL_SetTextureAlphaMod(display_texture, opacity);
            SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
            SDL_RenderPresent(game_renderer);
            SDL_Delay(MS_WAIT);
            SDL_RenderClear(game_renderer);
            opacity -= 4;
        }

        while (opacity < 255)
        {
            SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_intro_two");
            SDL_SetTextureAlphaMod(display_texture, opacity);
            SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
            SDL_RenderPresent(game_renderer);
            SDL_Delay(MS_WAIT);
            SDL_RenderClear(game_renderer);
            opacity += 4;
        }
        SDL_Delay(2000);
        while (opacity > 15)
        {
            SDL_Texture* display_texture = game_state->gfx_handler->get_texture_ptr("bg_intro_two");
            SDL_SetTextureAlphaMod(display_texture, opacity);
            SDL_RenderCopy(game_renderer, display_texture, NULL, &hitbox);
            SDL_RenderPresent(game_renderer);
            SDL_Delay(MS_WAIT);
            SDL_RenderClear(game_renderer);
            opacity -= 4;
        }
    game_state->game_phase = Phase::TITLE_SCREEN;

    }
}

/**
 * Main logic/loop for the title screen.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void title_screen_logic(unique_ptr<Game_State>& game_state)
{
    if (game_state->game_phase == Phase::TITLE_SCREEN)
    {
        game_state->aud_handler->switch_music("mus_title_screen");
        game_state->aud_handler->start_music();
        game_state->ent_handler->spawn_new<Attract_Screen>(game_state, generate_attract_screen_init(game_state));
        game_state->main_cursor = make_shared<Main_Menu_Cursor>(generate_main_menu_cursor_init(game_state));
        game_state->entities_on_screen.push_back(game_state->main_cursor);
        SDL_PumpEvents();
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_CONTROLLERBUTTONDOWN);
    }
    while (game_state->game_phase == Phase::TITLE_SCREEN)
    {
        SDL_RenderClear(game_state->game_renderer);
                
        game_state->ent_handler->clear_buffer(game_state);
        
        frame_render(game_state, game_state->game_renderer);
        
        attract_mode_polling(game_state);

        SDL_Delay(MS_WAIT);
    }    
}

/**
 * Open the file highscore and read the currently saved high score from it.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void load_highscore(unique_ptr<Game_State>& game_state)
{
    fstream max_depth_file(get_path(game_state->cwd, "data\\highscore"));
    string max_depth_buffer_string;
    getline(max_depth_file, max_depth_buffer_string);
    game_state->max_depth = strtol(max_depth_buffer_string.c_str(), nullptr, 10);
    max_depth_file.close();
}

/**
 * Init SDL functions and necessary game_state variables, to be run once at
 * game-start.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void game_initialize(unique_ptr<Game_State>& game_state)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        log_SDL_error("SDL_Init failed");
    }

    IMG_Init(IMG_INIT_PNG);
    
    game_state->game_window = SDL_CreateWindow("Splonking", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, 0);

    if (!game_state->game_window)
    {
        log_SDL_error("SDL_CreateWindow failed");
    }

    if (game_state->fullscreen)
    {
        if (SDL_SetWindowFullscreen(game_state->game_window, SDL_WINDOW_FULLSCREEN) < 0)
            {
                log_SDL_error("SDL_SetWindowFullscreen failed");
            }
    }

    game_state->game_renderer = SDL_CreateRenderer(game_state->game_window, -1, SDL_RENDERER_ACCELERATED);

    if (!game_state->game_renderer)
    {
        log_SDL_error("SDL_CreateRenderer failed");
    }

    if (SDL_RenderSetLogicalSize(game_state->game_renderer, 1600, 900) < 0)
    {
        log_SDL_error("SDL_RenderSetLogicalSize failed");
    }

    SDL_Surface* scanlines = IMG_Load((game_state->cwd + "assets\\graphics\\gfx_scanline_shader.png").c_str());

    if (!scanlines)
    {
        log_SDL_error("IMG_Load failed for scanlines");
    }

    game_state->overlay = SDL_CreateTextureFromSurface(game_state->game_renderer, scanlines);
    
    if (!game_state->overlay)
    {
        log_SDL_error("SDL_CreateTextureFromSurface for scanlines failed");
    }

    game_state->overlay_src_rect = {0, 0, 0, 0};

    SDL_GetRendererOutputSize(game_state->game_renderer, &game_state->overlay_src_rect.w, &game_state->overlay_src_rect.h); 
    if (TTF_Init() < 0)
    {
        log("TTF_Init failed");
    }

    game_state->delta->delta_ms = 3;
    game_state->aud_handler->load_all_sounds();
    game_state->aud_handler->init_volume(game_state);

    game_state->gfx_handler->load_renderer(game_state->game_renderer);

    game_state->gfx_handler->load_all_graphics();
    SDL_ShowCursor(SDL_DISABLE);

    game_state->gamepad = NULL;

    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i)) 
        {
            game_state->gamepad = SDL_GameControllerOpen(i);
            if (game_state->gamepad)
            {
                break;
            }
        }
    }

    if (!game_state->gamepad)
    {
        log_SDL_error("SDL_GameControllerOpen failed");
    }

    SDL_GameControllerEventState(SDL_ENABLE);
}

/**
 * Render the scanlines overlay to the renderer game_renderer.
 * 
 * @param game_renderer The renderer to render the scanlines to.
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void render_scanlines(SDL_Renderer* game_renderer, unique_ptr<Game_State>& game_state)
{

    SDL_RenderCopy(game_renderer, game_state->overlay, &game_state->overlay_src_rect, NULL);

}

/**
 * Check for whether or not to spawn an entity of the given type, and spawn it if
 * required.
 * 
 * @param game_state The current game_state.
 * @param init An Entity_Data struct with the initialization for the new object.
 * 
 ******************************************************************************/
template <class T>
void check_for_spawn(unique_ptr<Game_State>& game_state, Entity_Data init)
{
    game_state->ent_handler->increment_ticks(init.ent_type);
    if (init.ent_type == Entity_Type::FUEL_PICKUP && game_state->current_player->get_fuel() <= (rand() % 30) + 30 && game_state->ent_handler->get_current_on_screen(Entity_Type::FUEL_PICKUP) < 1)
    {
        game_state->ent_handler->spawn_new<T>(game_state, init);
        game_state->ent_handler->increment_current_on_screen(init.ent_type);
        game_state->ent_handler->ticks_minus();
        game_state->ent_handler->reset_ticks(init.ent_type);
    }
    else if (init.ent_type == Entity_Type::PLATFORM && game_state->ent_handler->get_current_ticks(init.ent_type) > game_state->ent_handler->get_max_ticks(init.ent_type))
    {
        game_state->ent_handler->spawn_new<T>(game_state, init);
        game_state->ent_handler->increment_current_on_screen(init.ent_type);
        game_state->ent_handler->ticks_minus();
        game_state->ent_handler->reset_ticks(init.ent_type);
    }
    else if (game_state->ent_handler->get_current_ticks(init.ent_type) > game_state->ent_handler->get_max_ticks(init.ent_type))
    {
        if (rand() % SPAWN_RATE == 0 && 
            game_state->ent_handler->get_current_on_screen(init.ent_type) < game_state->ent_handler->get_max_on_screen(init.ent_type) && 
            game_state->current_level->get_difficulty_level() >= game_state->ent_handler->get_min_difficulty(init.ent_type))
        {
            game_state->ent_handler->spawn_new<T>(game_state, init);
            game_state->ent_handler->increment_current_on_screen(init.ent_type);
            game_state->ent_handler->ticks_minus();
            game_state->ent_handler->reset_ticks(init.ent_type);
        }
        else
        {
            game_state->ent_handler->decrement_ticks(init.ent_type);
        }
    }
}

/**
 * Add the proper amount to the current depth counter: to run once per tick.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void process_depth(unique_ptr<Game_State>& game_state)
{
    game_state->depth += DEPTH_PER_TICK;
    game_state->current_level->set_depth_this_level(game_state->current_level->get_depth_this_level() + DEPTH_PER_TICK);
}

/**
 * Add the current delta to the ms_this_level counter and if enough time has passed,
 * increment the difficulty level and update relevant variables.
 * 
 * @param game_state The current game_state.
 * @param delta The most recent delta-time value.
 * 
 ******************************************************************************/
void process_level_up(unique_ptr<Game_State>& game_state, const int delta)
{
    game_state->current_level->set_ms_this_level(game_state->current_level->get_ms_this_level() + delta);
    int target = (game_state->current_level->get_difficulty_level() < 5 ? 100 : 200);
    if (game_state->current_level->get_depth_this_level() >= target)
    {
        if (game_state->current_level->get_difficulty_level() % 5 == 0)
        {
            game_state->ent_handler->increment_max_on_screen(Entity_Type::MOONROCK);
        }
        game_state->current_level->set_difficulty_level(game_state->current_level->get_difficulty_level() + 1);
        game_state->current_level->set_ms_this_level(0);
        game_state->current_level->reset_depth_this_level();
        randomize_target_color(game_state);
        game_state->current_level->set_min_wall_width(game_state->current_level->get_min_wall_width() - MIN_WALL_WIDTH_FACTOR);
        game_state->current_level->set_max_wall_width(game_state->current_level->get_max_wall_width() - MAX_WALL_WIDTH_FACTOR);
        game_state->aud_handler->play_sfx("sfx_next_level", AH_FIRST_AVAIL, 0);
    }
}

/**
 * Check if player thrust is active and if so, increment the ms_this_fuel counter. If
 * the counter is over the maximum allowable level, decrement player fuel.
 * 
 * @param game_state The current game_state.
 * @param delta The most recent delta-time value.
 * 
 ******************************************************************************/
void process_fuel(unique_ptr<Game_State>& game_state, const int delta)
{
    if (game_state->current_player->get_thrust_active())
    {
        game_state->aud_handler->resume_sfx(1);
        game_state->current_player->set_ms_this_fuel(game_state->current_player->get_ms_this_fuel() + delta);
        if (game_state->current_player->get_ms_this_fuel() > MS_FUEL)
        {
            game_state->current_player->set_fuel(game_state->current_player->get_fuel() - 1);
            game_state->current_player->set_ms_this_fuel(0);
        }
    }
    else
    {
        game_state->aud_handler->pause_sfx(1);
    }
}

/**
 * Set the 'target color' to a random value.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void randomize_target_color(unique_ptr<Game_State>& game_state)
{
    game_state->target_color->red = 10 + (rand() % 240);
    game_state->target_color->green = 10 + (rand() % 240);
    game_state->target_color->blue = 40 + (rand() % 200);
}

/**
 * Update the current wall color by incrementing/decrementing it toward the target value.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void update_wall_color(unique_ptr<Game_State>& game_state)
{
    game_state->wall_color->red += ((game_state->wall_color->red < game_state->target_color->red) ? 1 : -1);
    game_state->wall_color->green += ((game_state->wall_color->green < game_state->target_color->green) ? 1 : -1);
    game_state->wall_color->blue += ((game_state->wall_color->blue < game_state->target_color->blue) ? 1 : -1);
    game_state->current_level->set_wall_color(*game_state->wall_color);
}

/**
 * Reset the game_state fields to its initial state.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void reset_game_state(unique_ptr<Game_State>& game_state)
{
    game_state->current_id_pos = 0;
    game_state->current_level->set_tick_counter(0);
    game_state->current_level->set_ms_this_level(0);
    game_state->current_level->set_difficulty_level(STARTING_DIFFICULTY_LEVEL);
    game_state->current_level->set_min_wall_width(STARTING_MIN_WALL_WIDTH - (MIN_WALL_WIDTH_FACTOR * STARTING_DIFFICULTY_LEVEL));
    game_state->current_level->set_max_wall_width(STARTING_MAX_WALL_WIDTH - (MAX_WALL_WIDTH_FACTOR * STARTING_DIFFICULTY_LEVEL));
    game_state->current_level->set_left_wall_coord(2);
    game_state->current_level->set_right_wall_coord(X_COORDS - 2);
    game_state->current_level->reset_depth_this_level();
    game_state->current_game_ms = 0;
    game_state->depth = 0;
    game_state->max_depth = 1000;
    game_state->wall_color->red = 255;
    game_state->wall_color->green = 255;
    game_state->wall_color->blue = 255;
    game_state->target_color->red = 50;
    game_state->target_color->green = 50;
    game_state->target_color->blue = 200;
    game_state->game_phase = Phase::TITLE_SCREEN;
    
    for (auto& ent : game_state->entities_on_screen)
    {
        ent->despawn();
    }
    
    game_state->ent_handler->reset_all();
    
    game_state->entities_on_screen.clear();
    
}

/**
 * Return a C-string with the relative directory concatenated to the current working directory.
 * 
 * @param cwd The current working directory.
 * @param relative The relative target directory.
 * 
 ******************************************************************************/
const char* get_path(string cwd, string relative)
{
    cwd += relative;
    return cwd.c_str();
}

/**
 * Frame update logic: run frame_process for all entities on screen, clear out any OOB 
 * entities, check for collisions, and cleanup the game_state.
 * 
 * @param game_state The current game_state.
 * @param delta The most recent delta-time value.
 * 
 ******************************************************************************/
void frame_update(unique_ptr<Game_State>& game_state, const int delta)
{
    
    for (auto& ent : game_state->entities_on_screen)
    {
        ent->frame_process(game_state, delta);
    }

    for (auto& ent : game_state->entities_on_screen)
    {
        if (ent->get_exists() && ent->is_oob())
        { 
            ent->die(game_state); 
        }
    }
    
    for (vector<shared_ptr<Entity>>::iterator it1 = game_state->entities_on_screen.begin(); it1 != game_state->entities_on_screen.end(); it1++)
    {
        if ((*it1)->get_exists())
        { 
            for (vector<shared_ptr<Entity>>::iterator it2 = (it1 + 1); it2 != game_state->entities_on_screen.end(); it2++)
            {
                SDL_Rect box2 = (*it2)->get_hitbox();
                if ((*it1)->check_collision(&box2))
                {
                    if ((*it1)->get_exists() && (*it1)->get_id() < (*it2)->get_id())
                    {
                        (*it1)->resolve_collision(game_state, (*it2));
                    }
                }
            }
        }
    }
    
    cleanup(game_state);
    
    game_state->ent_handler->clear_buffer(game_state);

    ++game_state->frame_counter;
    
}

/**
 * Render all renderable entities, render the scanline overlay, and update the renderer.
 * To be run each frame.
 * 
 * @param game_state The current game_state.
 * @param game_renderer The current renderer to render to.
 * 
 ******************************************************************************/
void frame_render(unique_ptr<Game_State>& game_state, SDL_Renderer* game_renderer)
{
    for (int z = 0; z < MAX_Z_INDEX; z++)
    {
        for (auto& ent : game_state->entities_on_screen)
        {
            if (ent->get_z_index() == z && ent->get_exists())
            {
                ent->render(game_state, game_renderer);
            }
        }
    }

    if (game_state->scanlines)
    {
        render_scanlines(game_renderer, game_state);
    }

    SDL_RenderPresent(game_renderer);
}

/**
 * Check all entities for existence: if they don't exist, remove them from the 
 * entities_on_screen vector.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void cleanup(unique_ptr<Game_State>& game_state)
{
    int index = 0;
    vector<int> del_indices;
    for (auto& ent : game_state->entities_on_screen)
    {
        if (!ent->get_exists())
        {
            del_indices.push_back(index);
        }
        index++;
    }

    for (int i = del_indices.size()-1; i > -1; i--)
    {
        game_state->entities_on_screen.erase(game_state->entities_on_screen.begin() + del_indices[i]);
    }

}

/**
 * Update logic for 'tick' steps. Check if enough time has passed to run the 
 * tick update logic: if so, run tick_process for every entity on screen,
 * check for and spawn any new entities, and update the depth counter.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void tick_update(unique_ptr<Game_State>& game_state, const int delta)
{
    game_state->current_level->set_tick_counter(game_state->current_level->get_tick_counter() + delta);

    if (game_state->current_level->get_tick_counter() > ((TICK_LENGTH) - (game_state->current_level->get_difficulty_level() * 2)))
    {
        for (auto& ent : game_state->entities_on_screen)
        {
            ent->tick_process(game_state);
        }
        
        game_state->current_level->new_walls(game_state);

        update_wall_color(game_state);

        check_for_spawn<Moonrock>(game_state, generate_moonrock_init(game_state));
        check_for_spawn<Platform>(game_state, generate_platform_init(game_state));
        check_for_spawn<Ghostal>(game_state, generate_ghostal_init(game_state));
        check_for_spawn<Fuel_Pickup>(game_state, generate_fuel_pickup_init(game_state));
        check_for_spawn<Laser_Turret>(game_state, generate_laser_turret_init(game_state));
        check_for_spawn<Homing_Missile>(game_state, generate_homing_missile_init(game_state));
        check_for_spawn<Shield_Pickup>(game_state, generate_shield_pickup_init(game_state));
        check_for_spawn<Bee>(game_state, generate_bee_init(game_state));
        check_for_spawn<Bomb_Pickup>(game_state, generate_bomb_pickup_init(game_state));

        process_depth(game_state);

        game_state->current_level->set_tick_counter(0);
    }
}

/**
 * Polling logic for the main game loop.
 * 
 * @param game_state The current game_state.
 * @param delta The most recent delta-time value.
 * 
 ******************************************************************************/
void in_game_polling(unique_ptr<Game_State>& game_state, const int delta)
{
    SDL_Event event;
    const int ANALOG_DEADZONE = 8000;

    while (SDL_PollEvent(&event) > 0)
    {
        if (event.type == SDL_QUIT)
        {
            game_state->current_player->die(game_state);
            game_state->game_phase = Phase::QUIT;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                game_state->current_player->die(game_state);
            }
            if (event.key.keysym.sym == SDLK_p)
            {
                game_state->is_paused = true;
            }
            if (event.key.keysym.sym == SDLK_6)
            {
                save_screenshot(game_state);
            }
        }
        else if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
            {
                game_state->is_paused = true;
            }
        }
    }


    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    const Sint16 left_stick_x = SDL_GameControllerGetAxis(game_state->gamepad, SDL_CONTROLLER_AXIS_LEFTX);

    if (left_stick_x < ANALOG_DEADZONE || left_stick_x > ANALOG_DEADZONE)
    {
        game_state->current_player->rotate_analog(delta, left_stick_x);
    }

    if (SDL_GameControllerGetButton(game_state->gamepad, SDL_CONTROLLER_BUTTON_DPAD_LEFT) || keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A])
    {
        game_state->current_player->rotate_left(delta);
    }
    if (SDL_GameControllerGetButton(game_state->gamepad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D])
    {
        game_state->current_player->rotate_right(delta);
    }
    if (SDL_GameControllerGetButton(game_state->gamepad, SDL_CONTROLLER_BUTTON_A) || keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_SPACE] || keystates[SDL_SCANCODE_W])
    {
        game_state->current_player->set_thrust_active(true);
    }
    else
    {
        game_state->current_player->set_thrust_active(false);
    }

}

/**
 * Polling logic for the main menu.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void attract_mode_polling(unique_ptr<Game_State>& game_state)
{
    SDL_Event event;
    const Sint16 prev_left_stick_y = SDL_GameControllerGetAxis(game_state->gamepad, SDL_CONTROLLER_AXIS_LEFTY);

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            game_state->game_phase = Phase::QUIT;
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_DOWN:
                case SDLK_s:
                {
                    game_state->main_cursor->move_down();
                    break;
                }
                case SDLK_UP:
                case SDLK_w:
                {
                    game_state->main_cursor->move_up();
                    break;
                }
                case SDLK_SPACE:
                case SDLK_RETURN:
                {   
                    if (game_state->main_cursor->get_current() == Main_Menu_Option::START)
                    {
                        game_state->game_phase = Phase::MAIN_GAME;
                    }
                    else if (game_state->main_cursor->get_current() == Main_Menu_Option::QUIT)
                    {
                        game_state->game_phase = Phase::QUIT;
                    }
                    break;
                }
                case SDLK_ESCAPE:
                {
                    game_state->game_phase = Phase::QUIT;
                    break;
                }
                case SDLK_6:
                {
                    save_screenshot(game_state);
                    break;
                }
            }
        }

        if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                {
                    if (game_state->main_cursor->get_current() == Main_Menu_Option::START)
                    {
                        game_state->game_phase = Phase::MAIN_GAME;
                    }
                    else if (game_state->main_cursor->get_current() == Main_Menu_Option::QUIT)
                    {
                        game_state->game_phase = Phase::QUIT;
                    }
                    break;
                }
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                {
                    game_state->main_cursor->move_up();
                    break;
                }
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                {
                    game_state->main_cursor->move_down();
                    break;
                }
            }
        }

        if (event.type == SDL_CONTROLLERAXISMOTION)
        {
            if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (event.caxis.value > 30000 && event.caxis.value > prev_left_stick_y + 2000)
                {
                    game_state->main_cursor->move_down();
                }
                else if (event.caxis.value < -30000 && event.caxis.value < prev_left_stick_y - 2000)
                {
                    game_state->main_cursor->move_up();
                }
                
            }
        }

    }
}

/**
 * Polling logic for the 'game over' state.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void game_over_polling(unique_ptr<Game_State>& game_state)
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            game_state->game_phase = Phase::QUIT;
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_SPACE:
                case SDLK_UP:
                {
                    game_state->game_phase = Phase::TITLE_SCREEN;
                    break;
                }
                case SDLK_ESCAPE:
                {
                    game_state->game_phase = Phase::TITLE_SCREEN;
                    break;
                }
                case SDLK_6:
                {
                    save_screenshot(game_state);
                    break;
                }
            }
        }

        if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                {
                    game_state->game_phase = Phase::TITLE_SCREEN;
                    break;
                }
            }
        }
    }
}

/**
 * Randomizes music for the current level.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void randomize_music(unique_ptr<Game_State>& game_state)
{
    int x = rand() % 11;
    while (x == game_state->last_music)
    {
        x = rand() % 11;
    }
    
    switch (x)
    {
        case 0:
        {
            game_state->aud_handler->switch_music("mus_level_theme");
            break;
        }
        case 1:
        {
            game_state->aud_handler->switch_music("mus_level_theme_two");
            break;
        }
        case 2:
        {
            game_state->aud_handler->switch_music("mus_level_theme_three");
            break;
        }
        case 3:
        {
            game_state->aud_handler->switch_music("mus_level_theme_four");
            break;
        }
        case 4:
        {
            game_state->aud_handler->switch_music("mus_level_theme_five");
            break;
        }
        case 5:
        {
            game_state->aud_handler->switch_music("mus_level_theme_six");
            break;
        }
        case 6:
        {
            game_state->aud_handler->switch_music("mus_level_theme_seven");
            break;
        }
        case 7:
        {
            game_state->aud_handler->switch_music("mus_level_theme_eight");
            break;
        }
        case 8:
        {
            game_state->aud_handler->switch_music("mus_level_theme_nine");
            break;
        }
        case 9:
        {
            game_state->aud_handler->switch_music("mus_level_theme_ten");
            break;
        }
        case 10:
        {
            game_state->aud_handler->switch_music("mus_level_theme_eleven");
            break;
        }
    }
    game_state->last_music = x;
}

/**
 * Polling logic for the 'pause' state.
 * 
 * @param game_state The current game_state.
 * 
 ******************************************************************************/
void pause_mode_polling(unique_ptr<Game_State>& game_state)
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            game_state->game_phase = Phase::QUIT;
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_p:
                {
                    game_state->is_paused = false;
                    break;
                }
                case SDLK_6:
                {
                    save_screenshot(game_state);
                    break;
                }
            }
        }

        if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_START:
                {
                    game_state->is_paused = false;
                    break;
                }
            }
        }
    }
}