#include <chrono>
#include <filesystem>
#include <string>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

#include <specula/iio/pnm.hpp>
#include <specula/md5.hpp>

using namespace specula;

TEST_CASE("PNM Image Writer", "[iio]") {
  SECTION("P2") {
    std::string fname =
        MD5(std::chrono::system_clock::now()).to_string() + ".pnm";

    auto writer = iio::PnmWriter({255, 255, 1, 1}, false);
    if (writer.open(fname)) {
      CHECK(writer.write_header());
      std::uint8_t buffer[255] = {0};
      for (std::uint8_t i = 0; i < 255; ++i) {
        for (std::uint8_t j = 0; j < 255; ++j) {
          buffer[j] = i / 2 + j / 2;
        }
        writer.write_scanline(i, 0, buffer);
      }
      CHECK(writer.close());
    }

    REQUIRE(MD5::from_file(fname).to_string() ==
            "8d2e5410d7ba57eb7ddb44740bdb783c");

    CHECK(std::filesystem::remove(fname));
  }
  SECTION("P3") {
    std::string fname =
        MD5(std::chrono::system_clock::now()).to_string() + ".pnm";

    auto writer = iio::PnmWriter({255, 255, 1, 3}, false);
    if (writer.open(fname)) {
      CHECK(writer.write_header());
      std::uint8_t buffer[255 * 3] = {0};
      for (std::uint8_t i = 0; i < 255; ++i) {
        for (std::uint8_t j = 0; j < 255; ++j) {
          buffer[j * 3 + 0] = i;
          buffer[j * 3 + 1] = j;
          buffer[j * 3 + 2] = 0;
        }
        writer.write_scanline(i, 0, buffer);
      }
      CHECK(writer.close());
    }

    REQUIRE(MD5::from_file(fname).to_string() ==
            "950673f3cbf2dd624c04a722df28bb31");

    CHECK(std::filesystem::remove(fname));
  }
  SECTION("P5") {
    std::string fname =
        MD5(std::chrono::system_clock::now()).to_string() + ".pnm";

    auto writer = iio::PnmWriter({255, 255, 1, 1}, true);
    if (writer.open(fname)) {
      CHECK(writer.write_header());
      std::uint8_t buffer[255] = {0};
      for (std::uint8_t i = 0; i < 255; ++i) {
        for (std::uint8_t j = 0; j < 255; ++j) {
          buffer[j] = i / 2 + j / 2;
        }
        writer.write_scanline(i, 0, buffer);
      }
      CHECK(writer.close());
    }

    REQUIRE(MD5::from_file(fname).to_string() ==
            "963cf907cae12976b8bd3a71dfd0862b");

    CHECK(std::filesystem::remove(fname));
  }
  SECTION("P6") {
    std::string fname =
        MD5(std::chrono::system_clock::now()).to_string() + ".pnm";

    auto writer = iio::PnmWriter({255, 255, 1, 3}, true);
    if (writer.open(fname)) {
      CHECK(writer.write_header());
      std::uint8_t buffer[255 * 3] = {0};
      for (std::uint8_t i = 0; i < 255; ++i) {
        for (std::uint8_t j = 0; j < 255; ++j) {
          buffer[j * 3 + 0] = i;
          buffer[j * 3 + 1] = j;
          buffer[j * 3 + 2] = 0;
        }
        writer.write_scanline(i, 0, buffer);
      }
      CHECK(writer.close());
    }

    REQUIRE(MD5::from_file(fname).to_string() ==
            "609d2acb472dbc8aa1c85bdfb7614507");

    CHECK(std::filesystem::remove(fname));
  }
  SECTION("P7") {
    SECTION("GRAYSCALE_ALPHA") {
      std::string fname =
          MD5(std::chrono::system_clock::now()).to_string() + ".pnm";

      auto writer = iio::PnmWriter({255, 255, 1, 2});
      if (writer.open(fname)) {
        CHECK(writer.write_header());
        std::uint8_t buffer[255 * 2] = {0};
        for (std::uint8_t i = 0; i < 255; ++i) {
          for (std::uint8_t j = 0; j < 255; ++j) {
            buffer[j * 2 + 0] = i / 2 + j / 2;
            buffer[j * 2 + 1] = 255 - (i >= j ? i - j : j - i);
          }
          writer.write_scanline(i, 0, buffer);
        }
        CHECK(writer.close());
      }

      REQUIRE(MD5::from_file(fname).to_string() ==
              "4427e0ebbdef25323ca7ab9353dee5b9");

      CHECK(std::filesystem::remove(fname));
    }
    SECTION("RGB_ALPHA") {
      std::string fname =
          MD5(std::chrono::system_clock::now()).to_string() + ".pnm";

      auto writer = iio::PnmWriter({255, 255, 1, 4});
      if (writer.open(fname)) {
        CHECK(writer.write_header());
        std::uint8_t buffer[255 * 4] = {0};
        for (std::uint8_t i = 0; i < 255; ++i) {
          for (std::uint8_t j = 0; j < 255; ++j) {
            buffer[j * 4 + 0] = i;
            buffer[j * 4 + 1] = j;
            buffer[j * 4 + 2] = 0;
            buffer[j * 4 + 3] = i / 2 + j / 2;
          }
          writer.write_scanline(i, 0, buffer);
        }
        CHECK(writer.close());
      }

      REQUIRE(MD5::from_file(fname).to_string() ==
              "141809d5f562e273903c463039484059");

      CHECK(std::filesystem::remove(fname));
    }
  }
}