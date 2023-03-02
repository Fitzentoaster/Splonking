#ifndef GENERATE_INIT_H
#define GENERATE_INIT_H

#include <memory>

#include "entity.h"
#include "datastructs.h"

Entity_Data generate_fuel_pickup_init(std::unique_ptr<Game_State>&);
Entity_Data generate_shield_pickup_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bomb_pickup_init(std::unique_ptr<Game_State>&);
Entity_Data generate_ghostal_init(std::unique_ptr<Game_State>&);
Entity_Data generate_moonrock_init(std::unique_ptr<Game_State>&);
Entity_Data generate_platform_init(std::unique_ptr<Game_State>&);
Entity_Data generate_player_init(std::unique_ptr<Game_State>&);
Entity_Data generate_attract_screen_init(std::unique_ptr<Game_State>&);
Entity_Data generate_main_menu_cursor_init(std::unique_ptr<Game_State>&);
Entity_Data generate_laser_turret_init(std::unique_ptr<Game_State>&);
Entity_Data generate_homing_missile_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bee_init(std::unique_ptr<Game_State>&);
Entity_Data generate_shield_init(std::unique_ptr<Game_State>&);
Entity_Data generate_top_border_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bottom_border_init(std::unique_ptr<Game_State>&);
Entity_Data generate_hud_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bg_pic_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bg_pause_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bg_gameover_init(std::unique_ptr<Game_State>&);
Entity_Data generate_bullet_init(std::unique_ptr<Game_State>&);

#endif