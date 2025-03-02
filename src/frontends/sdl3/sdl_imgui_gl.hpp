#pragma once
#include <SDL3/SDL.h>
#include <imgui.h>

struct SDLContext {
  bool initialized = false;
  SDL_Window *window = nullptr;
  SDL_GLContext gl_context = nullptr;
};

SDLContext init_sdl(const char *name, int32_t width, int32_t height);

void init_gl(SDLContext *ctx);

void init_imgui(SDLContext *ctx);

void deinit_imgui();

void deinit_sdl(SDLContext *ctx);

void start_frame();

void flush_frame(SDLContext *ctx);