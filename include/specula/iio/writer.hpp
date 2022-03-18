#ifndef SPECULA_IIO_WRITER_HPP_
#define SPECULA_IIO_WRITER_HPP_

#include <cstdio>
#include <string>
#include <string_view>

namespace specula::iio {
class ImageWriter {
 public:
  ImageWriter();
  virtual ~ImageWriter();

  bool open(std::string_view file_path);
  bool close();

  virtual bool write_header() = 0;
  virtual bool write_scanline(const void* data) = 0;
  virtual bool write_footer() = 0;

  inline void abort() { _abort = true; }

 protected:
  bool _abort = false;
  std::string _path, _ext;
  FILE* _file;
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_WRITER_HPP_