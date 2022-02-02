#include "md5.hpp"

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac)                                    \
  {                                                                 \
    (a) += F((b), (c), (d)) + (x) + static_cast<std::uint32_t>(ac); \
    (a) = ROTATE_LEFT((a), (s));                                    \
    (a) += (b);                                                     \
  }
#define GG(a, b, c, d, x, s, ac)                                    \
  {                                                                 \
    (a) += G((b), (c), (d)) + (x) + static_cast<std::uint32_t>(ac); \
    (a) = ROTATE_LEFT((a), (s));                                    \
    (a) += (b);                                                     \
  }
#define HH(a, b, c, d, x, s, ac)                                    \
  {                                                                 \
    (a) += H((b), (c), (d)) + (x) + static_cast<std::uint32_t>(ac); \
    (a) = ROTATE_LEFT((a), (s));                                    \
    (a) += (b);                                                     \
  }
#define II(a, b, c, d, x, s, ac)                                    \
  {                                                                 \
    (a) += I((b), (c), (d)) + (x) + static_cast<std::uint32_t>(ac); \
    (a) = ROTATE_LEFT((a), (s));                                    \
    (a) += (b);                                                     \
  }

static std::uint8_t PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const char* HEX_CHARS = "0123456789abcdef";

MD5 MD5::from_file(const std::string& filename) {
  MD5 md5;
  FILE* file = fopen(filename.c_str(), "rb");
  if (file == nullptr) {
    throw std::runtime_error("Could not open file " + filename);
  }
  std::uint8_t buffer[1024];
  while (true) {
    std::size_t read = fread(buffer, 1, sizeof(buffer), file);
    if (read == 0) break;
    md5.update(buffer, read);
  }
  fclose(file);
  md5.final();
  return md5;
}

void MD5::init() {
  count[0] = count[1] = 0;
  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;
  finalized = false;
  std::memset(buffer, 0, sizeof(buffer));
  std::memset(digest, 0, sizeof(digest));
  std::memset(digest_str, 0, sizeof(digest_str));
}

void MD5::update(const std::uint8_t* input, std::size_t length) {
  if (finalized == true) throw std::runtime_error("MD5 already finalized");
  std::uint32_t i, index, part_len;
  index = (count[0] >> 3) & 0x3F;

  if ((count[0] += static_cast<std::uint32_t>(length << 3)) <
      static_cast<std::uint32_t>(length << 3))
    count[1]++;
  count[1] += static_cast<std::uint32_t>(length >> 29);

  part_len = 64 - index;

  if (length >= part_len) {
    std::memcpy(&buffer[index], input, part_len);
    transform(buffer);

    for (i = part_len; i + 63 < length; i += 64) transform(&input[i]);

    index = 0;
  } else {
    i = 0;
  }

  std::memcpy(&buffer[index], &input[i], length - i);
}

void MD5::transform(const std::uint8_t block[64]) {
  std::uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
  decode(x, block, 64);

  FF(a, b, c, d, x[0], S11, 0xd76aa478);  /* 1 */
  FF(d, a, b, c, x[1], S12, 0xe8c7b756);  /* 2 */
  FF(c, d, a, b, x[2], S13, 0x242070db);  /* 3 */
  FF(b, c, d, a, x[3], S14, 0xc1bdceee);  /* 4 */
  FF(a, b, c, d, x[4], S11, 0xf57c0faf);  /* 5 */
  FF(d, a, b, c, x[5], S12, 0x4787c62a);  /* 6 */
  FF(c, d, a, b, x[6], S13, 0xa8304613);  /* 7 */
  FF(b, c, d, a, x[7], S14, 0xfd469501);  /* 8 */
  FF(a, b, c, d, x[8], S11, 0x698098d8);  /* 9 */
  FF(d, a, b, c, x[9], S12, 0x8b44f7af);  /* 10 */
  FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

  GG(a, b, c, d, x[1], S21, 0xf61e2562);  /* 17 */
  GG(d, a, b, c, x[6], S22, 0xc040b340);  /* 18 */
  GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);  /* 20 */
  GG(a, b, c, d, x[5], S21, 0xd62f105d);  /* 21 */
  GG(d, a, b, c, x[10], S22, 0x2441453);  /* 22 */
  GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);  /* 24 */
  GG(a, b, c, d, x[9], S21, 0x21e1cde6);  /* 25 */
  GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG(c, d, a, b, x[3], S23, 0xf4d50d87);  /* 27 */
  GG(b, c, d, a, x[8], S24, 0x455a14ed);  /* 28 */
  GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG(d, a, b, c, x[2], S22, 0xfcefa3f8);  /* 30 */
  GG(c, d, a, b, x[7], S23, 0x676f02d9);  /* 31 */
  GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  HH(a, b, c, d, x[5], S31, 0xfffa3942);  /* 33 */
  HH(d, a, b, c, x[8], S32, 0x8771f681);  /* 34 */
  HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH(a, b, c, d, x[1], S31, 0xa4beea44);  /* 37 */
  HH(d, a, b, c, x[4], S32, 0x4bdecfa9);  /* 38 */
  HH(c, d, a, b, x[7], S33, 0xf6bb4b60);  /* 39 */
  HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH(d, a, b, c, x[0], S32, 0xeaa127fa);  /* 42 */
  HH(c, d, a, b, x[3], S33, 0xd4ef3085);  /* 43 */
  HH(b, c, d, a, x[6], S34, 0x4881d05);   /* 44 */
  HH(a, b, c, d, x[9], S31, 0xd9d4d039);  /* 45 */
  HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH(b, c, d, a, x[2], S34, 0xc4ac5665);  /* 48 */

  II(a, b, c, d, x[0], S41, 0xf4292244);  /* 49 */
  II(d, a, b, c, x[7], S42, 0x432aff97);  /* 50 */
  II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II(b, c, d, a, x[5], S44, 0xfc93a039);  /* 52 */
  II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II(d, a, b, c, x[3], S42, 0x8f0ccc92);  /* 54 */
  II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II(b, c, d, a, x[1], S44, 0x85845dd1);  /* 56 */
  II(a, b, c, d, x[8], S41, 0x6fa87e4f);  /* 57 */
  II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II(c, d, a, b, x[6], S43, 0xa3014314);  /* 59 */
  II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II(a, b, c, d, x[4], S41, 0xf7537e82);  /* 61 */
  II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II(c, d, a, b, x[2], S43, 0x2ad7d2bb);  /* 63 */
  II(b, c, d, a, x[9], S44, 0xeb86d391);  /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  std::memset(&x[0], 0, sizeof(x));
}

void MD5::final(std::uint8_t* output) {
  if (finalized == true) throw std::runtime_error("MD5 already finalized");
  std::uint8_t bits[8];
  encode(bits, count, 8);
  std::uint32_t index = (count[0] >> 3) & 0x3f;
  std::uint32_t pad_len = (index < 56) ? (56 - index) : (120 - index);
  update(PADDING, pad_len);
  update(bits, 8);
  encode(digest, state, 16);

  if (output != nullptr) std::memcpy(output, digest, 16);

  for (std::uint8_t i = 0, j = 0; i < 16; i++, j += 2) {
    digest_str[j] = HEX_CHARS[(digest[i] >> 4) & 0xf];
    digest_str[j + 1] = HEX_CHARS[digest[i] & 0xf];
  }
  digest_str[32] = '\0';
  finalized = true;
}

void MD5::reset() {
  state[0] = state[1] = state[2] = state[3] = 0;
  count[0] = count[1] = 0;
  finalized = false;
  std::memset(buffer, 0, sizeof(buffer));
  std::memset(digest, 0, sizeof(digest));
  std::memset(digest_str, 0, sizeof(digest_str));
}

void MD5::encode(std::uint8_t* output, const std::uint32_t* input,
                 std::size_t length) {
  for (std::uint32_t i = 0, j = 0; j < length; ++i, j += 4) {
    output[j] = static_cast<std::uint8_t>(input[i] & 0xff);
    output[j + 1] = static_cast<std::uint8_t>((input[i] >> 8) & 0xff);
    output[j + 2] = static_cast<std::uint8_t>((input[i] >> 16) & 0xff);
    output[j + 3] = static_cast<std::uint8_t>((input[i] >> 24) & 0xff);
  }
}

void MD5::decode(std::uint32_t* output, const std::uint8_t* input,
                 std::size_t length) {
  for (std::uint32_t i = 0, j = 0; j < length; ++i, j += 4)
    output[i] = static_cast<std::uint32_t>(input[j]) |
                static_cast<std::uint32_t>(input[j + 1]) << 8 |
                static_cast<std::uint32_t>(input[j + 2]) << 16 |
                static_cast<std::uint32_t>(input[j + 3]) << 24;
}