#include "qoi.hpp"

#include <qoi.h>

#include <specula/common/logging.hpp>

bool specula::iio::QoiWriter::write_header() {
  switch (_spec.channels) {
    case 3:
      break;
    case 4:
      break;
    default:
      LWARN("iio",
            "QOI images only support 3-4 channels, images with {} channels are "
            "not supported",
            _spec.channels);
      return false;
  }
  return true;
}

bool specula::iio::QoiWriter::write_footer() {
  void* encoded = nullptr;
  int size;

  qoi_desc desc = {.width = _spec.width,
                   .height = _spec.height,
                   .channels = _spec.channels,
                   .colorspace = QOI_SRGB};

  encoded = qoi_encode(_data, &desc, &size);

  if (encoded == nullptr) {
    LWARN("iio", "Error while encoding QOI image file");
    return false;
  }
  if (fwrite(encoded, 1, size, _file) != static_cast<std::size_t>(size)) {
    LWARN("iio", "Failed to write all image data to file");
    return false;
  }

  free(encoded);
  return true;
}