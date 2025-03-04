#include "application.hpp"
#include "draw_helpers.hpp"
#include "ui.hpp"
#include <backends/imgui_impl_sdl3.h>
#include <controller/controller.hpp>
#include <ppu/palette.hpp>
#include <premitives/sprite.hpp>

namespace sdl3_app {
SDL3Application::SDL3Application() {

  ctx = init_sdl("SDL-Nes", 1480, 840);
  init_audio(&ctx);
  init_gl(&ctx);
  init_imgui(&ctx);

  nes.load_cartridge("nestest.nes");
  nes.reset();
}

void SDL3Application::run() {
  bool done = false;
  SDL_Event event;

  auto tex_id = create_texture();
  auto *draw_surface = SDL_CreateSurface(256, 240, SDL_PIXELFORMAT_RGB24);

  auto pat1_id = create_texture();
  auto *pat1_srf = SDL_CreateSurface(128, 128, SDL_PIXELFORMAT_RGB24);

  auto pat2_id = create_texture();
  auto *pat2_srf = SDL_CreateSurface(128, 128, SDL_PIXELFORMAT_RGB24);

  auto pal_id = create_texture();
  auto *pal_srf = SDL_CreateSurface(256, 256, SDL_PIXELFORMAT_RGB24);
  SDL_ClearSurface(pal_srf, 0.556F, 0.629F, 0.830F, 255.0F);

  std::vector<std::pair<uint16_t, std::string_view>> dissassm;
  auto dissassm_map = nes.cpu.disassemble(0x0000, 0xFFFF);
  dissassm.reserve(dissassm_map.size());
  for (auto &&[k, v] : dissassm_map) {
    dissassm.emplace_back(k, v);
  }

  while (!done) {
    done = process_events(&event);

    if ((SDL_GetWindowFlags(ctx.window) & SDL_WINDOW_MINIMIZED) != 0U) {
      SDL_Delay(10);
      continue;
    }

    while (!nes.ppu.frame_complete) {
      nes.run();
    }
    nes.ppu.frame_complete = false;

    start_frame();

    // ImGui::ShowDemoWindow();

    draw_cpu(&nes.cpu, dissassm);

    ImGui::Begin("OpenGL Texture Test");
    update_surface(draw_surface, nes.ppu.get_screen());
    update_texture(tex_id, draw_surface->w, draw_surface->h,
                   draw_surface->pixels);
    ImGui::Image((ImTextureID)(intptr_t)tex_id,
                 {(float)draw_surface->w * 2, (float)draw_surface->h * 2});
    ImGui::End();

    draw_pattern_and_palette(&nes.ppu, {pat1_srf, pat2_srf}, {pat1_id, pat2_id},
                             pal_srf, pal_id, pal_idx);

    flush_frame(&ctx);
  }
  SDL_DestroySurface(draw_surface);
  deinit_imgui();
  deinit_audio(&ctx);
  deinit_sdl(&ctx);
  ctx = {};
}

bool SDL3Application::process_events(SDL_Event *event) {
  bool done = false;
  nes_emu::Buttons one{};
  while (SDL_PollEvent(event)) {
    ImGui_ImplSDL3_ProcessEvent(event);
    switch (event->type) {
    case SDL_EVENT_QUIT: {
      done = true;
      break;
    }
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
      if (event->window.windowID == SDL_GetWindowID(ctx.window)) {
        done = true;
      }
      break;
    }
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_KEY_DOWN: {
      switch (event->key.scancode) {
      case SDL_SCANCODE_P: {
        if (event->key.down) {
          (++pal_idx) &= 0x07;
        }
        break;
      }
      case SDL_SCANCODE_A: {
        one.select = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_S: {
        one.start = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_X: {
        one.a = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_Z: {
        one.b = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_UP: {
        one.up = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_DOWN: {
        one.down = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_LEFT: {
        one.left = static_cast<uint8_t>(event->key.down);
        break;
      }
      case SDL_SCANCODE_RIGHT: {
        one.right = static_cast<uint8_t>(event->key.down);
        break;
      }
      default:
        break;
      }
    }
    default:
      break;
    }
  }
  nes.set_controller_one_status(one);
  return done;
}

} // namespace sdl3_app