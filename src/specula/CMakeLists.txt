configure_file(
  "${PROJECT_SOURCE_DIR}/include/specula/specula/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/specula/specula/version.hpp" @ONLY
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCES "${PROJECT_BINARY_DIR}/include/specula/specula/version.hpp")

add_library(specula_specula STATIC ${SOURCES})
add_library(specula::specula ALIAS specula_specula)

target_compile_features(specula_specula PUBLIC cxx_std_20)

target_link_libraries(
  specula_specula PRIVATE specula::common specula::iio PUBLIC semver::semver magic_enum::magic_enum
)

target_include_directories(
  specula_specula
  PUBLIC "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/specula/specula"
)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  target_compile_definitions(specula_specula PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(specula_specula)

if(SPECULA_ENABLE_COVERAGE)
  include(EnableCoverage)
  enable_coverage(specula_specula)
endif()
