//******************************************************************************
//  graphics_handler.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "graphics_handler.h"

#include <iostream>

using std::string;

// Sprites
#define BG_ATTRACT_SCREEN "\\assets\\graphics\\bg_attract_screen.bmp"
#define BG_INTRO_ONE "\\assets\\graphics\\bg_intro_one.bmp"
#define BG_INTRO_TWO "\\assets\\graphics\\bg_intro_two.bmp"
#define BG_SCROLLER "\\assets\\graphics\\bg_scroller.bmp"

#define GFX_SPRITE_BEE_L "\\assets\\graphics\\sprite_bee_l.bmp"
#define GFX_SPRITE_BEE_R "\\assets\\graphics\\sprite_bee_r.bmp"

#define GFX_SPRITE_SELECT_ARROW "\\assets\\graphics\\sprite_select_arrow.bmp"
#define GFX_SPRITE_BULLET "\\assets\\graphics\\sprite_bullet.bmp"
#define GFX_SPRITE_FUEL_PICKUP "\\assets\\graphics\\sprite_fuel_pickup.bmp"
#define GFX_SPRITE_GHOSTAL "\\assets\\graphics\\sprite_ghostal.bmp"
#define GFX_SPRITE_HOMING_MISSILE "\\assets\\graphics\\sprite_homing_missile.bmp"

#define GFX_SPRITE_LASER_TURRET_L "\\assets\\graphics\\sprite_laser_turret_l.bmp"
#define GFX_SPRITE_LASER_TURRET_L_PARTIAL_ONE "\\assets\\graphics\\sprite_laser_turret_l_one.bmp"
#define GFX_SPRITE_LASER_TURRET_L_PARTIAL_TWO "\\assets\\graphics\\sprite_laser_turret_l_two.bmp"
#define GFX_SPRITE_LASER_TURRET_L_PARTIAL_THREE "\\assets\\graphics\\sprite_laser_turret_l_three.bmp"
#define GFX_SPRITE_LASER_TURRET_L_FULL "\\assets\\graphics\\sprite_laser_turret_l_full.bmp"

#define GFX_SPRITE_LASER_TURRET_R "\\assets\\graphics\\sprite_laser_turret_r.bmp"
#define GFX_SPRITE_LASER_TURRET_R_PARTIAL_ONE "\\assets\\graphics\\sprite_laser_turret_r_one.bmp"
#define GFX_SPRITE_LASER_TURRET_R_PARTIAL_TWO "\\assets\\graphics\\sprite_laser_turret_r_two.bmp"
#define GFX_SPRITE_LASER_TURRET_R_PARTIAL_THREE "\\assets\\graphics\\sprite_laser_turret_r_three.bmp"
#define GFX_SPRITE_LASER_TURRET_R_FULL "\\assets\\graphics\\sprite_laser_turret_r_full.bmp"

#define GFX_SPRITE_MOONROCK "\\assets\\graphics\\sprite_moonrock.bmp"
#define GFX_SPRITE_MOONROCK_2 "\\assets\\graphics\\sprite_moonrock_2.bmp"
#define GFX_SPRITE_MOONROCK_3 "\\assets\\graphics\\sprite_moonrock_3.bmp"

#define GFX_SPRITE_PLAYER_NOTHRUST "\\assets\\graphics\\sprite_player_nothrust.bmp"
#define GFX_SPRITE_PLAYER_THRUST "\\assets\\graphics\\sprite_player_thrust.bmp"
#define GFX_SPRITE_SHIELD "\\assets\\graphics\\sprite_shield.bmp"
#define GFX_SPRITE_SHIELD_FLASH "\\assets\\graphics\\sprite_shield_flash.bmp"
#define GFX_SPRITE_SHIELD_PICKUP "\\assets\\graphics\\sprite_shield_pickup.bmp"
#define GFX_SPRITE_BOMB_PICKUP "\\assets\\graphics\\sprite_bomb_pickup.bmp"
#define ANIM_EXPLOSION1 "\\assets\\graphics\\anim_explosion1.bmp"
#define BG_KEYS "\\assets\\graphics\\bg_keys.bmp"
#define BG_PAUSE "\\assets\\graphics\\bg_pause.bmp"
#define BG_GAMEOVER "\\assets\\graphics\\bg_gameover.bmp"

Graphics_Handler::Graphics_Handler(const std::string& cwd)
{
    path = cwd;
    current_id = 0;
}

Graphics_Handler::Graphics_Handler()
{
    current_id = 0;
}

void Graphics_Handler::load_renderer(SDL_Renderer* rend)
{
    renderer = rend;
}

SDL_Texture* Graphics_Handler::get_texture_ptr(const std::string& gfx_name)
{
    for (auto& x : graphics)
    {
        if (x.name == gfx_name)
            return x.texture;
    }
    return nullptr;
}

SDL_Texture* Graphics_Handler::get_texture_ptr(const int gfx_id)
{
    for (auto& x : graphics)
    {
        if (x.id == gfx_id)
            return x.texture;
    }
    return nullptr;
}

void Graphics_Handler::init_gfx(const std::string& gfxpath, const std::string& name) 
{
    Gfx gfx_to_add = Gfx(current_id++, gfxpath, name);
    gfx_to_add.surface = SDL_LoadBMP(gfxpath.c_str());
    SDL_PixelFormat *pixel_format = gfx_to_add.surface->format;
    Uint32 colorkey = SDL_MapRGB(pixel_format, 255, 174, 201);
    SDL_SetColorKey(gfx_to_add.surface, SDL_TRUE, colorkey);
    gfx_to_add.texture = SDL_CreateTextureFromSurface(renderer, gfx_to_add.surface);
    graphics.push_back(gfx_to_add);
}

std::vector<Gfx>& Graphics_Handler::get_gfx_vector() {return graphics;}

void Graphics_Handler::clear_all_graphics() 
{
    for (auto& x : graphics)
    {
        SDL_DestroyTexture(x.texture);
        SDL_FreeSurface(x.surface);
    }
    graphics.clear();
}
void Graphics_Handler::set_path(const string& cwd) {path = cwd;}

void Graphics_Handler::load_all_graphics()
{
    init_gfx(path + BG_ATTRACT_SCREEN, "bg_attract_screen");
    init_gfx(path + BG_INTRO_ONE, "bg_intro_one");
    init_gfx(path + BG_INTRO_TWO, "bg_intro_two");
    init_gfx(path + BG_SCROLLER, "bg_scroller");

    init_gfx(path + GFX_SPRITE_SELECT_ARROW, "gfx_sprite_select_arrow");
    init_gfx(path + GFX_SPRITE_BEE_L, "gfx_sprite_bee_l");
    init_gfx(path + GFX_SPRITE_BEE_R, "gfx_sprite_bee_r");

    init_gfx(path + GFX_SPRITE_BULLET, "gfx_sprite_bullet");
    init_gfx(path + GFX_SPRITE_FUEL_PICKUP, "gfx_sprite_fuel_pickup");
    init_gfx(path + GFX_SPRITE_GHOSTAL, "gfx_sprite_ghostal");
    init_gfx(path + GFX_SPRITE_HOMING_MISSILE, "gfx_sprite_homing_missile");

    init_gfx(path + GFX_SPRITE_LASER_TURRET_L, "gfx_sprite_laser_turret_l");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_L_PARTIAL_ONE, "gfx_sprite_laser_turret_l_partial_one");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_L_PARTIAL_TWO, "gfx_sprite_laser_turret_l_partial_two");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_L_PARTIAL_THREE, "gfx_sprite_laser_turret_l_partial_three");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_L_FULL, "gfx_sprite_laser_turret_l_full");

    init_gfx(path + GFX_SPRITE_LASER_TURRET_R, "gfx_sprite_laser_turret_r");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_R_PARTIAL_ONE, "gfx_sprite_laser_turret_r_partial_one");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_R_PARTIAL_TWO, "gfx_sprite_laser_turret_r_partial_two");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_R_PARTIAL_THREE, "gfx_sprite_laser_turret_r_partial_three");
    init_gfx(path + GFX_SPRITE_LASER_TURRET_R_FULL, "gfx_sprite_laser_turret_r_full");

    init_gfx(path + GFX_SPRITE_MOONROCK, "gfx_sprite_moonrock");
    init_gfx(path + GFX_SPRITE_MOONROCK_2, "gfx_sprite_moonrock_2");
    init_gfx(path + GFX_SPRITE_MOONROCK_3, "gfx_sprite_moonrock_3");

    init_gfx(path + GFX_SPRITE_PLAYER_NOTHRUST, "gfx_sprite_player_nothrust");
    init_gfx(path + GFX_SPRITE_PLAYER_THRUST, "gfx_sprite_player_thrust");
    init_gfx(path + GFX_SPRITE_SHIELD, "gfx_sprite_shield");
    init_gfx(path + GFX_SPRITE_SHIELD_FLASH, "gfx_sprite_shield_flash");
    init_gfx(path + GFX_SPRITE_SHIELD_PICKUP, "gfx_sprite_shield_pickup");
    init_gfx(path + GFX_SPRITE_BOMB_PICKUP, "gfx_sprite_bomb_pickup");

    init_gfx(path + ANIM_EXPLOSION1, "anim_explosion1");
    init_gfx(path + BG_KEYS, "bg_keys");
    init_gfx(path + BG_PAUSE, "bg_pause");
    init_gfx(path + BG_GAMEOVER, "bg_gameover");

}
