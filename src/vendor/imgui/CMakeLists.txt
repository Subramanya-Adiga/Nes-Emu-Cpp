cmake_minimum_required(VERSION 3.28)

project(
  DearImGui
  VERSION 1.9.1.8
  LANGUAGES CXX)

add_library(DearImGui STATIC "imgui.cpp")
find_package(SDL3 CONFIG REQUIRED)

target_sources(
  DearImGui
  PRIVATE "imgui_tables.cpp" "imgui_widgets.cpp" "imgui_draw.cpp"
          "imgui_demo.cpp" "backends/imgui_impl_sdl3.cpp"
          "backends/imgui_impl_opengl3.cpp"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         FILES
         "imgui.h"
         "backends/imgui_impl_sdl3.h"
         "backends/imgui_impl_opengl3.h")

target_link_libraries(DearImGui PRIVATE SDL3::SDL3)
