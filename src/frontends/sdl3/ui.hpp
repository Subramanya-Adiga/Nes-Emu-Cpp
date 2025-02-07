#pragma once

#include <cpu/cpu.hpp>
#include <imgui.h>

namespace sdl3_app {
void draw_cpu(
    nes_emu::Cpu *cpu,
    const std::vector<std::pair<uint16_t, std::string_view>> &dissassembly);
}