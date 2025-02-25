#pragma once

namespace nes_emu {

struct Buttons {
  uint8_t a : 1;
  uint8_t b : 1;
  uint8_t select : 1;
  uint8_t start : 1;
  uint8_t up : 1;
  uint8_t down : 1;
  uint8_t left : 1;
  uint8_t right : 1;
};

struct Controller {
  bool strobe = false;
  Buttons status1{};
  Buttons status2{};
  uint16_t input1 = 1;
  uint16_t input2 = 1;

  uint8_t read_status1();
  uint8_t read_status2();

  void write(uint8_t data);

private:
  uint8_t read_controller(Buttons status, uint16_t *input);
};

} // namespace nes_emu