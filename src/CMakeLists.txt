include(EnableExtraCompilerWarnings)

configure_file("${PROJECT_SOURCE_DIR}/include/specula/version.hpp.in"
               "${PROJECT_BINARY_DIR}/include/specula/version.hpp" @ONLY)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCES "${PROJECT_BINARY_DIR}/include/specula/version.hpp")

add_library(specula STATIC ${SOURCES})
add_library(specula::specula ALIAS specula)

target_compile_features(specula PUBLIC cxx_std_17)

target_link_libraries(specula PUBLIC semver::semver spdlog::spdlog fmt::fmt)

target_include_directories(
  specula
  PUBLIC "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/specula")

enable_extra_compiler_warnings(specula)
