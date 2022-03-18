cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# --- Project ---
project(
  Specula
  VERSION 0.1.0
  DESCRIPTION "C++17 Pathtracing Renderer"
  LANGUAGES C CXX
)

# --- Include Guard ---
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(SPECULA_MAIN_PROJECT TRUE)
endif()

# --- Setup ---

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(SetDefaultBuildType)
set_default_build_type(Debug)

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
  include(GetGitRevisionDescription)

  get_git_head_revision(_ GIT_COMMIT)
  git_local_changes(GIT_STATUS)

  string(SUBSTRING "${GIT_COMMIT}" 0 7 PROJECT_VERSION_BUILD)
  if(GIT_STATUS STREQUAL "DIRTY")
    set(PROJECT_VERSION_BUILD "${PROJECT_VERSION_BUILD}-dirty")
  endif()
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT SPECULA_IPO)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/compile_commands.json")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E create_symlink "${PROJECT_BINARY_DIR}/compile_commands.json"
              "${PROJECT_SOURCE_DIR}/compile_commands.json"
    )
  endif()
endif()

# --- Options ---

option(SPECULA_ENABLE_TESTING "Enable local unit tests" ${SPECULA_MAIN_PROJECT})
option(SPECULA_ENABLE_COVERAGE "Enable test coverage reporting" OFF)
option(SPECULA_ENABLE_DOCUMENTATION "Enable documentation generation" ${SPECULA_MAIN_PROJECT})

if(SPECULA_ENABLE_TESTING)
  include(CTest)
endif()

if(NOT DEFINED CPM_USE_LOCAL_PACKAGES)
  set(CPM_USE_LOCAL_PACKAGES TRUE)
endif()

# --- Dependencies ---

find_package(OpenCL QUIET)
if(OpenCL_FOUND)
  set(SPECULA_HAS_OPENCL ON)
else()
  set(SPECULA_HAS_OPENCL OFF)
endif()

include(cmake/CPM.cmake)
CPMAddPackage("gh:Neargye/magic_enum#master")
CPMAddPackage("gh:Neargye/nameof#master")
CPMAddPackage("gh:Neargye/semver#feature/enhanced_version")
CPMAddPackage("gh:agauniyal/rang#v3.2")
CPMAddPackage("gh:catchorg/Catch2#v3.0.0-preview4")
CPMAddPackage("gh:fmtlib/fmt#8.1.1")
CPMAddPackage(
  NAME spdlog
  GIT_TAG v1.9.2
  GITHUB_REPOSITORY gabime/spdlog
  OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)
CPMAddPackage("gh:p-ranav/argparse#master")
CPMAddPackage("gh:jarro2783/cxxopts#v3.0.0")
CPMAddPackage("gh:marzer/tomlplusplus#v3.0.1")
CPMAddPackage(
  NAME infoware
  GIT_TAG main
  GITHUB_REPOSITORY ThePhD/infoware
  OPTIONS "INFOWARE_USE_OPENCL ${SPECULA_HAS_OPENCL}"
)
CPMAddPackage(
  NAME xxHash
  GIT_TAG v0.8.1
  GITHUB_REPOSITORY Cyan4973/xxHash
  DOWNLOAD_ONLY True
)
CPMAddPackage(
  NAME libpng
  GIT_TAG libpng16
  GITHUB_REPOSITORY glennrp/libpng
  OPTIONS "PNG_EXECUTABLES NO" "PNG_TESTS NO"
)

list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
if(xxHash_ADDED)
  set(XXHASH_BUILD_ENABLE_INLINE_API OFF)
  set(XXHASH_BUILD_XXHSUM OFF)
  add_subdirectory("${xxHash_SOURCE_DIR}/cmake_unofficial" "${xxHash_BINARY_DIR}" EXCLUDE_FROM_ALL)
endif()

# --- Targets ---

add_subdirectory(src)
add_subdirectory(app)

if(SPECULA_ENABLE_DOCUMENTATION)
  add_subdirectory(docs)
endif()

if(SPECULA_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

# --- Packaging ---

# --- Summary ---

include(FeatureSummary)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
endif()
