#include "application.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  sdl3_app::SDL3Application app;
  app.run();
  return 0;
}