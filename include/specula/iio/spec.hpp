#ifndef SPECULA_IIO_SPEC_HPP
#define SPECULA_IIO_SPEC_HPP

#include <cinttypes>

namespace specula::iio {
struct Spec {
  std::uint32_t width, height, depth;
  std::uint8_t channels;
};
enum TypeDef { NONE, UINT8, UINT16, UINT32 };
}  // namespace specula::iio

#endif  // !SPECULA_IIO_SPEC_HPP