cmake_minimum_required(VERSION 3.25)

project(my_project LANGUAGES CXX)

option(MYPRJ_BUILD_TESTING "Enable testing" ON)
option(MYPRJ_ENABLE_ADDRESS_SANITIZER "Enable address sanitizer" OFF)
option(MYPRJ_ENABLE_CODE_COVERAGE "Enable code coverage" OFF)
option(MYPRJ_ENABLE_ALL_WARNINGS "Enables all compiler warnings" ON)
option(MYPRJ_ENABLE_WARNINGS_AS_ERRORS "Treat all warnings as errors" ON)

if(MYPRJ_ENABLE_ALL_WARNINGS)
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MYPRJ_ENABLE_WARNINGS_AS_ERRORS)
  add_compile_options(-Werror)
endif()

if(MYPRJ_BUILD_TESTING)
  include(CTest)
endif()

if(MYPRJ_ENABLE_ADDRESS_SANITIZER OR MYPRJ_ENABLE_CODE_COVERAGE)
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

  if(MYPRJ_ENABLE_CODE_COVERAGE)
    add_compile_options(
      -O0
      -g
      --coverage
      -fprofile-arcs
      -ftest-coverage)

    add_link_options(--coverage -fprofile-arcs -ftest-coverage)
  endif()

  if(MYPRJ_ENABLE_ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)

    add_link_options(-fsanitize=address)
  endif()
endif()

include(cmake/CPM.cmake)

cpmaddpackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  VERSION 1.14.0
  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

add_subdirectory(src/lib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE lib)

