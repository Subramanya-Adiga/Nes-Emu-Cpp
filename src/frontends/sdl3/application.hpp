#pragma once

#include <SDL3/SDL.h>
#include <emulator.hpp>
#include <imgui.h>

namespace sdl3_app {
class SDL3Application {
public:
  SDL3Application();
  ~SDL3Application();
  void run();

private:
  bool initialized = false;
  SDL_Window *window = nullptr;
  SDL_GLContext gl_context = nullptr;
  nes_emu::Emulator nes;
};
} // namespace sdl3_app