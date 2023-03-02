//******************************************************************************
//  datastructs.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <cstdint>

struct Timer
{
    int minutes;
    int seconds;
    int ms;
};

struct RGB_Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct Delta_Timer
{
    int current_ms;
    int prev_ms;
    int delta_ms;
};

enum class Entity_Type
{
    FUEL_PICKUP,
    SHIELD_PICKUP,
    BOMB_PICKUP,
    EXPLOSION,
    GHOSTAL,
    MOONROCK,
    PLATFORM,
    PLAYER,
    ATTRACT_SCREEN,
    MAIN_MENU_CURSOR,
    LASER_TURRET,
    LASER,
    HOMING_MISSILE,
    BEE,
    SHIELD,
    BORDER,
    BULLET,
    HUD,
    BG_PIC,
    BG_PAUSE,
    BG_GAMEOVER,
    WALL
};

enum class Main_Menu_Option
{
    START,
    QUIT
};

enum class Phase
{
    INTRO,
    TITLE_SCREEN,
    SETTINGS_MENU,
    MAIN_GAME,
    RESTART_SCREEN,
    GAME_OVER,
    QUIT
};

enum class Col_Class
{
    PLAYER,
    OBSTACLE,
    ENEMY,
    BULLET,
    MISSILE,
    BOUNCER,
    LASER,
    FUEL_PICKUP,
    SHIELD_PICKUP,
    BOMB_PICKUP,
    SHIELD,
    NONE
};

#endif
