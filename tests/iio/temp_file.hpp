#ifndef SPECULA_TEST_TEMP_FILE_HPP_
#define SPECULA_TEST_TEMP_FILE_HPP_

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

#include <fmt/core.h>
#include <xxhash.h>

class TemporaryFile {
 public:
  inline TemporaryFile(std::string ext = std::string()) {
    const auto now = std::chrono::high_resolution_clock::now();
    XXH128_hash_t hash = XXH3_128bits(
        &now, sizeof(std::chrono::high_resolution_clock::time_point));
    name = fmt::format("{:08X}{:08X}{}", hash.high64, hash.low64, ext);
  }

  inline void remove() const {
    if (std::filesystem::exists(name)) std::filesystem::remove(name);
  }

  inline std::string hash() const {
    XXH3_state_t* const state = XXH3_createState();
    if (state == NULL) return "";

    if (XXH3_128bits_reset(state) == XXH_ERROR) return "";

    char buffer[512] = {0};
    std::size_t len = 0;
    FILE* fh = fopen(name.c_str(), "rb");

    if (fh != nullptr) {
      while ((len = fread(buffer, sizeof(char),
                          sizeof(buffer) / sizeof(buffer[0]), fh)) > 0) {
        if (XXH3_128bits_update(state, buffer, len) == XXH_ERROR) return "";
      }
    }
    fclose(fh);

    const XXH128_hash_t hash = XXH3_128bits_digest(state);
    XXH3_freeState(state);

    return fmt::format("{:08X}{:08X}", hash.high64, hash.low64);
  }

  std::string name;
};

#endif