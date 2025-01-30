#pragma once

// #define OLC_PGE_APPLICATION
#include "../../vendor/olcPixelGameEngine.h"
#include <emulator.hpp>

namespace olc_app {

class OlcApplication : public olc::PixelGameEngine {
  bool bEmulationRun = false;

  nes_emu::Emulator nes;
  std::map<uint16_t, std::string> mapAsm;

  float fResidualTime = 0.0F;

  uint8_t nSelectedPalette = 0x00;

  void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) noexcept;
  void DrawCpu(int x, int y) noexcept;
  void DrawCode(int x, int y, int nLines) noexcept;

public:
  OlcApplication();

  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
};

} // namespace olc_app
