//******************************************************************************
//  main.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "game_state.h"
#include "datastructs.h"
#include "freefunctions.h"

#include <random>
#include <string>
#include <cstring>
#include <iostream>
#include <chrono>
#include <ctime>
#include <memory>
#include <fstream>

using std::string;
using std::fstream;
using std::unique_ptr;

/**
 * Main game logic/loop
 *
 ******************************************************************************/
int main(int argc, char* argv[])
{
    string cwd = argv[0];
    cwd = cwd.substr(0, cwd.find_last_of('\\') + 1);
    freopen(get_path(cwd, "data\\log.txt"), "a", stderr);
    
    srand(time(NULL));

    unique_ptr<Game_State> game_state(new Game_State(cwd));
    
    load_from_ini(game_state);

    game_initialize(game_state);


    while (game_state->game_phase != Phase::QUIT)
    {    

        intro_screen_logic(game_state, game_state->game_renderer);

        reset_game_state(game_state);

        load_highscore(game_state);
                

        title_screen_logic(game_state);

        main_game_logic(game_state);

        check_for_highscore(game_state);

        game_over_logic(game_state);

        restart_screen_logic(game_state);
    }

    close(game_state);

    return 0;

}
