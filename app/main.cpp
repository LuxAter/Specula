#include <cstdint>
#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <windows.h>
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include <cxxopts.hpp>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <sycl/sycl.hpp>

#include <specula/logging.hpp>
#include <specula/version.hpp>

#include "sysinfo.hpp"

#ifdef _WIN32
const std::string LQUOTE("\'");
const std::string RQUOTE("\'");
#else
const std::string LQUOTE("‘");
const std::string RQUOTE("’");
#endif

using namespace specula;
using namespace sycl;

namespace cxxopts {
class argument_invalid_choice : public cxxopts::OptionParseException {
 public:
  explicit argument_invalid_choice(const std::string &option,
                                   const std::string &arg,
                                   const std::vector<std::string> &choices)
      : OptionParseException(fmt::format(
            "Option {}{}{} invalid argument {}{}{} (choose from {})", LQUOTE,
            option, RQUOTE, LQUOTE, arg, RQUOTE, choices)) {}
};
}  // namespace cxxopts

int main(int argc, char *argv[]) {
  try {
    cxxopts::Options options("Specula", "C++17 Pathtracing Renderer");

    // clang-format off
    options.add_options()
      ("h,help", "shows this help message and exits")
      ("v,version", "prints version information and exits")
      ("V,verbose", "enable additional logging verbosity");
  
    options.add_options("Command")
      ("system-info", "prints system information and exits",
          cxxopts::value<std::string>()->implicit_value("toml"));
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result.count("system-info")) {
      static const std::vector<std::string> system_info_choices = {
          "toml", "json", "yaml"};

      std::string val = result["system-info"].as<std::string>();
      transform(val.begin(), val.end(), val.begin(), ::tolower);
      if (std::find(system_info_choices.begin(), system_info_choices.end(),
                    val) == system_info_choices.end())
        throw cxxopts::argument_invalid_choice(
            "system-info", result["system-info"].as<std::string>(),
            system_info_choices);
    }

    if (result.count("help") != 0L) {
      std::cout << options.help() << std::endl;
      std::exit(0);
    } else if (result.count("version") != 0L) {
      std::cout << specula::version.to_string() << std::endl;
      std::exit(0);
    } else if (result.count("system-info") != 0L) {
      std::exit(specula::cli::sysinfo(
          std::move(result["system-info"].as<std::string>())));
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

        switch (result.count("verbose")) {
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

    const auto platforms = platform::get_platforms();
    for (auto &it : platforms) {
      LINFO("specula::bin", "Platform: {}",
            it.get_info<info::platform::name>());
    }

    return 0;

  } catch (const cxxopts::OptionException &err) {
    std::cerr << err.what() << std::endl;
    std::exit(1);
  }
}
