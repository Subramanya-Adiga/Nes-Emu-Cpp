#pragma once
#include <SDL3/SDL.h>
#include <cpu/cpu.hpp>
#include <imgui.h>
#include <ppu/ppu.hpp>

namespace sdl3_app {
void draw_cpu(
    nes_emu::Cpu *cpu,
    const std::vector<std::pair<uint16_t, std::string_view>> &dissassembly);

void draw_pattern_and_palette(nes_emu::PPU *ppu,
                              std::array<SDL_Surface *, 2> surface,
                              std::array<uint32_t, 2> tex_id,
                              SDL_Surface *palette, uint32_t pal_tex_id,
                              uint8_t pal_idx);

} // namespace sdl3_app