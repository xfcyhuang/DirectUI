cmake_minimum_required(VERSION 3.28)

project(DirectUI LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -fwrapv
  -fno-strict-aliasing
  -ffunction-sections
  -fdata-sections
)

add_link_options(-Wl,--gc-sections)

enable_testing()

add_subdirectory(src)
add_subdirectory(demo)
add_subdirectory(tests)
