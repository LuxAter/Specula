#ifndef SPECULA_TEST_TEST_IMAGE_HPP_
#define SPECULA_TEST_TEST_IMAGE_HPP_

#include <cmath>
#include <limits>
#include <type_traits>

#include <fmt/core.h>

template <typename T>
T convert_float(float val) {
  if constexpr (std::is_floating_point_v<T>) {
    return static_cast<T>(val);
  } else {
    return static_cast<T>(static_cast<float>(std::numeric_limits<T>::max()) *
                          val);
  }
}

template <typename T>
T* image_black(std::size_t width, std::size_t, std::size_t channels,
               std::size_t, T* data) {
  std::fill(data, data + (width * channels), 0);
  return data;
}

template <typename T>
T* image_checker_board(std::size_t width, std::size_t, std::size_t channels,
                       std::size_t y, T* data) {
  for (std::size_t x = 0; x < width; ++x) {
    for (std::size_t c = 0; c < channels; ++c) {
      if ((y / (2 << (2 + c))) % 2 != (x / (2 << (2 + c))) % 2)
        data[x * channels + c] = convert_float<T>(1.0f);
      else
        data[x * channels + c] = convert_float<T>(0.0f);
    }
  }
  return data;
}

template <typename T>
T* image_gradient(std::size_t width, std::size_t height, std::size_t channels,
                  std::size_t y, T* data) {
  float ga = static_cast<float>(y) / static_cast<float>(height);
  float gc = (1.0f - ga);
  for (std::size_t x = 0; x < width; ++x) {
    float gb = static_cast<float>(x) / static_cast<float>(width);
    float gd = (1.0f - gb);
    for (std::size_t c = 0; c < channels; ++c) {
      if (c % 4 == 0)
        data[x * channels + c] = convert_float<T>(ga);
      else if (c % 4 == 1)
        data[x * channels + c] = convert_float<T>(gb);
      else if (c % 4 == 2)
        data[x * channels + c] = convert_float<T>(gc);
      else if (c % 4 == 3)
        data[x * channels + c] = convert_float<T>(gd);
    }
  }
  return data;
}

#endif