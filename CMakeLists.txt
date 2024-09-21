cmake_minimum_required(VERSION 3.25)

project(handmade_hero LANGUAGES CXX)

option(HMH_BUILD_TESTING "Enable testing" ON)
option(HMH_ENABLE_ADDRESS_SANITIZER "Enable address sanitizer" OFF)
option(HMH_ENABLE_CODE_COVERAGE "Enable code coverage" OFF)
option(HMH_ENABLE_ALL_WARNINGS "Enables all compiler warnings" ON)
option(HMH_ENABLE_WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)

if(HMH_ENABLE_ALL_WARNINGS)
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(HMH_ENABLE_WARNINGS_AS_ERRORS)
  add_compile_options(-Werror)
endif()

if(HMH_BUILD_TESTING)
  include(CTest)
endif()

if(HMH_ENABLE_ADDRESS_SANITIZER OR HMH_ENABLE_CODE_COVERAGE)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(
      FATAL_ERROR
        "Code coverage results with an optimised (non-Debug) build may be "
        "misleading. Please switch to Debug mode "
        "(-DCMAKE_BUILD_TYPE=Debug).")
  endif()

  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(
      FATAL_ERROR
        "Code coverage requires Clang or GCC. Please use one of these "
        "compilers.")
  endif()

  if(HMH_ENABLE_CODE_COVERAGE)
    add_compile_options(
      -O0
      -g
      --coverage
      -fprofile-arcs
      -ftest-coverage)

    add_link_options(--coverage -fprofile-arcs -ftest-coverage)
  endif()

  if(HMH_ENABLE_ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)

    add_link_options(-fsanitize=address)
  endif()
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(handmade_hero src/main.cpp)

target_link_libraries(handmade_hero PRIVATE SDL2::SDL2)

