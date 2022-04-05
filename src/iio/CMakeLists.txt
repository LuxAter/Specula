file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(specula_iio STATIC ${SOURCES})
add_library(specula::iio ALIAS specula_iio)

target_compile_features(specula_iio PUBLIC cxx_std_20)

target_link_libraries(specula_iio PRIVATE specula::common png qoi)

target_include_directories(
  specula_iio
  PUBLIC "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/specula/iio"
)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  target_compile_definitions(specula_iio PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(specula_iio)

if(SPECULA_ENABLE_COVERAGE)
  include(EnableCoverage)
  enable_coverage(specula_iio)
endif()
