#include "writer.hpp"

#include <cstdio>
#include <filesystem>
#include <string_view>

#include <specula/common/logging.hpp>

specula::iio::ImageWriter::ImageWriter()
    : _abort{false}, _path{}, _ext{}, _file{nullptr} {}
specula::iio::ImageWriter::~ImageWriter() {
  if (_file != nullptr) std::fclose(_file);
}

bool specula::iio::ImageWriter::open(std::string_view file_path) {
  _file = std::fopen(file_path.data(), "wb");
  if (_file == nullptr) {
    LWARN("iio", "Failed to open file \"{}\" to write image to", file_path);
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
  _abort = false;

  return true;
}

specula::iio::FullImageWriter::FullImageWriter(std::uint32_t width,
                                               std::uint32_t height,
                                               std::uint8_t channels,
                                               std::uint8_t bytes)
    : ImageWriter(), _scanline_size{width * channels * (bytes / 8)} {
  _data = reinterpret_cast<void*>(std::malloc(_scanline_size * height));
}
specula::iio::FullImageWriter::~FullImageWriter() {
  if (_data != nullptr) free(_data);
}

bool specula::iio::FullImageWriter::write_scanline(const void* data) {
  std::memcpy(_data + _offset, data, _scanline_size);
  _offset += _scanline_size;
  return true;
}
