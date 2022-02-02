#ifndef SPECULA_IIO_PNM_HPP
#define SPECULA_IIO_PNM_HPP

#include <limits>
#include <string>

#include "spec.hpp"
#include "writer.hpp"

namespace specula::iio {
struct PnmSettings {};

class PnmReader {};

class PnmWriter : public ImageWriter {
 public:
  PnmWriter(const Spec& spec, bool binary = true)
      : ImageWriter(spec, TypeDef::UINT8), binary_{binary} {}
  PnmWriter(const std::string& filename, const Spec& spec, bool binary = true)
      : ImageWriter(filename, spec, TypeDef::UINT8), binary_{binary} {}

  bool write_header() override;
  bool write_scanline(int y, int z, const void* data,
                      std::uint32_t xstride =
                          std::numeric_limits<std::uint32_t>::max()) override;

 protected:
  bool write_ascii(const std::uint8_t* data, const std::uint32_t stride);
  bool write_raw(const std::uint8_t* data, const std::uint32_t stride);

  std::uint8_t pnm_type_ = 0, bits_per_sample_ = 8, dither_ = 0;
  std::uint32_t max_val_ = 255;
  bool binary_ = false;
};
}  // namespace specula::iio

#endif  // !SPECULA_IIO_PNM_HPP