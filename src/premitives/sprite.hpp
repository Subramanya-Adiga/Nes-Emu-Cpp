#pragma once
#include <premitives/color.hpp>

namespace nes_emu {
class Sprite {
public:
  Sprite(int32_t width, int32_t height);

  [[nodiscard]] Color get_pixel(int32_t x, int32_t y) const noexcept;
  void set_pixel(int32_t x, int32_t y, Color pixel) noexcept;

  [[nodiscard]] Color *get_data() noexcept;

private:
  int32_t width{};
  int32_t height{};
  std::vector<Color> data;
};
} // namespace nes_emu