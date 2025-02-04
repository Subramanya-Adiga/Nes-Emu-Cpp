#include "sprite.hpp"

namespace nes_emu {
Sprite::Sprite(int32_t width, int32_t height) : width(width), height(height) {
  data.resize(width * height);
}

void Sprite::set_pixel(int32_t x, int32_t y, Color pixel) noexcept {
  if ((x >= 0 && x < width) && (y >= 0 && y < height)) {
    data[static_cast<uint32_t>(y * width) + static_cast<uint32_t>(x)] = pixel;
  }
}

Color Sprite::get_pixel(int32_t x, int32_t y) const noexcept {
  if ((x >= 0 && x < width) && (y >= 0 && y < height)) {
    return data[static_cast<uint32_t>(y * width) + static_cast<uint32_t>(x)];
  } else {
    return {};
  }
}

Color *Sprite::get_data() noexcept { return data.data(); }

} // namespace nes_emu
