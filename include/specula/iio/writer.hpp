#ifndef SPECULA_IIO_WRITER_HPP
#define SPECULA_IIO_WRITER_HPP

#include <cstdint>
#include <limits>
#include <string>

#include "spec.hpp"

namespace specula::iio {
class ImageWriter {
 public:
  ImageWriter(const Spec& spec, const TypeDef& expected);
  ImageWriter(const std::string& filename, const Spec& spec,
              const TypeDef& expected);
  virtual ~ImageWriter();

  bool open(const std::string& filename);
  inline bool is_open() const { return file_ != nullptr; }
  bool close();

  virtual bool write_header() { return true; };
  virtual bool write_scanline(
      int y, int z, const void* data,
      std::uint32_t xstride = std::numeric_limits<std::uint32_t>::max()) = 0;
  virtual bool write_footer() { return true; };

 protected:
  std::string filename_ = std::string();
  std::string_view ext_ = std::string_view();
  FILE* file_ = nullptr;
  Spec spec_;
  TypeDef expected_type_;
};
}  // namespace specula::iio

#endif  // !SPECULA_IIO_WRITER_HPP