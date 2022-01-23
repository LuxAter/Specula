#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <windows.h>
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include <argparse/argparse.hpp>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <specula/logging.hpp>
#include <specula/version.hpp>

using namespace specula;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser app("specula", specula::version.to_string());

  std::uint8_t verbosity = 0;

  app.add_argument("-o", "--output")
      .default_value(
          fmt::format("{:%Y-%m-%d-%M-%H-%S}", std::chrono::system_clock::now()))
      .help("output directory to save the image sequence to");
  app.add_argument("-V", "--verbose")
      .action([&](const auto &) { ++verbosity; })
      .append()
      .default_value(false)
      .implicit_value(true)
      .nargs(0)
      .help("set logging verbosity level");

  try {
    app.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << app;
    std::exit(1);
  }

  try {
    logging::sink = std::make_shared<spdlog::sinks::dist_sink_mt>();
    logging::sink->set_pattern(
        "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%#] <%t> %v");

    {
#if defined(_WIN32) || defined(_WIN64)
      const bool use_color = _isatty(_fileno(stdout));
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
      const bool use_color = ::isatty(fileno(stdout)) != 0;
#else
      const bool use_color = false;
#endif

      std::shared_ptr<spdlog::sinks::sink> new_sink = nullptr;
      if (use_color)
        new_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      else
        new_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();

      switch (verbosity) {
      case 0:
        new_sink->set_level(spdlog::level::warn);
        break;
      case 1:
        new_sink->set_level(spdlog::level::info);
        break;
      case 2:
        new_sink->set_level(spdlog::level::debug);
        break;
      case 3:
      default:
        new_sink->set_level(spdlog::level::trace);
        break;
      }
      if (use_color)
        new_sink->set_pattern(
            "[\033[33m%H:%M:%S.%e\033[0m] [\033[1m%n\033[0m] [%^%l%$] "
            "[\033[35m%s\033[0m:\033[36m%#\033[0m:\033[90m%!\033[0m] %v");
      else
        new_sink->set_pattern("[%H:%M:%S.%e] [%n] [%^%l%$] [%s:%#:%!] %v");
      logging::sink->add_sink(new_sink);
    }

    {
      std::shared_ptr<spdlog::sinks::sink> new_sink =
          std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
              "logs/specula.log", 5 * 1024 * 1024, 5);
      new_sink->set_level(spdlog::level::trace);
      new_sink->set_pattern(
          "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%#:%!] %v");
      logging::sink->add_sink(new_sink);
    }

  } catch (const spdlog::spdlog_ex &err) {
    std::cerr << "Log initialization failed: " << err.what() << std::endl;
    std::exit(1);
  }

  LINFO("specula::bin", "Specula v{}", version);

  return 0;
}
