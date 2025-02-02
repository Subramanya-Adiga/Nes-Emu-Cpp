#pragma once
#include <ppu/ppu_bus.hpp>
#include <premitives/color.hpp>

namespace nes_emu {

union StatusRegister {
  struct {
    uint8_t unused : 5;
    uint8_t sprite_overflow : 1;
    uint8_t sprite_zero_hit : 1;
    uint8_t vertical_blank : 1;
  };
  uint8_t reg{};
};

union MaskRegister {
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
  uint8_t reg{};
};

union ControlRegister {
  struct {
    uint8_t nametable_x : 1;
    uint8_t nametable_y : 1;
    uint8_t increment_mode : 1;
    uint8_t pattern_sprite : 1;
    uint8_t pattern_background : 1;
    uint8_t sprite_size : 1;
    uint8_t slave_mode : 1;
    uint8_t enable_nmi : 1;
  };
  uint8_t reg{};
};

union LoopyRegister {
  struct {
    uint16_t coarse_x : 5;
    uint16_t coarse_y : 5;
    uint16_t nametable_x : 1;
    uint16_t nametable_y : 1;
    uint16_t fine_y : 3;
    uint16_t unused : 1;
  };
  uint16_t reg{};
};

struct PPU {
  PPU();

  bool nmi = false;
  bool irq = false;
  bool frame_complete = false;

  void connect_to_cartridge(Cartridge *cartridge) noexcept;

  void write_to_mask_register(uint8_t data) noexcept;
  void write_to_control_register(uint8_t data) noexcept;
  void write_to_address_register(uint8_t data) noexcept;
  void write_to_data_register(uint8_t data) noexcept;
  void write_to_scroll_register(uint8_t data) noexcept;

  [[nodiscard]] uint8_t read_from_status_register() noexcept;
  [[nodiscard]] uint8_t read_from_data_register() noexcept;

  void reset() noexcept;
  void clock() noexcept;

  [[nodiscard]] olc::Sprite &get_screen() const noexcept;
  [[nodiscard]] olc::Sprite &get_pattern_table(uint8_t index,
                                               uint8_t palette) noexcept;
  [[nodiscard]] Color get_color_from_palette(uint8_t palette,
                                             uint8_t pixel) const noexcept;

private:
  std::unique_ptr<olc::Sprite> spr_screen;
  std::array<std::unique_ptr<olc::Sprite>, 2> spr_pattern{};

  uint8_t write_enabled = 0;
  uint8_t fine_x{};
  uint8_t ppu_buffered_data{};
  int16_t scanlines{};
  int16_t cycles{};

  uint8_t bg_next_tile_id = {};
  uint8_t bg_next_tile_attrib = {};
  uint8_t bg_next_tile_lsb = {};
  uint8_t bg_next_tile_msb = {};
  uint16_t bg_shifter_pattern_lo = {};
  uint16_t bg_shifter_pattern_hi = {};
  uint16_t bg_shifter_attrib_lo = {};
  uint16_t bg_shifter_attrib_hi = {};

  PPUBus ppu_bus;
  ControlRegister control_register{};
  StatusRegister status_register{};
  MaskRegister mask_register{};
  LoopyRegister vram_address{};
  LoopyRegister tram_address{};

  void increment_scroll_x() noexcept;
  void increment_scroll_y() noexcept;
  void transfer_x() noexcept;
  void transfer_y() noexcept;
  void load_background_shifters() noexcept;
  void updata_shifters() noexcept;
};
} // namespace nes_emu
