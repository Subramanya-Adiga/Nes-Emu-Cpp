#pragma once
#include "cartridge.hpp"
#include "olcPixelGameEngine.h"

namespace nes_emu {
struct nes_ppu {

  nes_ppu();
  ~nes_ppu();

  void clock() noexcept;
  void reset() noexcept;

  void connect_to_cartridge(nes_cartridge *cart) noexcept;

  void cpu_write(uint16_t addr, uint8_t data);
  [[nodiscard]] uint8_t cpu_read(uint16_t addr);

  void ppu_write(uint16_t addr, uint8_t data);
  [[nodiscard]] uint8_t ppu_read(uint16_t addr);

  olc::Sprite &GetScreen();
  olc::Sprite &GetNameTable(uint8_t i);
  olc::Sprite &GetPatternTable(uint8_t i, uint8_t pallet);
  olc::Pixel &GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
  bool frame_complete = false;
  bool nmi = false;

private:
  uint16_t ppu_cycles{};
  uint16_t scanlines{};
  nes_cartridge *m_cart = nullptr;

  std::array<olc::Pixel, 0x40> pal_screen{};
  uint8_t tblName[2][1024];
  uint8_t tblPattern[2][4096];
  uint8_t tblPalette[32];
  olc::Sprite *sprScreen;
  olc::Sprite *sprNameTable[2];
  olc::Sprite *sprPatternTable[2];

  union {
    struct {
      uint8_t unused : 5;
      uint8_t sprite_overflow : 1;
      uint8_t sprite_zero_hit : 1;
      uint8_t vertical_blank : 1;
    };
    uint8_t reg;
  } status;

  union {
    struct {
      uint8_t grayscale : 1;
      uint8_t render_background_left : 1;
      uint8_t render_sprites_left : 1;
      uint8_t render_background : 1;
      uint8_t render_sprites : 1;
      uint8_t enhance_red : 1;
      uint8_t enhance_green : 1;
      uint8_t enhance_blue : 1;
    };
    uint8_t reg;
  } mask;

  union PPUCTRL {
    struct {
      uint8_t nametable_x : 1;
      uint8_t nametable_y : 1;
      uint8_t increment_mode : 1;
      uint8_t pattern_sprite : 1;
      uint8_t pattern_background : 1;
      uint8_t sprite_size : 1;
      uint8_t slave_mode : 1; // unused
      uint8_t enable_nmi : 1;
    };
    uint8_t reg;
  } control;

  union loopy_register {
    // Credit to Loopy for working this out :D
    struct {
      uint16_t coarse_x : 5;
      uint16_t coarse_y : 5;
      uint16_t nametable_x : 1;
      uint16_t nametable_y : 1;
      uint16_t fine_y : 3;
      uint16_t unused : 1;
    };
    uint16_t reg = 0x0000;
  };

  loopy_register vram_addr;
  loopy_register tram_addr;

  uint8_t fine_x = 0;
  uint8_t address_latch = 0;
  uint8_t ppu_data_buffer = 0;

  uint8_t bg_next_tile_id = 0x00;
  uint8_t bg_next_tile_attrib = 0x00;
  uint8_t bg_next_tile_lsb = 0x00;
  uint8_t bg_next_tile_msb = 0x00;
  uint16_t bg_shifter_pattern_lo = 0x0000;
  uint16_t bg_shifter_pattern_hi = 0x0000;
  uint16_t bg_shifter_attrib_lo = 0x0000;
  uint16_t bg_shifter_attrib_hi = 0x0000;
};

} // namespace nes_emu