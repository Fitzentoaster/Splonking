#include "generate_init.h"
#include "constants.h"
#include "entity.h"
#include "game_state.h"
#include "freefunctions.h"
#include <string>


using std::unique_ptr;

Entity_Data generate_fuel_pickup_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::FUEL_PICKUP;
    init.col = Col_Class::FUEL_PICKUP;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SPRITE_SIZE; init.hgt = SPRITE_SIZE;
    init.move = true; init.exist = true; init.tied = false;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = SCREEN_HEIGHT - init.hgt - 1;
    init.z = 3;
    init.xspd = ((rand() % 2) + 1) * ((init.x < SCREEN_WIDTH/2) ? 1 : -1);
    init.yspd = ((rand() % 2) + 1) * -1;
    return init;
}

Entity_Data generate_bomb_pickup_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BOMB_PICKUP;
    init.col = Col_Class::BOMB_PICKUP;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SPRITE_SIZE; init.hgt = SPRITE_SIZE;
    init.move = true; init.exist = true; init.tied = false;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = SCREEN_HEIGHT - init.hgt - 1;
    init.z = 3;
    init.xspd = ((rand() % 2) + 1) * ((init.x < SCREEN_WIDTH/2) ? 1 : -1);
    init.yspd = ((rand() % 2) + 1) * -1;
    return init;
}

Entity_Data generate_shield_pickup_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::SHIELD_PICKUP;
    init.col = Col_Class::SHIELD_PICKUP;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SPRITE_SIZE; init.hgt = SPRITE_SIZE;
    init.move = true; init.exist = true; init.tied = false;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = SCREEN_HEIGHT - init.hgt - 1;
    init.z = 3;
    init.xspd = ((rand() % 2) + 1) * ((init.x < SCREEN_WIDTH/2) ? 1 : -1);
    init.yspd = ((rand() % 2) + 1) * -1;
    return init;
}

Entity_Data generate_ghostal_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::GHOSTAL;
    init.col = Col_Class::ENEMY;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 26; init.hgt = 26;
    init.move = true; init.exist = true; init.tied = true;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = SCREEN_HEIGHT - init.hgt - 10;
    init.z = 5;
    init.xspd  = rand() % game_state->current_level->get_difficulty_level() / 5;
    init.yspd = -2;
    return init;
}

Entity_Data generate_moonrock_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::MOONROCK;
    init.col = Col_Class::BOUNCER;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SPRITE_SIZE; init.hgt = SPRITE_SIZE;
    init.move = true; init.exist = true; init.tied = false;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = SCREEN_HEIGHT - init.hgt - 1;
    init.z = 5;
    init.xspd = ((rand() % 3) + 1) * ((init.x < SCREEN_WIDTH/2) ? 1 : -1);
    init.yspd = ((rand() % 3) + 1) * -1;
    return init;
}

Entity_Data generate_platform_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::PLATFORM;
    init.col = Col_Class::OBSTACLE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.hgt = PLATFORM_THICKNESS;
    init.wid = ((rand() % 200) + (300 - game_state->current_level->get_difficulty_level() * 10)); init.hgt = PLATFORM_THICKNESS;
    init.move = true; init.exist = true; init.tied = true;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + 100;
    int max_x = ((game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid) - 300;

    
    init.x = (rand() % max_x - min_x) + min_x;
    if (init.x < game_state->current_level->get_left_wall_coord() * GRID_WIDTH + 200)
    {
        init.x = min_x;
    }

    init.y = SCREEN_HEIGHT;
    init.z = 5;
    init.xspd = ((rand() % 3) + 1) * ((rand() % 2 == 1) ? 1 : -1);
    init.yspd = 0;
    init.color = *game_state->wall_color;
    return init;
}

Entity_Data generate_player_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::PLAYER;
    init.col = Col_Class::PLAYER;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SPRITE_SIZE; init.hgt = SPRITE_SIZE;
    init.move = true; init.exist = true; init.tied = false;
    init.x = 800;
    init.y = 700;
    init.z = 7;
    init.xspd = 0; init.yspd = -2.5;
    return init;
}

Entity_Data generate_attract_screen_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::ATTRACT_SCREEN;
    init.col = Col_Class::NONE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SCREEN_WIDTH; init.hgt = SCREEN_HEIGHT;
    init.move = false; init.exist = true; init.tied = false;
    init.x = 0;
    init.y = 0;
    init.z = 5;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_main_menu_cursor_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::ATTRACT_SCREEN;
    init.col = Col_Class::NONE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 26; init.hgt = 26;
    init.move = false; init.exist = true; init.tied = false;
    init.x = 650;
    init.y = 535;
    init.z = 6;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_laser_turret_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::LASER_TURRET;
    init.col = Col_Class::OBSTACLE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SPRITE_SIZE; init.hgt = SPRITE_SIZE;
    init.move = true; init.exist = true; init.tied = true;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 450;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 450;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = SCREEN_HEIGHT;
    init.z = 5;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_homing_missile_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::HOMING_MISSILE;
    init.col = Col_Class::MISSILE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 32; init.hgt = 32;
    init.move = true; init.exist = true; init.tied = false;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = HUD_HEIGHT + init.hgt;
    init.z = 5;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_bee_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BEE;
    init.col = Col_Class::ENEMY;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 32; init.hgt = 32;
    init.move = true; init.exist = true; init.tied = false;
    int min_x = (game_state->current_level->get_left_wall_coord() * GRID_WIDTH) + init.wid + 1;
    int max_x = (game_state->current_level->get_right_wall_coord() * GRID_WIDTH) - init.wid - 1;
    init.x = (rand() % ((max_x - min_x == 0) ? 1 : max_x - min_x) + min_x);
    init.y = HUD_HEIGHT + init.hgt;
    init.z = 5;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_shield_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::SHIELD;
    init.col = Col_Class::SHIELD;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 80; init.hgt = 80;
    init.move = true; init.exist = true; init.tied = false;
    init.x = game_state->current_player->get_x_origin() - 14;
    init.y = game_state->current_player->get_y_origin() - 14;
    init.z = 5;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_top_border_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BORDER;
    init.col = Col_Class::OBSTACLE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SCREEN_WIDTH; init.hgt = 2;
    init.move = false; init.exist = true; init.tied = false;
    init.x = 0;
    init.y = HUD_HEIGHT;
    init.z = 4;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_bottom_border_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BORDER;
    init.col = Col_Class::OBSTACLE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = SCREEN_WIDTH; init.hgt = 2;
    init.move = false; init.exist = true; init.tied = false;
    init.x = 0;
    init.y = SCREEN_HEIGHT - 2;
    init.z = 4;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_hud_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::HUD;
    init.col = Col_Class::NONE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 0; init.hgt = 0;
    init.move = false; init.exist = true; init.tied = false;
    init.x = 0;
    init.y = HUD_HEIGHT - 5;
    init.z = 8;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_bg_pause_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BG_PAUSE;
    init.col = Col_Class::NONE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 800; init.hgt = 450;
    init.move = false; init.exist = true; init.tied = false;
    init.x = SCREEN_WIDTH / 2 - 400;
    init.y = 300;
    init.z = 8;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_bg_gameover_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BG_GAMEOVER;
    init.col = Col_Class::NONE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 800; init.hgt = 450;
    init.move = false; init.exist = true; init.tied = false;
    init.x = SCREEN_WIDTH / 2 - 400;
    init.y = 300;
    init.z = 8;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_bg_pic_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.ent_type = Entity_Type::BG_PIC;
    init.col = Col_Class::NONE;
    init.id = game_state->current_id_pos++;
    init.cwd = game_state->cwd;
    init.wid = 1600; init.hgt = 900;
    init.move = true; init.exist = true; init.tied = false;
    init.x = 0;
    init.y = 0;
    init.z = 1;
    init.xspd = 0; init.yspd = 0;
    return init;
}

Entity_Data generate_bullet_init(unique_ptr<Game_State>& game_state)
{
    Entity_Data init;
    init.id = game_state->current_id_pos++;
    init.ent_type = Entity_Type::BULLET;
    init.col = Col_Class::BULLET;
    init.cwd = game_state->cwd;
    init.hgt = 6;
    init.wid = 6;
    init.z = 5;
    return init;
}