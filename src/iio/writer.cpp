#include "writer.hpp"

#include <cstdio>
#include <filesystem>
#include <string_view>

#include <specula/common/logging.hpp>

specula::iio::ImageWriter::ImageWriter() {}
specula::iio::ImageWriter::~ImageWriter() {
  if (_file != nullptr) std::fclose(_file);
}

bool specula::iio::ImageWriter::open(std::string_view file_path) {
  _file = std::fopen(file_path.data(), "wb");
  if (_file == nullptr) {
    LERROR("iio", "Failed to open file \"{}\" to write image to", file_path);
    return false;
  }
  _path = file_path;
  _ext = std::filesystem::path(file_path).extension().string();

  return true;
}

bool specula::iio::ImageWriter::close() {
  if (_file != nullptr) std::fclose(_file);
  _file = nullptr;
  _path.clear();
  _ext.clear();

  return true;
}