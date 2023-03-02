//******************************************************************************
//  game_state.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <memory>
#include <string>
#include "level.h"
#include "player.h"
#include "audio_handler.h"
#include "entity_handler.h"
#include "main_menu_cursor.h"
#include "graphics_handler.h"
#include "datastructs.h"

struct Game_State
{
    int current_id_pos;
    std::string cwd;
    double depth, max_depth;
    bool is_paused = false;
    uint64_t frame_counter = 0;
    uint64_t current_game_ms = 0;
    uint8_t last_music = rand() % 8;
    bool fullscreen = true;
    bool scanlines = true;
    uint8_t mus_volume = 128;
    uint8_t sfx_volume = 128;
    Phase game_phase;
    SDL_Rect overlay_src_rect;
    SDL_Texture* overlay;
    SDL_GameController* gamepad;
    SDL_Renderer* game_renderer;
    SDL_Window* game_window;
    std::vector<std::shared_ptr<Entity>> entities_on_screen;
    std::shared_ptr<Player> current_player;
    std::shared_ptr<Main_Menu_Cursor> main_cursor;
    std::unique_ptr<Delta_Timer> delta;
    std::unique_ptr<RGB_Color> wall_color;
    std::unique_ptr<RGB_Color> target_color;
    std::unique_ptr<Level> current_level;
    std::unique_ptr<Audio_Handler> aud_handler;
    std::unique_ptr<Entity_Handler> ent_handler;
    std::unique_ptr<Graphics_Handler> gfx_handler;

    Game_State(std::string);
};

#endif