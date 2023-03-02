//******************************************************************************
//  audio_handler.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef AUDIO_HANDLER_H
#define AUDIO_HANDLER_H

#include <vector>
#include <SDL_mixer.h>
#include <string>
#include <memory>

#define AH_INFINITE_LOOP -1
#define AH_FIRST_AVAIL -1
#define AH_ONESHOT 0

class Game_State;

struct Sfx
{
    Mix_Chunk* sound_effect;
    int id;
    std::string path;
    std::string name;
    
    Sfx(int newid, std::string newpath, std::string sfx_name) : id(newid), path(newpath), name(sfx_name) {};
    
};

struct Mus
{
    Mix_Music* music_track;
    int id;
    std::string path;
    std::string name;

    Mus(int newid, std::string newpath, std::string mus_name) : id(newid), path(newpath), name(mus_name) {};
    
};

class Audio_Handler
{
    private:
        std::vector<Sfx> sound_effects;
        std::vector<Mus> music;
        bool is_playing_music;
        std::string path;
        int current_id;

    public:
        Audio_Handler(const std::string&);
        Audio_Handler();
        void load_all_sounds();
        void init_sfx(const std::string&, const std::string&);
        void init_mus(const std::string&, const std::string&);
        void init_volume(std::unique_ptr<Game_State>&);
        bool get_is_playing_music() const;
        std::vector<Sfx>& get_sfx_vector();
        std::vector<Mus>& get_mus_vector();
        void set_path(const std::string& cwd);
        void play_sfx(const std::string&, const int, const int);
        void play_sfx(const int, const int, const int);
        void pause_sfx(const int);
        void resume_sfx(const int);
        void stop_sfx(const int);
        void switch_music(const std::string&);
        void switch_music(const int);
        void stop_music();
        void pause_music();
        void start_music();
        void clear_all_sounds();

};

#endif