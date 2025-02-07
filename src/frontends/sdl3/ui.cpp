#include "ui.hpp"

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
} // namespace sdl3_app