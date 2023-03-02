//******************************************************************************
//  audio_handler.cpp
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#include "audio_handler.h"
#include "game_state.h"

#include <iostream>

using std::string;
using std::unique_ptr;

// Sound Effects
#define SFX_ENEMY_DEATH "\\assets\\audio\\sfx_enemy_death.wav"
#define SFX_ENEMY_SHOOT "\\assets\\audio\\sfx_enemy_shoot.wav"
#define SFX_MOONROCK_DEATH "\\assets\\audio\\sfx_moonrock_death.wav"
#define SFX_PLAYER_DEATH "\\assets\\audio\\sfx_player_death.wav"
#define SFX_PLAYER_THRUST "\\assets\\audio\\sfx_player_thrust.wav"
#define SFX_FUEL_PICKUP "\\assets\\audio\\sfx_fuel_pickup.wav"
#define SFX_LASER_STATE "\\assets\\audio\\sfx_laser_state.wav"
#define SFX_NEXT_LEVEL "\\assets\\audio\\sfx_next_level.wav"
#define SFX_MISSILE_EXPLOSION "\\assets\\audio\\sfx_missile_explosion.wav"
#define SFX_PAUSE "\\assets\\audio\\sfx_pause.wav"
#define SFX_UNPAUSE "\\assets\\audio\\sfx_unpause.wav"
#define SFX_SPLONKING "\\assets\\audio\\sfx_splonking.wav"
#define SFX_SCREEN_CLEAR "\\assets\\audio\\sfx_screen_clear.wav"

// Music
#define MUS_LEVEL_THEME "\\assets\\audio\\mus_level_theme.ogg"
#define MUS_TITLE_SCREEN "\\assets\\audio\\mus_title_screen.ogg"
#define MUS_LEVEL_THEME_TWO "\\assets\\audio\\mus_level_theme_two.ogg"
#define MUS_LEVEL_THEME_THREE "\\assets\\audio\\mus_level_theme_three.ogg"
#define MUS_LEVEL_THEME_FOUR "\\assets\\audio\\mus_level_theme_four.ogg"
#define MUS_LEVEL_THEME_FIVE "\\assets\\audio\\mus_level_theme_five.ogg"
#define MUS_LEVEL_THEME_SIX "\\assets\\audio\\mus_level_theme_six.ogg"
#define MUS_LEVEL_THEME_SEVEN "\\assets\\audio\\mus_level_theme_seven.ogg"
#define MUS_LEVEL_THEME_EIGHT "\\assets\\audio\\mus_level_theme_eight.ogg"
#define MUS_LEVEL_THEME_NINE "\\assets\\audio\\mus_level_theme_nine.ogg"
#define MUS_LEVEL_THEME_TEN "\\assets\\audio\\mus_level_theme_ten.ogg"
#define MUS_LEVEL_THEME_ELEVEN "\\assets\\audio\\mus_level_theme_eleven.ogg"

#define MUS_GAMEOVER "\\assets\\audio\\mus_gameover.ogg"

Audio_Handler::Audio_Handler(const std::string& cwd)
{
    is_playing_music = false;
    path = cwd;
    current_id = 0;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

Audio_Handler::Audio_Handler()
{
    is_playing_music = false;
    current_id = 0;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void Audio_Handler::init_volume(std::unique_ptr<Game_State>& game_state)
{
    Mix_Volume(-1, game_state->sfx_volume);
    Mix_VolumeMusic(game_state->mus_volume);
}

void Audio_Handler::init_sfx(const std::string& sfxpath, const std::string& name) 
{
    Sfx sfx_to_add = Sfx(current_id++, sfxpath, name);
    sfx_to_add.sound_effect = Mix_LoadWAV(sfxpath.c_str());
    sound_effects.push_back(sfx_to_add);
}

void Audio_Handler::init_mus(const std::string& muspath, const std::string& name) 
{
    Mus mus_to_add = Mus(current_id++, muspath, name);
    mus_to_add.music_track = Mix_LoadMUS(muspath.c_str());
    music.push_back(mus_to_add);
}

bool Audio_Handler::get_is_playing_music() const {return is_playing_music;}
std::vector<Sfx>& Audio_Handler::get_sfx_vector() {return sound_effects;}
std::vector<Mus>& Audio_Handler::get_mus_vector() {return music;}
void Audio_Handler::stop_music() {Mix_HaltMusic(); is_playing_music = false;}
void Audio_Handler::pause_music() {Mix_PauseMusic(); is_playing_music = false;}
void Audio_Handler::start_music() {Mix_ResumeMusic(); is_playing_music = true;}
void Audio_Handler::clear_all_sounds() 
{
    for (auto& x : sound_effects)
    {
        Mix_FreeChunk(x.sound_effect);
    }
    sound_effects.clear(); music.clear();
}
void Audio_Handler::set_path(const string& cwd) {path = cwd;}
void Audio_Handler::pause_sfx(const int channel) {Mix_Pause(channel);}
void Audio_Handler::resume_sfx(const int channel) {Mix_Resume(channel);}
void Audio_Handler::stop_sfx(const int channel) {Mix_HaltChannel(channel);}

void Audio_Handler::load_all_sounds()
{
    init_sfx(path + SFX_ENEMY_DEATH, "sfx_enemy_death");
    init_sfx(path + SFX_ENEMY_SHOOT, "sfx_enemy_shoot");
    init_sfx(path + SFX_MOONROCK_DEATH, "sfx_moonrock_death");
    init_sfx(path + SFX_PLAYER_DEATH, "sfx_player_death");
    init_sfx(path + SFX_PLAYER_THRUST, "sfx_player_thrust");
    init_sfx(path + SFX_FUEL_PICKUP, "sfx_fuel_pickup");
    init_sfx(path + SFX_LASER_STATE, "sfx_laser_state");
    init_sfx(path + SFX_NEXT_LEVEL, "sfx_next_level");
    init_sfx(path + SFX_MISSILE_EXPLOSION, "sfx_missile_explosion");
    init_sfx(path + SFX_PAUSE, "sfx_pause");
    init_sfx(path + SFX_UNPAUSE, "sfx_unpause");
    init_sfx(path + SFX_SPLONKING, "sfx_splonking");
    init_sfx(path + SFX_SCREEN_CLEAR, "sfx_screen_clear");

    init_mus(path + MUS_LEVEL_THEME, "mus_level_theme");
    init_mus(path + MUS_TITLE_SCREEN, "mus_title_screen");
    init_mus(path + MUS_LEVEL_THEME_TWO, "mus_level_theme_two");
    init_mus(path + MUS_LEVEL_THEME_THREE, "mus_level_theme_three");
    init_mus(path + MUS_LEVEL_THEME_FOUR, "mus_level_theme_four");
    init_mus(path + MUS_LEVEL_THEME_FIVE, "mus_level_theme_five");
    init_mus(path + MUS_LEVEL_THEME_SIX, "mus_level_theme_six");
    init_mus(path + MUS_LEVEL_THEME_SEVEN, "mus_level_theme_seven");
    init_mus(path + MUS_LEVEL_THEME_EIGHT, "mus_level_theme_eight");
    init_mus(path + MUS_LEVEL_THEME_NINE, "mus_level_theme_nine");
    init_mus(path + MUS_LEVEL_THEME_TEN, "mus_level_theme_ten");
    init_mus(path + MUS_LEVEL_THEME_ELEVEN, "mus_level_theme_eleven");

    init_mus(path + MUS_GAMEOVER, "mus_gameover");
}

void Audio_Handler::play_sfx(const std::string& sfx_name, const int channel, const int loops)
{
    for (auto& x : sound_effects)
    {
        if (x.name == sfx_name)
            Mix_PlayChannel(channel, x.sound_effect, loops);
    }
}

void Audio_Handler::play_sfx(const int sfx_id, const int channel, const int loops)
{
    for (auto& x : sound_effects)
    {
        if (x.id == sfx_id)
            Mix_PlayChannel(channel, x.sound_effect, loops);
    }
}

void Audio_Handler::switch_music(const std::string& music_name)
{
    for (auto& x : music)
    {
        if (x.name == music_name)
        {
            Mix_PlayMusic(x.music_track, -1);
            Mix_PauseMusic();
            is_playing_music = false;
        }
    }
}

void Audio_Handler::switch_music(const int music_id)
{
    for (auto& x : music)
    {
        if (x.id == music_id)
        {
            Mix_PlayMusic(x.music_track, -1);
            Mix_PauseMusic();
            is_playing_music = false;
        }
    }
}
