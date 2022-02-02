#include "iio/pnm.hpp"

#include <cstdint>
#include <string>

#include <fmt/core.h>

#include "iio/spec.hpp"
#include "logging.hpp"

using namespace specula::iio;

bool PnmWriter::write_header() {
  if (file_ == nullptr) {
    LWARN("specula::iio", "Cannot write PNM header, file is not open");
    return false;
  }

  if (spec_.channels <= 0 || spec_.channels >= 5) {
    LWARN("specula::iio",
          "Cannot write PNM header, unsupported number of channels");
    return false;
  }

  if (spec_.channels == 1)
    pnm_type_ = 5;
  else if (spec_.channels == 2 || spec_.channels == 4)
    pnm_type_ = 7;
  else
    pnm_type_ = 6;
  if (!binary_ && pnm_type_ != 7)
    pnm_type_ -= 3;
  else if (!binary_ && pnm_type_ == 7)
    LWARN("specula::iio",
          "PNM ascii format is not supported for PAM files, defaulting to "
          "binary output");

  max_val_ = (1 << bits_per_sample_) - 1;

  fmt::print(file_, "P{}\n", pnm_type_);
  if (pnm_type_ != 7) {
    fmt::print(file_, "{} {}\n", spec_.width, spec_.height);
    if (pnm_type_ != 1 && pnm_type_ != 4) fmt::print(file_, "{}\n", max_val_);
  } else {
    fmt::print(file_,
               "WIDTH {}\nHEIGHT {}\nDEPTH {}\nMAXVAL {}\nTUPLTYPE "
               "{}\nENDHDR\n",
               spec_.width, spec_.height, spec_.channels, max_val_,
               spec_.channels == 2 ? "GRAYSCALE_ALPHA" : "RGB_ALPHA");
  }

  return true;
}

bool PnmWriter::write_scanline(int y, int z, const void* data,
                               std::uint32_t xstride) {
  if (file_ == nullptr) {
    LWARN("specula::iio", "Cannot write PNM scanline, file is not open");
    return false;
  }
  if (data == nullptr) {
    LWARN("specula::iio", "Cannot write PNM scanline, data is null");
    return false;
  }

  if (xstride == std::numeric_limits<std::uint32_t>::max())
    xstride = spec_.channels;

  switch (pnm_type_) {
    case 2:
      [[fallthrough]];
    case 3:
      return write_ascii(reinterpret_cast<const std::uint8_t*>(data), xstride);
    case 5:
      [[fallthrough]];
    case 6:
      [[fallthrough]];
    case 7:
      return write_raw(reinterpret_cast<const std::uint8_t*>(data), xstride);
    default:
      return false;
  }
  return false;
}

bool PnmWriter::write_ascii(const std::uint8_t* data,
                            const std::uint32_t stride) {
  std::uint8_t channels = spec_.channels;
  for (std::uint32_t x = 0; x < spec_.width; ++x) {
    std::uint32_t pixel = x * stride;
    for (std::uint8_t c = 0; c < channels; ++c) {
      std::uint32_t val = data[pixel + c];
      fmt::print(file_, "{} ",
                 val * max_val_ / std::numeric_limits<std::uint8_t>::max());
    }
  }
  fmt::print(file_, "\n");
  return true;
}

bool PnmWriter::write_raw(const std::uint8_t* data,
                          const std::uint32_t stride) {
  std::uint8_t channels = spec_.channels;
  for (std::uint32_t x = 0; x < spec_.width; ++x) {
    std::uint32_t pixel = x * stride;
    for (std::uint8_t c = 0; c < channels; ++c) {
      std::uint32_t val = data[pixel + c];
      if (std::fputc(val * max_val_ / std::numeric_limits<std::uint8_t>::max(),
                     file_) == EOF)
        return false;
    }
  }
  return true;
}
// bool
// PNMOutput::write_raw(const T* data, const stride_t stride, unsigned int
// max_val)
// {
//     int nc = m_spec.nchannels;
//     for (int x = 0; x < m_spec.width; x++) {
//         unsigned int pixel = x * stride;
//         for (int c = 0; c < nc; c++) {
//             unsigned int val = data[pixel + c];
//             val              = val * max_val / std::numeric_limits<T>::max();
//             if (sizeof(T) == 2) {
//                 // Writing a 16bit ppm file
//                 // I'll adopt the practice of Netpbm and write the MSB first
//                 uint8_t byte[2] = { static_cast<uint8_t>(val >> 8),
//                                     static_cast<uint8_t>(val & 0xff) };
//                 if (!iowrite(&byte, 2))
//                     return false;
//             } else {
//                 // This must be an 8bit ppm file
//                 uint8_t byte = static_cast<uint8_t>(val);
//                 if (!iowrite(&byte, 1))
//                     return false;
//             }
//         }
//     }
//     return true;
// }