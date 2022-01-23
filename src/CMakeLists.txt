file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

configure_file("${PROJECT_SOURCE_DIR}/include/specula/version.hpp.in"
               "${PROJECT_BINARY_DIR}/include/specula/version.hpp" @ONLY)
list(APPEND SOURCES "${PROJECT_BINARY_DIR}/include/specula/version.hpp")

add_library(specula-lib STATIC ${SOURCES})
target_link_libraries(specula-lib PUBLIC semver::semver)

target_include_directories(specula-lib PUBLIC "${PROJECT_BINARY_DIR}/include")
