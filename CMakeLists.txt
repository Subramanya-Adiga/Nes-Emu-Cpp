cmake_minimum_required(VERSION 3.21)

project(
  nes_emu
  VERSION 1.0.0.0
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(StanderdProjectSettings)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_23)

if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_SIMULATE_ID STREQUAL
                                               "MSVC"))
  target_compile_options(project_options INTERFACE /arch:AVX2)
  target_compile_definitions(project_options INTERFACE _CRT_SECURE_NO_WARNINGS
                                                       NOMINMAX UNICODE)
endif()

set_project_warnings(project_options)

add_library(project_dependency INTERFACE)
target_precompile_headers(
  project_dependency
  INTERFACE
  <vector>
  <string>
  <map>
  <unordered_map>
  <utility>
  <string_view>
  <array>
  <algorithm>
  <memory>
  <filesystem>
  <iostream>
  <tuple>
  <variant>
  <range/v3/all.hpp>
  <fmt/format.h>)

find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)
target_link_libraries(project_dependency INTERFACE range-v3::range-v3
                                                   fmt::fmt-header-only)

message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")
include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKEDIR CMake)
else()
  set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKEDIR
    ${DEF_INSTALL_CMAKEDIR}
    CACHE PATH "Installation directory for CMake files")

foreach(p LIB BIN INCLUDE CMAKE)
  file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
  message(STATUS "Installing ${p} components to ${_path}")
  unset(_path)
endforeach()

add_subdirectory("${CMAKE_SOURCE_DIR}/src")

if(BUILD_TESTING)
  find_package(Catch2 REQUIRED)
  include(Catch)
  add_subdirectory("${CMAKE_SOURCE_DIR}/tests")

endif()
