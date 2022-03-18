/**
 * @file writer.hpp
 * @brief ImageIO base image writer
 * @copyright Copyright (c) 2022 - Arden Rasmussen
 *
 * This file defines the base ImageWriter class, which is used as a base for the
 * specialized image writing classes.
 */
#ifndef SPECULA_IIO_WRITER_HPP_
#define SPECULA_IIO_WRITER_HPP_

#include <cstdio>
#include <string>
#include <string_view>

namespace specula::iio {

/**
 * @brief Base image writer class
 *
 * This class defines the interface for writing images, and is inteded to be
 * inherited by the other image writer classes. This class should not be used
 * directly as the methods are not implemented.
 *
 * This class also manages the file pointer and a few other common
 * functionalities for the specialized image writers.
 *
 * The ImageWriter class should not be used directly. Instead, using the
 * ImageOutput class is preferred.
 */
class ImageWriter {
 public:
  ImageWriter();
  virtual ~ImageWriter();

  /**
   * @brief Opens a file for writing an image to
   *
   * @param file_path The path to the file to write to
   * @return true if the file was successfully opened
   * @return false if there was an error opening the file
   */
  bool open(std::string_view file_path);
  /**
   * @brief Closes the any currently open file
   *
   * If no file is currently open (i.e. the open() method has not been called),
   * then this method will do nothing.
   *
   * @return this function will always return true
   */
  bool close();

  /**
   * @brief Writes the image format header to the file
   *
   * The implementation of this method is different for each image format. For
   * further details see the documentation for the specific image format.
   *
   * @return true if the image header was successfully written
   * @return false if there wasn an error writing the image header or
   * initializaing the specialized image writer
   */
  virtual bool write_header() = 0;
  /**
   * @brief Writes a single scanline of the image to the file
   *
   * The implementation of this method is different for each image format. For
   * further details see the documentation for the specific image format.
   *
   * @param data A pointer to the scanline data formated in the image format
   * writers prefered format.
   * @return true if the scanline was successfully written
   * @return false if there was an error writing the scanline
   */
  virtual bool write_scanline(const void* data) = 0;
  /**
   * @brief Writes the image format footer to the file
   *
   * The implementation of this method is different for each image format. For
   * further details see the documentation for the specific image format.
   *
   * @return true if the image footer was successfully written
   * @return false if there was an error writing the image footer
   */
  virtual bool write_footer() = 0;

  /**
   * @brief Aborts the writing process, and closes the file
   *
   * This method will abort the writing process, and close the file. This is
   * only to be used for fatal errors, the return values from write_header(),
   * write_scanline(), and write_footer() should be used when possible.
   *
   * After this function is executed, no other writing methods should be called.
   * And if any are, then they will do nothing, and return `false`.
   */
  inline void abort() {
    if (_file != nullptr) fclose(_file);
    _file = nullptr;
    _abort = true;
  }

 protected:
  bool _abort;        ///< Whether or not the writing process should be aborted
  std::string _path;  ///< The path to the file being written to
  std::string _ext;   ///< The extension of the file being written to
  FILE* _file = nullptr;  ///< The file pointer to the file being written to
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_WRITER_HPP_