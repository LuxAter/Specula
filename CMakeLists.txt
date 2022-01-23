cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# --- Project ---
project(
  Specula
  VERSION 0.1.0
  LANGUAGES C CXX)

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
      COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_BINARY_DIR}/compile_commands.json"
        "${PROJECT_SOURCE_DIR}/compile_commands.json")
  endif()
endif()

# --- Options ---

option(SPECULA_BUILD_TESTING "Enable local unit tests" ${SPECULA_MAIN_PROJECT})

if(SPECULA_BUILD_TESTING)
  include(CTest)
endif()

# --- Dependencies ---

include(cmake/CPM.cmake)
CPMAddPackage("gh:Neargye/magic_enum#master")
CPMAddPackage("gh:Neargye/nameof#master")
CPMAddPackage("gh:Neargye/semver#feature/enhanced_version")
CPMAddPackage("gh:agauniyal/rang#v3.2")
CPMAddPackage("gh:catchorg/Catch2#v3.0.0-preview4")
CPMAddPackage("gh:fmtlib/fmt#8.1.1")
CPMAddPackage("gh:gabime/spdlog#v1.9.2")
CPMAddPackage("gh:p-ranav/argparse#master")

list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")

# --- Targets ---

add_subdirectory(src)
add_subdirectory(app)

if(SPECULA_BUILD_TESTING)
  add_subdirectory(tests)
endif()

# --- Packaging ---

# --- Summary ---

include(FeatureSummary)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
endif()
