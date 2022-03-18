#ifndef SPECULA_IIO_SPEC_HPP_
#define SPECULA_IIO_SPEC_HPP_

#include <cstdint>

namespace specula::iio {
struct Spec {
  std::uint32_t width, height;
  std::uint8_t channels = 3;
};
}  // namespace specula::iio

#endif