#pragma once

#include <SDL3/SDL.h>
#include <emulator.hpp>
#include <imgui.h>

namespace sdl3_app {
class SDL3Application {
public:
  SDL3Application();
  ~SDL3Application();

  SDL3Application(const SDL3Application &) = delete;
  SDL3Application(SDL3Application &&) = delete;
  SDL3Application &operator=(const SDL3Application &) = delete;
  SDL3Application &operator=(SDL3Application &&) = delete;

  void run();

private:
  bool initialized = false;
  SDL_Window *window = nullptr;
  SDL_GLContext gl_context = nullptr;
  nes_emu::Emulator nes;

  void frame_start();
  void frame_flush();

  bool process_events(SDL_Event *event);

  void init_sdl_opengl_imgui();
};
} // namespace sdl3_app