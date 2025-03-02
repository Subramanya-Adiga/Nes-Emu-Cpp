#pragma once

#include "sdl_imgui_gl.hpp"
#include <emulator.hpp>

namespace sdl3_app {
class SDL3Application {
public:
  SDL3Application();
  void run();

private:
  SDLContext ctx;
  nes_emu::Emulator nes;
  uint8_t pal_idx = 0;

  bool process_events(SDL_Event *event);
};
} // namespace sdl3_app