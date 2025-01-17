# Main Executable Project

add_executable(nes_emu main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND (NOT CMAKE_CXX_SIMULATE_ID STREQUAL
                                            "MSVC"))
  target_compile_options(stub_exe PRIVATE /GL)
  target_link_options(stub_exe PRIVATE /LTCG)
endif()

target_sources(
  nes_emu
  PRIVATE main.cpp
          nes_cpu.cpp
          nes_cpu.hpp
          nes_bus.hpp
          utils.cpp
          utils.hpp
          nes_bus.cpp
          cpu/instruction.hpp
          cpu/instruction.cpp
          cpu/cpu.hpp
          cpu/cpu.cpp
          cpu/bus.cpp
          cpu/bus.hpp
          cartridge/mapper.cpp
          cartridge/mapper.hpp
          cartridge/mapper/mapper0.cpp
          cartridge/mapper/mapper0.hpp
          cartridge/cartridge.cpp
          cartridge/cartridge.hpp
          nes_ppu.cpp
          nes_ppu.hpp
          olcPixelGameEngine.h)

target_link_libraries(nes_emu PRIVATE project_options project_dependency X11 GL
                                      png)
