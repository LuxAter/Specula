#include <chrono>
#include <cstdlib>
#include <iostream>

#include <argparse/argparse.hpp>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <specula/version.hpp>

int main(int argc, char *argv[]) {
  argparse::ArgumentParser app("specula", specula::version.to_string());

  app.add_argument("-o", "--output")
      .default_value(
          fmt::format("{:%Y-%m-%d-%M-%H-%S}", std::chrono::system_clock::now()))
      .help("output directory to save the image sequence to");

  try {
    app.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << app;
    std::exit(1);
  }
  return 0;
}
