//******************************************************************************
//  graphics_handler.h
//  Copyright (C) 2021 by Jake Fitzenreider
//
//******************************************************************************
#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <vector>
#include <SDL.h>
#include <string>

struct Gfx
{
    SDL_Surface* surface;
    SDL_Texture* texture;
    int id;
    std::string path;
    std::string name;
    Gfx(int newid, std::string newpath, std::string gfx_name) : id(newid), path(newpath), name(gfx_name) {};
};


class Graphics_Handler
{
    private:
        std::vector<Gfx> graphics;
        std::string path;
        int current_id;
        SDL_Renderer* renderer;

    public:
        Graphics_Handler(const std::string&);
        Graphics_Handler();
        void load_renderer(SDL_Renderer*);
        void load_all_graphics();
        void init_gfx(const std::string&, const std::string&);
        SDL_Texture* get_texture_ptr(const std::string&);
        SDL_Texture* get_texture_ptr(const int);
        std::vector<Gfx>& get_gfx_vector();
        void set_path(const std::string& cwd);
        void clear_all_graphics();

};

#endif