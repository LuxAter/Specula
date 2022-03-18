#include "png.hpp"

#include <cstdio>
#include <string_view>

#include <specula/common/logging.hpp>

static void write_error_handler(png_structp png, png_const_charp data) {
  specula::iio::PngWriter* writer =
      reinterpret_cast<specula::iio::PngWriter*>(png_get_error_ptr(png));
  LERROR("iio", "Error while writing PNG file: {}", data);
  if (writer != nullptr) writer->abort();
}

bool specula::iio::PngWriter::write_header() {
  if (_abort) return false;

  _ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, this,
                                 write_error_handler, NULL);
  if (_ptr == nullptr) {
    LERROR("iio", "Failed to create PNG write struct");
    return false;
  }

  _info = png_create_info_struct(_ptr);
  if (_info == nullptr) {
    LERROR("iio", "Failed to create PNG info struct");
    png_destroy_write_struct(&_ptr, nullptr);
    return false;
  }

  if (setjmp(png_jmpbuf(_ptr))) return false;

  png_init_io(_ptr, _file);

  auto color_type = PNG_COLOR_TYPE_RGB;
  switch (_spec.channels) {
    case 1:
      color_type = PNG_COLOR_TYPE_GRAY;
      break;
    case 2:
      color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
      break;
    case 3:
      color_type = PNG_COLOR_TYPE_RGB;
      break;
    case 4:
      color_type = PNG_COLOR_TYPE_RGB_ALPHA;
      break;
    default:
      LWARN("iio",
            "PNG images only support 1-4 channels, images with {} channels are "
            "not support",
            _spec.channels);
      png_destroy_write_struct(&_ptr, &_info);
      return false;
  }

  png_set_IHDR(_ptr, _info, _spec.width, _spec.height, _spec.bit_depth,
               color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_write_info(_ptr, _info);

  return true;
}
bool specula::iio::PngWriter::write_scanline(const void* data) {
  if (_abort || _ptr == nullptr) return false;
  if (setjmp(png_jmpbuf(_ptr))) return false;
  png_write_row(_ptr, reinterpret_cast<png_const_bytep>(data));
  return true;
}
bool specula::iio::PngWriter::write_footer() {
  if (_abort || _ptr == nullptr) return false;
  if (setjmp(png_jmpbuf(_ptr))) return false;
  png_write_end(_ptr, _info);
  png_destroy_write_struct(&_ptr, &_info);
  return true;
}