#pragma once

namespace nes_emu {

static const std::array<olc::Pixel, 0x40> NesPalette{
    olc::Pixel(84, 84, 84),    olc::Pixel(0, 30, 116),
    olc::Pixel(8, 16, 144),    olc::Pixel(48, 0, 136),
    olc::Pixel(68, 0, 100),    olc::Pixel(92, 0, 48),
    olc::Pixel(84, 4, 0),      olc::Pixel(60, 24, 0),
    olc::Pixel(32, 42, 0),     olc::Pixel(8, 58, 0),
    olc::Pixel(0, 64, 0),      olc::Pixel(0, 60, 0),
    olc::Pixel(0, 50, 60),     olc::Pixel(0, 0, 0),
    olc::Pixel(0, 0, 0),       olc::Pixel(0, 0, 0),

    olc::Pixel(152, 150, 152), olc::Pixel(8, 76, 196),
    olc::Pixel(48, 50, 236),   olc::Pixel(92, 30, 228),
    olc::Pixel(136, 20, 176),  olc::Pixel(160, 20, 100),
    olc::Pixel(152, 34, 32),   olc::Pixel(120, 60, 0),
    olc::Pixel(84, 90, 0),     olc::Pixel(40, 114, 0),
    olc::Pixel(8, 124, 0),     olc::Pixel(0, 118, 40),
    olc::Pixel(0, 102, 120),   olc::Pixel(0, 0, 0),
    olc::Pixel(0, 0, 0),       olc::Pixel(0, 0, 0),

    olc::Pixel(236, 238, 236), olc::Pixel(76, 154, 236),
    olc::Pixel(120, 124, 236), olc::Pixel(176, 98, 236),
    olc::Pixel(228, 84, 236),  olc::Pixel(236, 88, 180),
    olc::Pixel(236, 106, 100), olc::Pixel(212, 136, 32),
    olc::Pixel(160, 170, 0),   olc::Pixel(116, 196, 0),
    olc::Pixel(76, 208, 32),   olc::Pixel(56, 204, 108),
    olc::Pixel(56, 180, 204),  olc::Pixel(60, 60, 60),
    olc::Pixel(0, 0, 0),       olc::Pixel(0, 0, 0),

    olc::Pixel(236, 238, 236), olc::Pixel(168, 204, 236),
    olc::Pixel(188, 188, 236), olc::Pixel(212, 178, 236),
    olc::Pixel(236, 174, 236), olc::Pixel(236, 174, 212),
    olc::Pixel(236, 180, 176), olc::Pixel(228, 196, 144),
    olc::Pixel(204, 210, 120), olc::Pixel(180, 222, 120),
    olc::Pixel(168, 226, 144), olc::Pixel(152, 226, 180),
    olc::Pixel(160, 214, 228), olc::Pixel(160, 162, 160),
    olc::Pixel(0, 0, 0),       olc::Pixel(0, 0, 0)};
}