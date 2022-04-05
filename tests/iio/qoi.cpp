#include <catch2/catch_test_macros.hpp>

#include <specula/iio/qoi.hpp>

#include "temp_file.hpp"
#include "test_image.hpp"

using namespace specula::iio;

TEST_CASE("QOIWriter", "[iio][iio::qoi]") {
  SECTION("RGB") {
    QoiSpec spec{{.width = 128, .height = 128}};
    QoiWriter writer(spec);
    std::vector<std::uint8_t> row(spec.width * spec.channels, 0);

    SECTION("Black Image") {
      TemporaryFile tmp(".qoi");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(
            image_black(spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "E24B60D2AD7C6FB63EA15C97FF9F4B2A");
      tmp.remove();
    }

    SECTION("CheckerBoard Image") {
      TemporaryFile tmp(".qoi");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_checker_board(
            spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "52218B4261DD9F7457B516C02274B36");
      tmp.remove();
    }

    SECTION("Gradient Image") {
      TemporaryFile tmp(".qoi");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_gradient(spec.width, spec.height,
                                             spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "3CE781CF7D52CBEB527CD2B847BD8033");
      tmp.remove();
    }
  }

  SECTION("RGBA") {
    QoiSpec spec{{.width = 128, .height = 128, .channels = 4}};
    QoiWriter writer(spec);
    std::vector<std::uint8_t> row(spec.width * spec.channels, 0);

    SECTION("Black Image") {
      TemporaryFile tmp(".qoi");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(
            image_black(spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "75E89E4F479A73BAC1DBADF57C43DB68");
      tmp.remove();
    }

    SECTION("CheckerBoard Image") {
      TemporaryFile tmp(".qoi");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_checker_board(
            spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "C2876D4A25932D40E89C77A0CAB082F9");
      tmp.remove();
    }

    SECTION("Gradient Image") {
      TemporaryFile tmp(".qoi");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_gradient(spec.width, spec.height,
                                             spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "AE508ACFE98D715CD1C6484959D9090A");
      tmp.remove();
    }
  }
}
