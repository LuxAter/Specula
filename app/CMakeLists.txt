include(EnableExtraCompilerWarnings)

add_executable(specula main.cpp)
target_link_libraries(specula PRIVATE argparse::argparse fmt::fmt)
target_link_libraries(specula PRIVATE specula-lib)

enable_extra_compiler_warnings(specula)
