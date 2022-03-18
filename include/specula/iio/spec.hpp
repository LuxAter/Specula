/**
 * @file spec.hpp
 * @brief ImageIO base image specification data struct
 * @copyright Copyright (c) 2022 - Arden Rasmussen
 *
 * This file defines the base image specification data structure, which is used
 * for specifying/reading metadata about the image, such as the size or number
 * of channels.
 */
#ifndef SPECULA_IIO_SPEC_HPP_
#define SPECULA_IIO_SPEC_HPP_

#include <cstdint>

namespace specula::iio {

/**
 * @brief Defines the general setting for the image format
 *
 * Specifies details about the image format, such as the width, height, and the
 * number of channels.
 */
struct Spec {
  std::uint32_t width;        ///< The width of the image in pixels
  std::uint32_t height;       ///< The height of the image in pixels
  std::uint8_t channels = 3;  ///< The number of channels in the image
};
}  // namespace specula::iio

#endif