#ifndef SPECULA_MD5_HPP
#define SPECULA_MD5_HPP

#include <concepts>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#include "specula/concepts/container.hpp"

class MD5 {
 public:
  MD5() { init(); }

  template <typename T>
  MD5(const T& input) {
    init();
    update(input);
    final();
  }

  ~MD5() { reset(); }

  void init();
  void update(const std::uint8_t* input, std::size_t length);
  inline void update(const char* input, std::size_t length) {
    return update(reinterpret_cast<const std::uint8_t*>(input), length);
  }
  inline void update(const char* input) {
    return update(reinterpret_cast<const std::uint8_t*>(input), strlen(input));
  }
  inline void update(const std::string& input) {
    return update(reinterpret_cast<const std::uint8_t*>(input.data()),
                  input.size());
  }
  inline void update(const std::string_view& input) {
    return update(reinterpret_cast<const std::uint8_t*>(input.data()),
                  input.size());
  }
  template <typename T>
  requires specula::container<T>
  inline void update(const T& input) {
    for (auto it = input.begin(); it != input.end(); ++it)
      update(reinterpret_cast<const std::uint8_t*>(&(*it)),
             sizeof(typename T::value_type));
  }
  template <typename T>
  requires(not specula::container<T>) inline void update(const T& input) {
    return update(reinterpret_cast<const std::uint8_t*>(&input), sizeof(T));
  }

  void transform(const std::uint8_t block[64]);
  void final(std::uint8_t* output = nullptr);
  void reset();

  inline std::string to_string() const { return std::string(digest_str); }

  inline operator std::string() const { return to_string(); }

 private:
  void encode(std::uint8_t* output, const std::uint32_t* input,
              std::size_t length);
  void decode(std::uint32_t* output, const std::uint8_t* input,
              std::size_t length);

  std::uint32_t state[4], count[2];
  std::uint8_t buffer[64];

  bool finalized = false;
  std::uint8_t digest[16];
  char digest_str[33];
};

#endif  // SPECULA_MD5_HPP