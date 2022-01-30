configure_file(
  "${PROJECT_SOURCE_DIR}/include/specula/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/specula/version.hpp" @ONLY
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCES "${PROJECT_BINARY_DIR}/include/specula/version.hpp")

add_library(specula STATIC ${SOURCES})
add_library(specula::specula ALIAS specula)

target_compile_features(specula PUBLIC cxx_std_20)

target_link_libraries(specula PUBLIC semver::semver spdlog::spdlog fmt::fmt)

target_include_directories(
  specula
  PUBLIC "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/specula"
)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  target_compile_definitions(specula PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(specula)

if(SPECULA_ENABLE_COVERAGE)
  include(EnableCoverage)
  enable_coverage(specula)
endif()
