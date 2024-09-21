cmake_minimum_required(VERSION 3.25)

project(handmade_hero LANGUAGES CXX)

include(CMakeDependentOption)

option(HMH_BUILD_TESTING "Enable testing" ON)
option(HMH_ENABLE_ALL_WARNINGS "Enables all compiler warnings" ON)
option(HMH_ENABLE_WARNINGS_AS_ERRORS "Treat all warnings as errors" ON)
cmake_dependent_option(HMH_ENABLE_ASAN "Enable address sanitizer" OFF
  "NOT HMH_ENABLE_MSAN; NOT HMH_ENABLE_TSAN" OFF)
cmake_dependent_option(HMH_ENABLE_UBSAN "Enable UB sanitizer" OFF
  "NOT HMH_ENABLE_MSAN" OFF)
cmake_dependent_option(HMH_ENABLE_MSAN "Enable memory sanitizer" OFF
  "NOT HMH_ENABLE_ASAN; NOT HMH_ENABLE_UBSAN; NOT HMH_ENABLE_TSAN" OFF)
cmake_dependent_option(HMH_ENABLE_TSAN "Enable thread sanitizer" OFF
  "NOT HMH_ENABLE_ASAN; NOT HMH_ENABLE_TSAN" OFF)
cmake_dependent_option(HMH_ENABLE_CODE_COVERAGE "Enable code coverage" OFF
  "NOT HMH_ENABLE_MSAN; NOT HMH_ENABLE_TSAN" OFF)

if(HMH_BUILD_TESTING)
  include(CTest)
endif()

if(HMH_ENABLE_ALL_WARNINGS)
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(HMH_ENABLE_WARNINGS_AS_ERRORS)
  add_compile_options(-Werror)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug"
    AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if(HMH_ENABLE_ASAN)
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)
  endif()

  if(HMH_ENABLE_UBSAN)
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
  endif()

  if(HMH_ENABLE_MSAN)
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=memory>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=memory>)
  endif()

  if(HMH_ENABLE_TSAN)
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=thread>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=thread>)
  endif()

  if(HMH_ENABLE_COVERAGE)
    add_compile_options($<$<CONFIG:Debug>:--coverage>)
    add_link_options($<$<CONFIG:Debug>:--coverage>)
  endif()
else()
  message(STATUS
    "Code coverage and sanitizers disabled: Debug and gcc/clang required.")
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(handmade_hero src/main.cpp)

target_link_libraries(handmade_hero PRIVATE SDL2::SDL2)

