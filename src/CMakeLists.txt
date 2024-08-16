# Main Executable Project

add_executable(nes_emu main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND (NOT CMAKE_CXX_SIMULATE_ID STREQUAL
                                            "MSVC"))
  target_compile_options(stub_exe PRIVATE /GL)
  target_link_options(stub_exe PRIVATE /LTCG)
endif()

target_sources(nes_emu PRIVATE main.cpp nes_cpu.cpp nes_cpu.h)

target_link_libraries(nes_emu PRIVATE project_options project_dependency)
