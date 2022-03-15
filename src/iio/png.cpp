#include "png.hpp"

#include <cstdio>
#include <string_view>

#include <specula/common/logging.hpp>

bool specula::iio::PngWriter::write_header() { return false; }
bool specula::iio::PngWriter::write_scanline() { return false; }
bool specula::iio::PngWriter::write_footer() { return false; }