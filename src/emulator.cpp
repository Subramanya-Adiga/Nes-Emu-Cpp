#include "emulator.hpp"

namespace nes_emu {

Emulator::Emulator() {
  cpu.bus = &bus;
  bus.ppu = &ppu;
}

void Emulator::load_cartridge(std::string_view path) noexcept {
  cart = std::make_unique<Cartridge>(path);
  bus.cartridge = &*cart;
  bus.controller = &controller;
  ppu.connect_to_cartridge(&*cart);
}

void Emulator::run() noexcept {
  ppu.clock();
  if (bus.clock_counter % 3 == 0) {
    if (!bus.dma_transfer) {
      cpu.clock();
    }
  }
  bus.clock();
}

void Emulator::reset() noexcept {
  cpu.reset();
  ppu.reset();
  bus.reset();
}

void Emulator::set_controller_one_status(Buttons status) {
  controller.status1 = status;
}

void Emulator::set_controller_two_status(Buttons status) {
  controller.status2 = status;
}

} // namespace nes_emu
