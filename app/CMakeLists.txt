include(EnableExtraCompilerWarnings)
include(EnableCoverage)

add_executable(specula-bin main.cpp)

target_link_libraries(specula-bin PRIVATE specula::specula argparse::argparse
                                          fmt::fmt spdlog::spdlog)
target_include_directories(specula-bin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(specula-bin PROPERTIES RUNTIME_OUTPUT_NAME "specula")

enable_extra_compiler_warnings(specula-bin)
enable_coverage(specula-bin)
