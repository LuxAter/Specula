#ifndef SPECULA_IIO_PNG_HPP_
#define SPECULA_IIO_PNG_HPP_

#include "writer.hpp"

namespace specula::iio {
class PngWriter : public ImageWriter {
  bool write_header() override;
  bool write_scanline() override;
  bool write_footer() override;
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_PNG_HPP_