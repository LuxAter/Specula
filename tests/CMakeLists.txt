include(Catch)
include(EnableExtraCompilerWarnings)
include(EnableCoverage)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit-tests ${SOURCES})

target_link_libraries(
  unit-tests
  PUBLIC specula
  PRIVATE Catch2 fmt::fmt spdlog::spdlog)

catch_discover_tests(unit-tests TEST_LIST "CATCH_TESTS")
enable_extra_compiler_warnings(unit-tests)
enable_coverage(unit-tests)
