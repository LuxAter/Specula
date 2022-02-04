file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit-tests ${SOURCES})

target_link_libraries(
  unit-tests
  PUBLIC specula
  PRIVATE Catch2 fmt::fmt spdlog::spdlog
)

add_sycl_to_target(TARGET unit-tests)

include(Catch)
catch_discover_tests(unit-tests TEST_LIST "CATCH_TESTS")

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(unit-tests)

if(SPECULA_ENABLE_COVERAGE)
  include(EnableCoverage)
  enable_coverage(unit-tests)
endif()
