#include "ui.hpp"
#include "draw_helpers.hpp"

namespace sdl3_app {

static constexpr ImVec4 COLOR_RED = {0.880, 0.0176, 0.0176, 255.0};
static constexpr ImVec4 COLOR_GREEN = {0.0195, 0.650, 0.0826, 255.0};

void draw_cpu(
    nes_emu::Cpu *cpu,
    const std::vector<std::pair<uint16_t, std::string_view>> &dissassembly) {
  ImGui::Begin("Cpu");
  // Status Flags
  ImGui::Text("Status Falgs:");
  ImGui::Spacing();
  ImGui::TextColored(((cpu->status & nes_emu::CpuFlags::Negative) != 0)
                         ? COLOR_GREEN
                         : COLOR_RED,
                     "N");
  ImGui::SameLine();
  ImGui::TextColored(((cpu->status & nes_emu::CpuFlags::Overflow) != 0)
                         ? COLOR_GREEN
                         : COLOR_RED,
                     "V");
  ImGui::SameLine();
  ImGui::TextColored(((cpu->status & nes_emu::CpuFlags::Unused) != 0)
                         ? COLOR_GREEN
                         : COLOR_RED,
                     "-");
  ImGui::SameLine();
  ImGui::TextColored(
      ((cpu->status & nes_emu::CpuFlags::Break) != 0) ? COLOR_GREEN : COLOR_RED,
      "B");
  ImGui::SameLine();
  ImGui::TextColored(((cpu->status & nes_emu::CpuFlags::Decimal) != 0)
                         ? COLOR_GREEN
                         : COLOR_RED,
                     "D");
  ImGui::SameLine();
  ImGui::TextColored(((cpu->status & nes_emu::CpuFlags::IntruptDisable) != 0)
                         ? COLOR_GREEN
                         : COLOR_RED,
                     "I");
  ImGui::SameLine();
  ImGui::TextColored(
      ((cpu->status & nes_emu::CpuFlags::Zero) != 0) ? COLOR_GREEN : COLOR_RED,
      "Z");
  ImGui::SameLine();
  ImGui::TextColored(
      ((cpu->status & nes_emu::CpuFlags::Carry) != 0) ? COLOR_GREEN : COLOR_RED,
      "C");
  ImGui::Spacing();

  ImGui::Text("PC: %X", cpu->pc);
  ImGui::Text("A : %X [%d]", cpu->a, cpu->a);
  ImGui::Text("X : %X [%d]", cpu->x, cpu->x);
  ImGui::Text("Y : %X [%d]", cpu->y, cpu->y);
  ImGui::Text("StackPointer: %X", cpu->stack_p);
  ImGui::Spacing();

  // Dissassembly
  if (ImGui::BeginListBox(
          "##Dissassembly",
          {-FLT_MIN, 24 * ImGui::GetTextLineHeightWithSpacing()})) {

    static int selected_idx = 0;

    for (int i = 0; i < dissassembly.size(); i++) {
      bool selected = (dissassembly[i].first == cpu->pc);
      ImGuiSelectableFlags flags =
          selected ? ImGuiSelectableFlags_Highlight : 0;
      if (ImGui::Selectable(dissassembly[i].second.data(), selected, flags)) {
        selected_idx = i;
      }

      if (selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndListBox();
  }

  ImGui::End();
}

void draw_pattern_and_palette(nes_emu::PPU *ppu,
                              std::array<SDL_Surface *, 2> surface,
                              std::array<uint32_t, 2> tex_id,
                              SDL_Surface *palette, uint32_t pal_tex_id,
                              uint8_t pal_idx) {
  ImGui::Begin("Pattern Window");

  update_surface(surface[0], ppu->get_pattern_table(0, pal_idx));
  update_texture(tex_id[0], surface[0]->w, surface[0]->h, surface[0]->pixels);
  ImGui::Image((ImTextureID)(intptr_t)tex_id[0], {256, 256});

  update_surface(surface[1], ppu->get_pattern_table(1, pal_idx));
  update_texture(tex_id[1], surface[1]->w, surface[1]->h, surface[1]->pixels);
  ImGui::Image((ImTextureID)(intptr_t)tex_id[1], {256, 256});

  for (int pal = 0; pal < 4; pal++) {
    for (int col = 0; col < 4; col++) {
      SDL_Rect rect = {8 + (col * 28), 2 + (pal * 32), 28, 28};
      SDL_FillSurfaceRect(
          palette, &rect,
          color_to_surface_rgb(palette, ppu->get_color_from_palette(pal, col)));
    }
  }

  update_texture(pal_tex_id, palette->w, palette->h, palette->pixels);
  ImGui::Image((ImTextureID)(intptr_t)pal_tex_id, {128, 128});
  ImGui::SameLine();
  ImGui::Image((ImTextureID)(intptr_t)pal_tex_id, {128, 128});
  ImGui::End();
}

} // namespace sdl3_app