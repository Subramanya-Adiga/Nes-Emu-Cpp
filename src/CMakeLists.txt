# Main Executable Project

add_library(nes_emu STATIC)

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND (NOT CMAKE_CXX_SIMULATE_ID STREQUAL
                                            "MSVC"))
  target_compile_options(nes_emu PRIVATE /GL)
  target_link_options(nes_emu PRIVATE /LTCG)
endif()

target_sources(
  nes_emu
  PRIVATE utils/utils.cpp
          cpu/instruction.cpp
          cpu/cpu.cpp
          cpu/bus.cpp
          cartridge/mapper.cpp
          cartridge/mapper/mapper0.cpp
          cartridge/cartridge.cpp
          ppu/ppu_bus.cpp
          ppu/ppu.cpp
          emulator.cpp
          vendor/olcPixelGameEngine.h
  PUBLIC emulator.hpp
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         ${CMAKE_SOURCE_DIR}/src
         FILES
         emulator.hpp
         cpu/cpu.hpp
         cpu/bus.hpp
         cpu/instruction.hpp
         cartridge/cartridge.hpp
         cartridge/mapper.hpp
         cartridge/mapper/mapper0.hpp
         ppu/ppu.hpp
         ppu/ppu_bus.hpp
         ppu/palette.hpp
         utils/utils.hpp
         premitives/color.hpp)

target_link_libraries(nes_emu PRIVATE project_options project_dependency)

get_target_property(hdrs nes_emu HEADER_SET)
message(STATUS "Interface headers: ${hdrs}")

add_executable(nes_olc main.cpp)

target_sources(
  nes_olc PRIVATE frontends/olc/application.cpp frontends/olc/application.hpp
                  vendor/olcPixelGameEngine.h)

target_link_libraries(nes_olc PRIVATE project_options project_dependency
                                      nes_emu)

if(LINUX)
  target_link_libraries(nes_olc PRIVATE png GL X11)
endif()
