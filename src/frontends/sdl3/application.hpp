#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

namespace sdl3_app {
class SDL3Application {
public:
  SDL3Application();
  ~SDL3Application();
  void run();

private:
  ImGuiIO io;
  bool initialized = false;
  SDL_Window *window = nullptr;
  SDL_GLContext gl_context = nullptr;
};
} // namespace sdl3_app