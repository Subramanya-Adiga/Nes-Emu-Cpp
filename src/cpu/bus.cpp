#include "bus.hpp"

namespace nes_emu {

uint8_t Bus::read(uint16_t addr) noexcept {
  if ((addr >= 0x0000) && (addr <= 0x1FFF)) {
    return memory[addr & 0x07FF];
  }
  if ((addr >= 0x2000) && (addr <= 0x3FFF)) {
    switch (addr & 0x0007) {
    case 0x0000: {
      break;
    }
    case 0x0001: {
      break;
    }
    case 0x0002: {
      return ppu->read_from_status_register();
      break;
    }
    case 0x0003: {
      break;
    }
    case 0x0004: {
      return ppu->read_from_oam_data();
      break;
    }
    case 0x0005: {
      break;
    }
    case 0x0006: {
      break;
    }
    case 0x0007: {
      return ppu->read_from_data_register();
      break;
    }
    default:
      break;
    }
  }
  if (addr >= 0x4016 && addr <= 0x4017) {
    auto data = (m_controller_state[addr & 0x0001] & 0x80) > 0;
    m_controller_state[addr & 0x0001] <<= 1;
    return data;
  }
  if ((addr >= 0x4020) && (addr <= 0xFFFF)) {
    return cartridge->cpu_read(addr);
  }
  return 0;
}

void Bus::write(uint16_t addr, uint8_t data) noexcept {
  if ((addr >= 0x0000) && (addr <= 0x1FFF)) {
    memory[addr & 0x07FF] = data;
  }
  if ((addr >= 0x2000) && (addr <= 0x3FFF)) {
    switch (addr & 0x0007) {
    case 0x0000: {
      ppu->write_to_control_register(data);
      break;
    }
    case 0x0001: {
      ppu->write_to_mask_register(data);
      break;
    }
    case 0x0002: {
      break;
    }
    case 0x0003: {
      ppu->write_to_oam_address(data);
      break;
    }
    case 0x0004: {
      ppu->write_to_oam_data(data);
      break;
    }
    case 0x0005: {
      ppu->write_to_scroll_register(data);
      break;
    }
    case 0x0006: {
      ppu->write_to_address_register(data);
      break;
    }
    case 0x0007: {
      ppu->write_to_data_register(data);
      break;
    }
    default:
      break;
    }
  }
  if (addr == 0x4014) {
    dma_transfer = true;
    dma_page = data;
    dma_addr = 0x00;
  }
  if (addr >= 0x4016 && addr <= 0x4017) {
    m_controller_state[addr & 0x0001] = controllers[addr & 0x0001];
  }
  if ((addr >= 0x4020) && addr <= 0xFFFF) {
    cartridge->cpu_write(addr, data);
  }
}

void Bus::reset() {
  nmi = false;
  dma_addr = 0;
  dma_data = 0;
  dma_page = 0;
  dma_transfer = false;
  dma_wait = true;
}

void Bus::clock() {
  if (clock_counter % 3 == 0) {
    if (dma_transfer) {
      if (dma_wait) {
        if (clock_counter % 2 == 1) {
          dma_wait = false;
        }
      } else {
        if (clock_counter % 2 == 0) {
          dma_data = read(dma_page << 8 | dma_addr);
        } else {
          ppu->oam_buffer[dma_addr] = dma_data;
          dma_addr++;
          if (dma_addr == 0x00) {
            dma_transfer = false;
            dma_wait = true;
          }
        }
      }
    }
  }
  if (ppu->nmi) {
    nmi = true;
    ppu->nmi = false;
  }
  clock_counter++;
}

} // namespace nes_emu
