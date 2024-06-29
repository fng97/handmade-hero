add_library(lib)

target_sources(lib PRIVATE lib.cpp PUBLIC lib.hpp)

target_include_directories(lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)

if(MYPRJ_BUILD_TESTING)
  add_executable(lib_test lib.test.cpp)

  target_link_libraries(lib_test PRIVATE GTest::gtest_main)

  include(GoogleTest)

  gtest_discover_tests(lib_test)
endif()

