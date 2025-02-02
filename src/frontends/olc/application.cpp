#include "application.hpp"

namespace {
olc::Pixel color_to_pixel(nes_emu::Color color) {
  return {color.red, color.green, color.blue};
}

void sprite_to_olc_sprite(nes_emu::Sprite *sprite, olc::Sprite *olc_sprite) {
  for (int x = 0; x < sprite->width; x++) {
    for (int y = 0; y < sprite->height; y++) {
      olc_sprite->SetPixel(x, y, color_to_pixel(sprite->get_pixel(x, y)));
    }
  }
}

} // namespace

namespace olc_app {

OlcApplication::OlcApplication() {
  sAppName = "NES Olc";
  spr_screen = std::make_unique<olc::Sprite>(256, 240);
  spr_pat = {
      std::make_unique<olc::Sprite>(128, 128),
      std::make_unique<olc::Sprite>(128, 128),
  };
}

bool OlcApplication::OnUserCreate() {
  nes.load_cartridge("nestest.nes");
  nes.reset();
  mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);
  return true;
}

bool OlcApplication::OnUserUpdate(float fElapsedTime) {
  Clear(olc::DARK_BLUE);

  nes.bus.controllers[0] = 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
  nes.bus.controllers[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

  if (GetKey(olc::Key::SPACE).bPressed) {
    bEmulationRun = !bEmulationRun;
  }

  if (GetKey(olc::Key::R).bPressed) {
    nes.reset();
  }

  if (GetKey(olc::Key::P).bPressed) {
    (++nSelectedPalette) &= 0x07;
  }

  if (bEmulationRun) {
    if (fResidualTime > 0.0F) {
      fResidualTime -= fElapsedTime;
    } else {
      fResidualTime += (1.0F / 60.0F) - fElapsedTime;
      do {
        nes.run();
      } while (!nes.ppu.frame_complete);
      nes.ppu.frame_complete = false;
    }
  } else {
    if (GetKey(olc::Key::C).bPressed) {
      do {
        nes.run();
      } while (!nes.cpu.complete());
      do {
        nes.run();
      } while (nes.cpu.complete());
    }

    if (GetKey(olc::Key::F).bPressed) {
      do {
        nes.run();
      } while (!nes.ppu.frame_complete);
      do {
        nes.run();
      } while (!nes.cpu.complete());
      nes.ppu.frame_complete = false;
    }
  }

  DrawCpu(516, 2);
  DrawCode(516, 72, 26);

  const int nSwatchSize = 6;
  for (int p = 0; p < 8; p++) {   // For each palette
    for (int s = 0; s < 4; s++) { // For each index
      FillRect(516 + (p * (nSwatchSize * 5)) + (s * nSwatchSize), 340,
               nSwatchSize, nSwatchSize,
               color_to_pixel(nes.ppu.get_color_from_palette(p, s)));
    }
  }

  DrawRect(516 + (nSelectedPalette * (nSwatchSize * 5)) - 1, 339,
           (nSwatchSize * 4), nSwatchSize, olc::WHITE);
  sprite_to_olc_sprite(&nes.ppu.get_pattern_table(0, nSelectedPalette),
                       spr_pat.at(0).get());
  DrawSprite(516, 348, spr_pat.at(0).get());
  sprite_to_olc_sprite(&nes.ppu.get_pattern_table(1, nSelectedPalette),
                       spr_pat.at(1).get());
  DrawSprite(648, 348, spr_pat.at(1).get());

  sprite_to_olc_sprite(&nes.ppu.get_screen(), spr_screen.get());
  DrawSprite(0, 0, spr_screen.get(), 2);
  return true;
}

void OlcApplication::DrawRam(int x, int y, uint16_t nAddr, int nRows,
                             int nColumns) noexcept {
  int nRamX = x;
  int nRamY = y;
  for (int row = 0; row < nRows; row++) {
    std::string sOffset = fmt::format("${:4X}:", nAddr);
    for (int col = 0; col < nColumns; col++) {
      sOffset += " " + fmt::format(" {:2X}", nes.bus.read(nAddr));
      nAddr += 1;
    }
    DrawString(nRamX, nRamY, sOffset);
    nRamY += 10;
  }
}

void OlcApplication::DrawCpu(int x, int y) noexcept {
  DrawString(x, y, "STATUS:", olc::WHITE);
  DrawString(x + 64, y, "N",
             ((nes.cpu.status & nes_emu::Negative) != 0) ? olc::GREEN
                                                         : olc::RED);
  DrawString(x + 80, y, "V",
             ((nes.cpu.status & nes_emu::Overflow) != 0) ? olc::GREEN
                                                         : olc::RED);
  DrawString(x + 96, y, "-",
             ((nes.cpu.status & nes_emu::Unused) != 0) ? olc::GREEN : olc::RED);
  DrawString(x + 112, y, "B",
             ((nes.cpu.status & nes_emu::Break) != 0) ? olc::GREEN : olc::RED);
  DrawString(x + 128, y, "D",
             ((nes.cpu.status & nes_emu::Decimal) != 0) ? olc::GREEN
                                                        : olc::RED);
  DrawString(x + 144, y, "I",
             ((nes.cpu.status & nes_emu::IntruptDisable) != 0) ? olc::GREEN
                                                               : olc::RED);
  DrawString(x + 160, y, "Z",
             ((nes.cpu.status & nes_emu::Zero) != 0) ? olc::GREEN : olc::RED);
  DrawString(x + 178, y, "C",
             ((nes.cpu.status & nes_emu::Carry) != 0) ? olc::GREEN : olc::RED);

  DrawString(x, y + 10, fmt::format("PC: ${:4X}", nes.cpu.pc));
  DrawString(x, y + 20, fmt::format("A: ${:2X} [{}]", nes.cpu.a, nes.cpu.a));
  DrawString(x, y + 30, fmt::format("X: ${:2X} [{}]", nes.cpu.x, nes.cpu.x));
  DrawString(x, y + 40, fmt::format("Y: ${:2X} [{}]", nes.cpu.y, nes.cpu.y));
  DrawString(x, y + 50, fmt::format("Stack P: ${:4X}", nes.cpu.stack_p));
}

void OlcApplication::DrawCode(int x, int y, int nLines) noexcept {
  auto it_a = mapAsm.find(nes.cpu.pc);
  int nLineY = ((nLines >> 1) * 10) + y;
  if (it_a != mapAsm.end()) {
    DrawString(x, nLineY, (*it_a).second, olc::CYAN);
    while (nLineY < (nLines * 10) + y) {
      nLineY += 10;
      if (++it_a != mapAsm.end()) {
        DrawString(x, nLineY, (*it_a).second);
      }
    }
  }

  it_a = mapAsm.find(nes.cpu.pc);
  nLineY = (nLines >> 1) * 10 + y;
  if (it_a != mapAsm.end()) {
    while (nLineY > y) {
      nLineY -= 10;
      if (--it_a != mapAsm.end()) {
        DrawString(x, nLineY, (*it_a).second);
      }
    }
  }
}

} // namespace olc_app
