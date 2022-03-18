#ifndef SPECULA_IIO_PNG_HPP_
#define SPECULA_IIO_PNG_HPP_

#include <png.h>
#include <zlib.h>

#include "spec.hpp"
#include "writer.hpp"

namespace specula::iio {

struct PngSpec : Spec {
  std::uint8_t bit_depth = 8;
};

class PngWriter : public ImageWriter {
 public:
  PngWriter(const PngSpec& spec) : ImageWriter(), _spec(spec) {}
  bool write_header() override;
  bool write_scanline(const void* data) override;
  bool write_footer() override;

 private:
  PngSpec _spec;
  png_structp _ptr;
  png_infop _info;
  png_colorp _palette;
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_PNG_HPP_