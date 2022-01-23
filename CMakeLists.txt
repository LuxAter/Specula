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

# --- Setup ---

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(SetDefaultBuildType)
set_default_build_type(Debug)

if (EXISTS "${PROJECT_SOURCE_DIR}/.git")
  include(GetGitRevisionDescription)

  get_git_head_revision(A B)
  message("${A} : ${B}")
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

option(SPECULA_BUILD_TESTING "Enable local unit tests")

if(SPECULA_BUILD_TESTING)
  include(CTest)
endif()

# --- Dependencies ---

include(CPM)
cpmaddpackage("gh:fmtlib/fmt#8.1.1")
cpmaddpackage("gh:gabime/spdlog#v1.9.2")
cpmaddpackage("gh:catchorg/Catch2#v3.0.0-preview4")
cpmaddpackage("gh:p-ranav/argparse#master")
cpmaddpackage("gh:Neargye/magic_enum#master")
cpmaddpackage("gh:Neargye/nameof#master")
cpmaddpackage("gh:Neargye/semver#feature/enhanced_version")

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
