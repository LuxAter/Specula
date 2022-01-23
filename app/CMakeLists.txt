include(EnableExtraCompilerWarnings)

add_executable(specula main.cpp)
target_link_libraries(specula PRIVATE argparse::argparse)

enable_extra_compiler_warnings(specula)
