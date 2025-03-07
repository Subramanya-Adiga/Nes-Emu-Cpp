#include "ppu.hpp"
#include "palette.hpp"
#include <fmt/ranges.h>

namespace nes_emu {

PPU::PPU() {
  spr_screen = std::make_unique<Sprite>(256, 240);
  spr_pattern = {
      std::make_unique<Sprite>(128, 128),
      std::make_unique<Sprite>(128, 128),
  };
}

void PPU::connect_to_cartridge(Cartridge *cartridge) noexcept {
  ppu_bus.cart = cartridge;
}

Sprite &PPU::get_screen() const noexcept { return *spr_screen; }

Sprite &PPU::get_pattern_table(uint8_t index, uint8_t palette) noexcept {
  for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
    for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
      auto nOffset = static_cast<uint16_t>((nTileY * 256) + (nTileX * 16));

      for (uint16_t row = 0; row < 8; row++) {
        auto off = static_cast<uint16_t>((index * 0x1000) + nOffset + row);
        uint8_t tile_lsb = ppu_bus.read(off);
        uint8_t tile_msb = ppu_bus.read(off + 8);
        for (uint16_t col = 0; col < 8; col++) {
          auto pixel =
              static_cast<uint8_t>((tile_lsb & 0x01) << 1 | (tile_msb & 0x01));

          tile_lsb >>= 1;
          tile_msb >>= 1;

          spr_pattern.at(index)->set_pixel(
              (nTileX * 8) + (7 - col), (nTileY * 8) + row,
              get_color_from_palette(palette, pixel));
        }
      }
    }
  }
  return *spr_pattern.at(index);
}

void PPU::write_to_mask_register(uint8_t data) noexcept {
  mask_register.reg = data;
}

void PPU::write_to_control_register(uint8_t data) noexcept {
  control_register.reg = data;
  tram_address.nametable_x = control_register.nametable_x;
  tram_address.nametable_y = control_register.nametable_y;
}

void PPU::write_to_address_register(uint8_t data) noexcept {
  if (write_enabled == 0) {
    auto write_data = static_cast<uint16_t>(((data & 0x3F) << 8) |
                                            (tram_address.reg & 0x00FF));
    tram_address.reg = write_data;
    write_enabled = 1;
  } else {
    tram_address.reg = (tram_address.reg & 0xFF00) | data;
    vram_address = tram_address;
    write_enabled = 0;
  }
}

void PPU::write_to_data_register(uint8_t data) noexcept {
  ppu_bus.write(vram_address.reg, data);
  auto inc = static_cast<uint16_t>((control_register.increment_mode ? 32 : 1));
  vram_address.reg += inc;
}

void PPU::write_to_scroll_register(uint8_t data) noexcept {
  if (write_enabled == 0) {
    fine_x = data & 0x07;
    tram_address.coarse_x = data >> 3;
    write_enabled = 0;
  } else {
    tram_address.fine_y = data & 0x07;
    tram_address.coarse_y = data >> 3;
    write_enabled = 0;
  }
}

void PPU::write_to_oam_address(uint8_t data) noexcept { oam_address = data; }

void PPU::write_to_oam_data(uint8_t data) noexcept {
  oam_buffer[oam_address] = data;
  oam_address++;
  if (oam_address == 0x00) {
    oam =
        oam_buffer | ranges::views::chunk(4) |
        ranges::views::transform([](auto &&data) -> ObjectAttributeMemory {
          return {
              .y = data[0], .id = data[1], .attribute = data[2], .x = data[3]};
        }) |
        ranges::to<std::vector>();
  }
}

uint8_t PPU::read_from_oam_data() noexcept { return oam_buffer[oam_address]; }

uint8_t PPU::read_from_status_register() noexcept {
  auto data = (status_register.reg & 0xE0) | (ppu_buffered_data & 0x1F);
  status_register.vertical_blank = 0;
  write_enabled = 0;
  return static_cast<uint8_t>(data);
}

uint8_t PPU::read_from_data_register() noexcept {
  auto data = ppu_buffered_data;
  ppu_buffered_data = ppu_bus.read(vram_address.reg);
  if (vram_address.reg >= 0x3F00) {
    data = ppu_buffered_data;
  }
  auto inc = static_cast<uint16_t>((control_register.increment_mode ? 32 : 1));
  vram_address.reg += inc;
  return data;
}

void PPU::reset() noexcept {
  fine_x = 0;
  write_enabled = 0;
  ppu_buffered_data = 0;
  cycles = 0;
  scanlines = 0;
  status_register = {};
  control_register = {};
  mask_register = {};
  status_register = {};
  vram_address = {};
  tram_address = {};
  bg_next_tile_id = 0;
  bg_next_tile_attrib = 0;
  bg_next_tile_lsb = 0;
  bg_next_tile_msb = 0;
  bg_shifter_pattern_lo = 0;
  bg_shifter_pattern_hi = 0;
  bg_shifter_attrib_lo = 0;
  bg_shifter_attrib_hi = 0;
}

void PPU::clock() noexcept {
  if (scanlines >= -1 && scanlines < 240) {
    if (scanlines == 0 && cycles == 0) {
      cycles = 1;
    }

    if (scanlines == -1 && cycles == 1) {
      status_register.vertical_blank = 0;
    }

    if ((cycles >= 2 && cycles < 258) || (cycles >= 321 && cycles < 338)) {
      updata_shifters();
      switch ((cycles - 1) % 8) {
      case 0: {
        load_background_shifters();
        bg_next_tile_id = ppu_bus.read(0x2000 | (vram_address.reg & 0x0FFF));
        break;
      }
      case 2: {
        bg_next_tile_attrib = ppu_bus.read(
            static_cast<uint16_t>(0x23C0 | (vram_address.nametable_y << 11) |
                                  (vram_address.nametable_x << 10) |
                                  ((vram_address.coarse_y >> 2) << 3) |
                                  (vram_address.coarse_x >> 2)));
        if ((vram_address.coarse_y & 0x02) != 0) {
          bg_next_tile_attrib >>= 4;
        }
        if ((vram_address.coarse_x & 0x02) != 0) {
          bg_next_tile_attrib >>= 2;
        }
        bg_next_tile_attrib &= 0x03;
        break;
      }
      case 4: {
        bg_next_tile_lsb = ppu_bus.read(static_cast<uint16_t>(
            (control_register.pattern_background << 12) +
            (static_cast<uint16_t>(bg_next_tile_id) << 4) +
            (vram_address.fine_y) + 0));
        break;
      }
      case 6: {
        bg_next_tile_msb = ppu_bus.read(static_cast<uint16_t>(
            (control_register.pattern_background << 12) +
            (static_cast<uint16_t>(bg_next_tile_id) << 4) +
            (vram_address.fine_y) + 8));
        break;
      }
      case 7: {
        increment_scroll_x();
        break;
      }
      default:
        break;
      }
    }
    if (cycles == 256) {
      increment_scroll_y();
    }
    if (cycles == 257) {
      load_background_shifters();
      transfer_x();
    }
    if (cycles == 338 || cycles == 340) {
      bg_next_tile_id = ppu_bus.read(0x2000 | (vram_address.reg & 0x0FFF));
    }
    if (scanlines == -1 && cycles >= 280 && cycles < 305) {
      transfer_y();
    }
  }

  if (scanlines >= 241 && scanlines < 261) {
    if (scanlines == 241 && cycles == 1) {
      status_register.vertical_blank = 1;
      if (control_register.enable_nmi) {
        nmi = true;
      }
    }
  }

  uint8_t bg_pixel = 0x00;
  uint8_t bg_palette = 0x00;

  if (mask_register.render_background) {

    uint16_t bit_mux = 0x8000 >> fine_x;

    auto p0_pixel = static_cast<uint8_t>((bg_shifter_pattern_lo & bit_mux) > 0);
    auto p1_pixel = static_cast<uint8_t>((bg_shifter_pattern_hi & bit_mux) > 0);

    bg_pixel = static_cast<uint8_t>((p1_pixel << 1) | p0_pixel);

    auto bg_pal0 = static_cast<uint8_t>((bg_shifter_attrib_lo & bit_mux) > 0);
    auto bg_pal1 = static_cast<uint8_t>((bg_shifter_attrib_hi & bit_mux) > 0);
    bg_palette = static_cast<uint8_t>((bg_pal1 << 1) | bg_pal0);
  }

  spr_screen->set_pixel(cycles - 1, scanlines,
                        get_color_from_palette(bg_palette, bg_pixel));

  cycles++;
  if (cycles >= 341) {
    cycles = 0;
    scanlines++;
    if (scanlines >= 261) {
      scanlines = -1;
      frame_complete = true;
    }
  }
}

Color PPU::get_color_from_palette(uint8_t palette,
                                  uint8_t pixel) const noexcept {
  return NesPalette.at(
      ppu_bus.read(static_cast<uint16_t>(0x3F00 + (palette << 2) + pixel)) &
      0x3F);
}

void PPU::increment_scroll_x() noexcept {
  if (mask_register.render_background || mask_register.render_sprites) {
    if (vram_address.coarse_x == 31) {
      vram_address.coarse_x = 0;
      vram_address.nametable_x = ~vram_address.nametable_x;
    } else {
      vram_address.coarse_x++;
    }
  }
}

void PPU::increment_scroll_y() noexcept {
  if (mask_register.render_background || mask_register.render_sprites) {
    if (vram_address.fine_y < 7) {
      vram_address.fine_y++;
    } else {
      vram_address.fine_y = 0;
      if (vram_address.coarse_y == 29) {
        vram_address.coarse_y = 0;
        vram_address.nametable_y = ~vram_address.nametable_y;
      } else if (vram_address.coarse_y == 31) {
        vram_address.coarse_y = 0;
      } else {
        vram_address.coarse_y++;
      }
    }
  }
}

void PPU::transfer_x() noexcept {
  if (mask_register.render_background || mask_register.render_sprites) {
    vram_address.nametable_x = tram_address.nametable_x;
    vram_address.coarse_x = tram_address.coarse_x;
  }
}

void PPU::transfer_y() noexcept {
  if (mask_register.render_background || mask_register.render_sprites) {
    vram_address.fine_y = tram_address.fine_y;
    vram_address.nametable_y = tram_address.nametable_y;
    vram_address.coarse_y = tram_address.coarse_y;
  }
}

void PPU::load_background_shifters() noexcept {
  bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
  bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

  bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) |
                         (((bg_next_tile_attrib & 0b01) != 0) ? 0xFF : 0x00);
  bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) |
                         (((bg_next_tile_attrib & 0b10) != 0) ? 0xFF : 0x00);
}

void PPU::updata_shifters() noexcept {
  if (mask_register.render_background) {
    bg_shifter_pattern_lo <<= 1;
    bg_shifter_pattern_hi <<= 1;

    bg_shifter_attrib_lo <<= 1;
    bg_shifter_attrib_hi <<= 1;
  }
}

bool ObjectAttributeMemory::priority() const {
  return ((attribute >> 5) & 1) == 1;
}

bool ObjectAttributeMemory::horizontal() const {
  return ((attribute >> 6) & 1) == 1;
}

bool ObjectAttributeMemory::vertical() const {
  return ((attribute >> 7) & 1) == 1;
}

uint8_t ObjectAttributeMemory::palette_index() const {
  return (attribute & 0b11);
}

} // namespace nes_emu
