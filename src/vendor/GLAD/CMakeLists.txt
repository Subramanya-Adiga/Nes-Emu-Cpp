cmake_minimum_required(VERSION 3.28)

project(
  GLAD
  VERSION 3.0.0.0
  LANGUAGES C)

add_library(GLAD STATIC "src/glad.c")

target_sources(
  GLAD
  PRIVATE "src/glad.c"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "include"
         FILES
         "include/glad/glad.h"
         "include/KHR/khrplatform.h")