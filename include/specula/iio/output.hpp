#ifndef SPECULA_IIO_OUTPUT_HPP_
#define SPECULA_IIO_OUTPUT_HPP_

namespace specula::iio {
class Output {
 public:
  Output();

  bool open();
  bool is_open() const;

 private:
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_OUTPUT_HPP_