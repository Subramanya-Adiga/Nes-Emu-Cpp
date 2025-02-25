#include "application.hpp"
#include "draw_helpers.hpp"
#include "ui.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <controller/controller.hpp>
#include <glad/glad.h>
#include <ppu/palette.hpp>
#include <premitives/sprite.hpp>

namespace sdl3_app {
SDL3Application::SDL3Application() {

  init_sdl_opengl_imgui();

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
  auto *pal_srf = SDL_CreateSurface(128, 128, SDL_PIXELFORMAT_RGB24);
  SDL_ClearSurface(pal_srf, 0.556F, 0.629F, 0.830F, 255.0F);

  std::vector<std::pair<uint16_t, std::string_view>> dissassm;
  auto dissassm_map = nes.cpu.disassemble(0x0000, 0xFFFF);
  dissassm.reserve(dissassm_map.size());
  for (auto &&[k, v] : dissassm_map) {
    dissassm.emplace_back(k, v);
  }

  while (!done) {
    done = process_events(&event);

    if ((SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) != 0U) {
      SDL_Delay(10);
      continue;
    }

    while (!nes.ppu.frame_complete) {
      nes.run();
    }
    nes.ppu.frame_complete = false;

    frame_start();

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

    frame_flush();
  }
  SDL_DestroySurface(draw_surface);
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
      if (event->window.windowID == SDL_GetWindowID(window)) {
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

void SDL3Application::frame_start() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void SDL3Application::frame_flush() {
  auto &io = ImGui::GetIO();
  ImGui::Render();

  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

  glClearColor(0.556F, 0.629F, 0.830F, 255.0F);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    auto *backup_window = SDL_GL_GetCurrentWindow();
    auto *backup_context = SDL_GL_GetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    SDL_GL_MakeCurrent(backup_window, backup_context);
  }

  SDL_GL_SwapWindow(window);
}

void SDL3Application::init_sdl_opengl_imgui() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS |
                SDL_INIT_GAMEPAD)) {
    initialized = false;
    fmt::print("SDL Failed To Initialize: {}\n", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("SDL3-NES", 1280, 720,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (window == nullptr) {
    initialized = false;
    fmt::print("SDL Failed To Create Window: {}\n", SDL_GetError());
    return;
  }

  // Setup OpenGL Context
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  gl_context = SDL_GL_CreateContext(window);
  if (gl_context == nullptr) {
    initialized = false;
    fmt::print("SDL Failed To Create GL Context: {}\n", SDL_GetError());
    return;
  }

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);

  // GLAD Load Protocals
  if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
    initialized = false;
    fmt::print("GLAD Failed To Initialize\n SDL_ERROR:{}\n", SDL_GetError());
    return;
  } else {
    fmt::print("SDL Version: {}\nOPENGL Version: {}.{}\nGLSL Version: "
               "{}\nOPENGL Vendor: {}\nOPENGL Rendrer: {}\n",
               SDL_GetVersion(), GLVersion.major, GLVersion.minor,
               (char *)glGetString(GL_SHADING_LANGUAGE_VERSION),
               (char *)glGetString(GL_VENDOR),
               (char *)glGetString(GL_RENDERER));
  }

  // Init ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  auto &styles = ImGui::GetStyle();
  if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    styles.WindowRounding = 0.0F;
    styles.Colors[ImGuiCol_WindowBg].w = 1.0F;
  }

  ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 130");
  initialized = true;
}

SDL3Application::~SDL3Application() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

} // namespace sdl3_app