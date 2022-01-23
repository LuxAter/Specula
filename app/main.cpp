#include <argparse/argparse.hpp>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
  argparse::ArgumentParser app("specula", "0.1.0");

  try {
    app.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << app;
    std::exit(1);
  }
  return 0;
}
