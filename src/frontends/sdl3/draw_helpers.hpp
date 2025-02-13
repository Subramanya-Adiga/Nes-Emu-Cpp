#pragma once
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <premitives/color.hpp>
#include <premitives/sprite.hpp>

uint32_t create_texture();
void update_texture(uint32_t tex_id, int32_t width, int32_t height, void *data);
uint32_t color_to_surface_rgb(SDL_Surface *surface, nes_emu::Color color);
void update_surface(SDL_Surface *surface, nes_emu::Sprite &data);