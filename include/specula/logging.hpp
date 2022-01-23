#ifndef VISI_LOGGING_HPP_
#define VISI_LOGGING_HPP_

#include <memory>

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

#ifndef DEFAULT_LOGGER
#define DEFAULT_LOGGER "specula"
#endif

#define __LOG_LST(logger, timepoint, file, line, func, level, ...)             \
  ::logging::get(logger)->log(timepoint, spdlog::source_loc{file, line, func}, \
                              level, fmt::format(__VA_ARGS__))

#define __LOG_LS(logger, file, line, func, level, ...)                         \
  ::logging::get(logger)->log(spdlog::source_loc{file, line, func}, level,     \
                              fmt::format(__VA_ARGS__))

#define __LOG_LT(logger, timepoint, level, ...)                                \
  ::logging::get(logger)->log(                                                 \
      timepoint, spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level,  \
      fmt::format(__VA_ARGS__))

#define __LOG_ST(timepoint, file, line, func, level, ...)                      \
  ::logging::get(DEFAULT_LOGGER)                                               \
      ->log(timepoint, spdlog::source_loc{file, line, func}, level,            \
            fmt::format(__VA_ARGS__))

#define __LOG_L(logger, level, ...)                                            \
  ::logging::get(logger)->log(                                                 \
      spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level,             \
      fmt::format(__VA_ARGS__))

#define __LOG_T(timepoint, level, ...)                                         \
  ::logging::get(DEFAULT_LOGGER)                                               \
      ->log(timepoint, spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__},   \
            level, fmt::format(__VA_ARGS__))

#define __LOG_S(file, line, func, level, ...)                                  \
  ::logging::get(DEFAULT_LOGGER)                                               \
      ->log(spdlog::source_loc{file, line, func}, level,                       \
            fmt::format(__VA_ARGS__))

#define __LOG(level, ...)                                                      \
  ::logging::get(DEFAULT_LOGGER)                                               \
      ->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level,       \
            fmt::format(__VA_ARGS__))

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define LLTRACE(...) __LOG(spdlog::level::trace, __VA_ARGS__)
#define LSLTRACE(file, line, function, ...)                                    \
  __LOG_S(file, line, function, spdlog::level::trace, __VA_ARGS__)
#define LTRACE(logger, ...) __LOG_L(logger, spdlog::level::trace, __VA_ARGS__)
#define LSTRACE(logger, file, line, func, ...)                                 \
  __LOG_LS(logger, file, line, func, spdlog::level::trace, __VA_ARGS__)
#else
#define LLTRACE(...)
#define LSLTRACE(file, line, function, ...)
#define LTRACE(logger, ...)
#define LSTRACE(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define LLDEBUG(...) __LOG(spdlog::level::debug, __VA_ARGS__)
#define LSLDEBUG(file, line, function, ...)                                    \
  __LOG_S(file, line, function, spdlog::level::debug, __VA_ARGS__)
#define LDEBUG(logger, ...) __LOG_L(logger, spdlog::level::debug, __VA_ARGS__)
#define LSDEBUG(logger, file, line, func, ...)                                 \
  __LOG_LS(logger, file, line, func, spdlog::level::debug, __VA_ARGS__)
#else
#define LLDEBUG(...)
#define LSLDEBUG(file, line, function, ...)
#define LDEBUG(logger, ...)
#define LSDEBUG(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define LLINFO(...) __LOG(spdlog::level::info, __VA_ARGS__)
#define LSLINFO(file, line, function, ...)                                     \
  __LOG_S(file, line, function, spdlog::level::info, __VA_ARGS__)
#define LINFO(logger, ...) __LOG_L(logger, spdlog::level::info, __VA_ARGS__)
#define LSINFO(logger, file, line, func, ...)                                  \
  __LOG_LS(logger, file, line, func, spdlog::level::info, __VA_ARGS__)
#else
#define LLINFO(...)
#define LSLINFO(file, line, function, ...)
#define LINFO(logger, ...)
#define LSINFO(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define LLWARN(...) __LOG(spdlog::level::warn, __VA_ARGS__)
#define LSLWARN(file, line, function, ...)                                     \
  __LOG_S(file, line, function, spdlog::level::warn, __VA_ARGS__)
#define LWARN(logger, ...) __LOG_L(logger, spdlog::level::warn, __VA_ARGS__)
#define LSWARN(logger, file, line, func, ...)                                  \
  __LOG_LS(logger, file, line, func, spdlog::level::warn, __VA_ARGS__)
#else
#define LLWARN(...)
#define LSLWARN(file, line, function, ...)
#define LWARN(logger, ...)
#define LSWARN(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define LLERROR(...) __LOG(spdlog::level::err, __VA_ARGS__)
#define LSLERROR(file, line, function, ...)                                    \
  __LOG_S(file, line, function, spdlog::level::err, __VA_ARGS__)
#define LERROR(logger, ...) __LOG_L(logger, spdlog::level::err, __VA_ARGS__)
#define LSERROR(logger, file, line, func, ...)                                 \
  __LOG_LS(logger, file, line, func, spdlog::level::err, __VA_ARGS__)
#else
#define LLERROR(...)
#define LSLERROR(file, line, function, ...)
#define LERROR(logger, ...)
#define LSERROR(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define LLCRITICAL(...) __LOG(spdlog::level::critical, __VA_ARGS__)
#define LSLCRITICAL(file, line, function, ...)                                 \
  __LOG_S(file, line, function, spdlog::level::critical, __VA_ARGS__)
#define LCRITICAL(logger, ...)                                                 \
  __LOG_L(logger, spdlog::level::critical, __VA_ARGS__)
#define LSCRITICAL(logger, file, line, func, ...)                              \
  __LOG_LS(logger, file, line, func, spdlog::level::critical, __VA_ARGS__)
#else
#define LLCRITICAL(...)
#define LSLCRITICAL(file, line, function, ...)
#define LCRITICAL(logger, ...)
#define LSCRITICAL(logger, file, line, func, ...)
#endif

namespace specula::logging {
extern std::shared_ptr<spdlog::sinks::dist_sink_mt> sink;

inline std::shared_ptr<spdlog::logger> get(const std::string &name) {
  std::shared_ptr<spdlog::logger> logger = spdlog::get(name);
  if (logger == nullptr) {
    if (sink != nullptr)
      logger = std::make_shared<spdlog::logger>(name, sink);
    else
      logger = std::make_shared<spdlog::logger>(name);
    logger->set_level(spdlog::level::level_enum::trace);
    spdlog::register_logger(logger);
  }
  return logger;
}
} // namespace specula::logging

#endif
