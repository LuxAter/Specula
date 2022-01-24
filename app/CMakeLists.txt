file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(specula-bin ${SOURCES})

target_link_libraries(
  specula-bin
  PRIVATE specula::specula
          cxxopts::cxxopts
          fmt::fmt
          spdlog::spdlog
          tomlplusplus::tomlplusplus
          iware::infoware
          magic_enum::magic_enum
)
target_include_directories(specula-bin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(specula-bin PROPERTIES RUNTIME_OUTPUT_NAME "specula")

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(specula-bin)
if(SPECULA_ENABLE_COVERAGE)
  include(EnableCoverage)
  enable_coverage(specula-bin)
endif()
