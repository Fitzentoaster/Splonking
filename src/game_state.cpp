//******************************************************************************
//  game_state.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "game_state.h"

using std::make_unique;

Game_State::Game_State(std::string path)
{
    cwd = path;
    current_id_pos = 0;
    depth = 0;
    max_depth = 500;
    game_phase = Phase::INTRO;
    std::vector<std::shared_ptr<Entity>> entities_on_screen;

    aud_handler = make_unique<Audio_Handler>(path);
    gfx_handler = make_unique<Graphics_Handler>(path);
    ent_handler = make_unique<Entity_Handler>();
    delta = make_unique<Delta_Timer>();
    wall_color = make_unique<RGB_Color>();
    target_color = make_unique<RGB_Color>();
    current_level = make_unique<Level>();
}