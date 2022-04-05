/**
 * @file png.hpp
 * @brief ImageIO PNG image classes
 * @copyright Copyright (c) 2022 - Arden Rasmussen
 *
 * This file defines the ImageIO specializations of specula::iio::Spec,
 * specula::iio::ImageWriter and specula::iio::ImageReader classes for the PNG
 * image format. The actual image reading and writing is handled by
 * [libpng](https://www.libpng.org/pub/png/libpng.html) internally. These
 * classes provide a convienient wrapper around the functionality of the libpng
 * library.
 */
#ifndef SPECULA_IIO_PNG_HPP_
#define SPECULA_IIO_PNG_HPP_

#include <png.h>

#include "spec.hpp"
#include "writer.hpp"

namespace specula::iio {

/**
 * @brief Defines the setting for the PNG image format
 *
 * In addition to the settings defined by the Spec class, this class defines
 * additional fields specific to the PNG image format.
 */
struct PngSpec : Spec {
  std::uint8_t bit_depth = 8;  ///< The number of bits per channel
};

/**
 * @brief PNG image writer class
 *
 * This class is used for write PNG images, using
 * [libpng](http://www.libpng.org/pub/png/libpng.html) internally for the actual
 * image writing.
 */
class PngWriter : public ImageWriter {
 public:
  PngWriter(const PngSpec& spec) : ImageWriter(), _spec(spec) {}

  /**
   * @brief Initializes the PNG write struct, and writes the info header to the
   * file
   *
   * This function creates the png write struct, and configures the error
   * handler for libpng errors to be logged correctly. It then constrcts the
   * info struct and fills in the data provided in the PngSpec struct.
   *
   * The Spec.channels controls type color type for the PNG image. And the
   * PngSpec.bit_depth sets the number of bits per channel. Only certain
   * combindations of these are supported, see the table below for the valid
   * combindations.
   * | Channels | PNG Color Type | Bit Depth |
   * | -------- | -------------- | --------- |
   * | 1        | PNG_COLOR_TYPE_GRAY | 1, 2, 4, 8, 16 |
   * | 2        | PNG_COLOR_TYPE_GRAY_ALPHA | 8, 16 |
   * | 3        | PNG_COLOR_TYPE_RGB | 1, 2, 4, 8 |
   * | 4        | PNG_COLOR_TYPE_RGB_ALPHA | 8, 16 |
   *
   * @return true if the info header was successfully written
   * @return false if there was an error writing the info header or
   * initializaing the write struct
   */
  bool write_header() override;
  /**
   * @brief Writes a single scanline of the image to the file
   *
   * This function may only be called after the write_header() function has
   * been.
   *
   * This function assumes that the data provided is a byte array, with the
   * channels in order (i.e. RGBRGBRGB...), and that each channel is represented
   * by the number of bits specified in the PngSpec.bit_depth field.
   *
   * @param data A pointer to a byte array of the scanline data to write to the
   * image
   * @return true if the scanline was successfully written
   * @return false if there was an error while writing the scanline
   */
  bool write_scanline(const void* data) override;
  /**
   * @brief Writes the end of the PNG image and destroys the write structs
   *
   * This function must be the last called function in the image writing, as it
   * destroys the write structs. Any calls to write_scanline() after this will
   * fail.
   *
   * @return true if the image was successfully written
   * @return false if there was an error while ending the image
   */
  bool write_footer() override;

 private:
  PngSpec _spec;
  png_structp _ptr;
  png_infop _info;
  png_colorp _palette;
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_PNG_HPP_