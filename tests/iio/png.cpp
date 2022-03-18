#include <catch2/catch_test_macros.hpp>

#include <specula/iio/png.hpp>

#include "temp_file.hpp"
#include "test_image.hpp"

using namespace specula::iio;

TEST_CASE("PNGWriter", "[iio][iio::png]") {
  SECTION("Grayscale") {
    PngSpec spec{{.width{128}, .height{128}, .channels{1}}};
    PngWriter writer(spec);
    std::vector<std::uint8_t> row(spec.width * spec.channels, 0);

    SECTION("Black Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(
            image_black(spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "AEB8248EC7E4ACC9528E7770E42C5CCA");
      tmp.remove();
    }

    SECTION("CheckerBoard Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_checker_board(
            spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "822BEF644804609BDCA398E4142A7CCD");
      tmp.remove();
    }

    SECTION("Gradient Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_gradient(spec.width, spec.height,
                                             spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "FF9AACB5D329C4F84C2208AE46C3AB8D");
      tmp.remove();
    }
  }

  SECTION("Grayscale Alpha") {
    PngSpec spec{{.width{128}, .height{128}, .channels{2}}};
    PngWriter writer(spec);
    std::vector<std::uint8_t> row(spec.width * spec.channels, 0);

    SECTION("Black Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(
            image_black(spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "8F16C2BBD6BDF3CBD83C04B2C862B6EB");
      tmp.remove();
    }

    SECTION("CheckerBoard Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_checker_board(
            spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "6C76CD4BEA664145C7839F54E56CC6D0");
      tmp.remove();
    }

    SECTION("Gradient Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_gradient(spec.width, spec.height,
                                             spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "762AFEE4D6DB82F4B47DE055243E5A");
      tmp.remove();
    }
  }

  SECTION("RGB") {
    PngSpec spec{{.width{128}, .height{128}}};
    PngWriter writer(spec);
    std::vector<std::uint8_t> row(spec.width * spec.channels, 0);

    SECTION("Black Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(
            image_black(spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "3EEDA640EBD06D09A03ABD5A235B42EF");
      tmp.remove();
    }

    SECTION("CheckerBoard Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_checker_board(
            spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "10C98FB53B17BB384E5A286CFC3F9909");
      tmp.remove();
    }

    SECTION("Gradient Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_gradient(spec.width, spec.height,
                                             spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "ECF6CBF83592781FD76199ED9F50B184");
      tmp.remove();
    }
  }

  SECTION("RGBA") {
    PngSpec spec{{.width{128}, .height{128}, .channels{4}}};
    PngWriter writer(spec);
    std::vector<std::uint8_t> row(spec.width * spec.channels, 0);

    SECTION("Black Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(
            image_black(spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "E8CA1F11C1EF20208CFE1467C4CFB9FE");
      tmp.remove();
    }

    SECTION("CheckerBoard Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_checker_board(
            spec.width, spec.height, spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "57B24BF87582AA5B3C9E5C371B43BD2");
      tmp.remove();
    }

    SECTION("Gradient Image") {
      TemporaryFile tmp(".png");

      writer.open(tmp.name);
      writer.write_header();
      for (std::uint32_t i = 0; i < spec.height; ++i) {
        writer.write_scanline(image_gradient(spec.width, spec.height,
                                             spec.channels, i, row.data()));
      }
      writer.write_footer();
      writer.close();

      REQUIRE(tmp.hash() == "1C719DB96F18A8FE2C954A88F96B8CBE");
      tmp.remove();
    }
  }
}