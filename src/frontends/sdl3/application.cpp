#include "application.hpp"
#include "ui.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glad/glad.h>
#include <ppu/palette.hpp>
#include <premitives/sprite.hpp>

namespace {

uint32_t create_texture() {
  uint32_t tex_id{};
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  return tex_id;
}

void update_texture(uint32_t tex_id, int32_t width, int32_t height,
                    void *data) {
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t color_to_surface_rgb(SDL_Surface *surface, nes_emu::Color color) {
  return SDL_MapSurfaceRGB(surface, color.red, color.green, color.blue);
}

void update_surface(SDL_Surface *surface, nes_emu::Sprite &data) {
  for (int x = 0; x < data.width; x++) {
    for (int y = 0; y < data.height; y++) {
      SDL_Rect rect = {x, y, 1, 1};
      SDL_FillSurfaceRect(surface, &rect,
                          color_to_surface_rgb(surface, data.get_pixel(x, y)));
    }
  }
}

} // namespace

namespace sdl3_app {
SDL3Application::SDL3Application() {

  init_sdl_opengl_imgui();

  nes.load_cartridge("nestest.nes");
  nes.reset();
}

void SDL3Application::run() {
  bool done = false;
  SDL_Event event;

  bool frame_complete = false;
  int16_t cycles = 0;
  int16_t scanlines = 0;
  nes_emu::Sprite image(256, 240);
  auto tex_id = create_texture();
  auto *draw_surface = SDL_CreateSurface(256, 240, SDL_PIXELFORMAT_RGB24);

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

    while (!frame_complete) {
      image.set_pixel(cycles - 1, scanlines,
                      nes_emu::NesPalette[(rand() % 2) ? 0x30 : 0x3F]);
      cycles++;
      if (cycles >= 341) {
        cycles = 0;
        scanlines++;
        if (scanlines >= 261) {
          scanlines = -1;
          frame_complete = true;
          update_surface(draw_surface, image);
          update_texture(tex_id, draw_surface->w, draw_surface->h,
                         draw_surface->pixels);
        }
      }
    }
    frame_complete = false;

    frame_start();

    ImGui::ShowDemoWindow();

    draw_cpu(&nes.cpu, dissassm);

    ImGui::Begin("OpenGL Texture Test");
    ImGui::Text("Pointer: %X", tex_id);
    ImGui::Text("Size: %d x %d", draw_surface->w, draw_surface->h);
    ImGui::Image((ImTextureID)(intptr_t)tex_id,
                 {(float)draw_surface->w * 2, (float)draw_surface->h * 2});
    ImGui::End();

    frame_flush();
  }
  SDL_DestroySurface(draw_surface);
}

bool SDL3Application::process_events(SDL_Event *event) {
  bool done = false;
  while (SDL_PollEvent(event)) {
    ImGui_ImplSDL3_ProcessEvent(event);
    if (event->type == SDL_EVENT_QUIT) {
      done = true;
    }
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
        event->window.windowID == SDL_GetWindowID(window)) {
      done = true;
    }
  }
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