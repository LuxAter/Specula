/**
 * @file qoi.hpp
 * @brief ImageIO QOI image classes
 * @copyright Copyright (c) 2022 - Arden Rasmussen
 */
#ifndef SPECULA_IIO_QOI_HPP_
#define SPECULA_IIO_QOI_HPP_

#include "spec.hpp"
#include "writer.hpp"

namespace specula::iio {
struct QoiSpec : Spec {};

class QoiWriter : public FullImageWriter {
 public:
  QoiWriter(const QoiSpec& spec)
      : FullImageWriter(spec.width, spec.height, spec.channels, 8),
        _spec{spec} {}

  bool write_header() override;
  bool write_footer() override;

 private:
  QoiSpec _spec;
};
}  // namespace specula::iio

#endif  // SPECULA_IIO_QOI_HPP_