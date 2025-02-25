#include <controller/controller.hpp>

namespace nes_emu {

uint8_t Controller::read_status1() { return read_controller(status1, &input1); }
uint8_t Controller::read_status2() { return read_controller(status2, &input2); }

uint8_t Controller::read_controller(Buttons status, uint16_t *input) {
  if (*input > (1 << 7)) {
    return 1;
  }
  auto pressed = (std::bit_cast<uint8_t>(status) & *input) > 0;
  if (!strobe) {
    *input <<= 1;
  }
  return static_cast<uint8_t>(pressed);
}

void Controller::write(uint8_t data) {
  strobe = static_cast<bool>(data & 1);
  if (strobe) {
    input1 = 1;
    input2 = 1;
  }
}

} // namespace nes_emu