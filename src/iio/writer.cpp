#include "iio/writer.hpp"

#include <cstdint>
#include <string>

#include "iio/spec.hpp"

using namespace specula::iio;

ImageWriter::ImageWriter(const Spec& spec, const TypeDef& expected)
    : file_{nullptr}, spec_{spec}, expected_type_{expected} {}
ImageWriter::ImageWriter(const std::string& filename, const Spec& spec,
                         const TypeDef& expected)
    : file_{nullptr}, spec_{spec}, expected_type_{expected} {
  open(filename);
}

ImageWriter::~ImageWriter() { close(); }

bool ImageWriter::open(const std::string& filename) {
  filename_ = filename;
  ext_ = std::string_view(filename).substr(filename.find_last_of('.') + 1);
  file_ = fopen(filename.c_str(), "wb");
  return file_ != nullptr;
}

bool ImageWriter::close() {
  if (file_ != nullptr) fclose(file_);
  filename_ = std::string();
  ext_ = std::string_view();
  file_ = nullptr;
  return true;
}