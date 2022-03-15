file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(specula_common STATIC ${SOURCES})
add_library(specula::common ALIAS specula_common)

target_compile_features(specula_common PUBLIC cxx_std_20)

target_link_libraries(specula_common PUBLIC spdlog::spdlog fmt::fmt)

target_include_directories(
  specula_common
  PUBLIC "${PROJECT_BINARY_DIR}/include/" "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/specula/common"
)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  target_compile_definitions(specula_common PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(specula_common)

if(SPECULA_ENABLE_COVERAGE)
  include(EnableCoverage)
  enable_coverage(specula_common)
endif()
