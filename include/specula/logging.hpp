/** @file logging.hpp
 * @brief Specula logging handler wrapping spdlog
 *
 * This header provides the common interface to specula's logging interface.
 * The actuall implementation is provided by
 * [spdlog](https://github.com/gabime/spdlog). Included in this file are a
 * number of useful macros for implementing a common interface for logging, and
 * managing the different possible invocation methods for the loggers.
 */
#ifndef VISI_LOGGING_HPP_
#define VISI_LOGGING_HPP_

#include <memory>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

#ifndef DEFAULT_LOGGER
#define DEFAULT_LOGGER "specula"
#endif

/**
 * @brief Internal wrapper for a log message including logger, source location,
 * and timestamp
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_LST(logger, timepoint, file, line, func, level, ...)           \
  ::specula::logging::get(logger)->log(timepoint,                            \
                                       spdlog::source_loc{file, line, func}, \
                                       level, fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message including logger and source
 * location
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_LS(logger, file, line, func, level, ...)                       \
  ::specula::logging::get(logger)->log(spdlog::source_loc{file, line, func}, \
                                       level, fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message including logger and timestamp
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_LT(logger, timepoint, level, ...)                               \
  ::specula::logging::get(logger)->log(                                       \
      timepoint, spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level, \
      fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message including source location and
 * timestamp
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_ST(timepoint, file, line, func, level, ...)           \
  ::specula::logging::get(DEFAULT_LOGGER)                           \
      ->log(timepoint, spdlog::source_loc{file, line, func}, level, \
            fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message including only the logger
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_L(logger, level, ...)                                \
  ::specula::logging::get(logger)->log(                            \
      spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level, \
      fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message including only the timestamp
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_T(timepoint, level, ...)                                       \
  ::specula::logging::get(DEFAULT_LOGGER)                                    \
      ->log(timepoint, spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, \
            level, fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message including only the source location
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG_S(file, line, func, level, ...)            \
  ::specula::logging::get(DEFAULT_LOGGER)                \
      ->log(spdlog::source_loc{file, line, func}, level, \
            fmt::format(__VA_ARGS__))

/**
 * @brief Internal wrapper for a log message that includes _no_ additional
 * information
 *
 * @note This should not be directly used, instead use the `L*<LEVEL>` macros.
 *
 * @see logging::get()
 */
#define __LOG(level, ...)                                                \
  ::specula::logging::get(DEFAULT_LOGGER)                                \
      ->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, level, \
            fmt::format(__VA_ARGS__))

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
/// @brief Log a trace message using default values
#define LLTRACE(...) __LOG(spdlog::level::trace, __VA_ARGS__)
/// @brief Log a trace message with specified source location
#define LSLTRACE(file, line, function, ...) \
  __LOG_S(file, line, function, spdlog::level::trace, __VA_ARGS__)
/// @brief Log a trace message with specified logger
#define LTRACE(logger, ...) __LOG_L(logger, spdlog::level::trace, __VA_ARGS__)
/// @brief Log a trace message with specified logger and source location
#define LSTRACE(logger, file, line, func, ...) \
  __LOG_LS(logger, file, line, func, spdlog::level::trace, __VA_ARGS__)
#else
#define LLTRACE(...)
#define LSLTRACE(file, line, function, ...)
#define LTRACE(logger, ...)
#define LSTRACE(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
/// @brief Log a debug message using default values
#define LLDEBUG(...) __LOG(spdlog::level::debug, __VA_ARGS__)
/// @brief Log a debug message with specified source location
#define LSLDEBUG(file, line, function, ...) \
  __LOG_S(file, line, function, spdlog::level::debug, __VA_ARGS__)
/// @brief Log a debug message with specified logger
#define LDEBUG(logger, ...) __LOG_L(logger, spdlog::level::debug, __VA_ARGS__)
/// @brief Log a debug message with specified logger and source location
#define LSDEBUG(logger, file, line, func, ...) \
  __LOG_LS(logger, file, line, func, spdlog::level::debug, __VA_ARGS__)
#else
#define LLDEBUG(...)
#define LSLDEBUG(file, line, function, ...)
#define LDEBUG(logger, ...)
#define LSDEBUG(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
/// @brief Log a info message using default values
#define LLINFO(...) __LOG(spdlog::level::info, __VA_ARGS__)
/// @brief Log a info message with specified source location
#define LSLINFO(file, line, function, ...) \
  __LOG_S(file, line, function, spdlog::level::info, __VA_ARGS__)
/// @brief Log a info message with specified logger
#define LINFO(logger, ...) __LOG_L(logger, spdlog::level::info, __VA_ARGS__)
/// @brief Log a info message with specified logger and source location
#define LSINFO(logger, file, line, func, ...) \
  __LOG_LS(logger, file, line, func, spdlog::level::info, __VA_ARGS__)
#else
#define LLINFO(...)
#define LSLINFO(file, line, function, ...)
#define LINFO(logger, ...)
#define LSINFO(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
/// @brief Log a warning message using default values
#define LLWARN(...) __LOG(spdlog::level::warn, __VA_ARGS__)
/// @brief Log a warning message with specified source location
#define LSLWARN(file, line, function, ...) \
  __LOG_S(file, line, function, spdlog::level::warn, __VA_ARGS__)
/// @brief Log a warning message with specified logger
#define LWARN(logger, ...) __LOG_L(logger, spdlog::level::warn, __VA_ARGS__)
/// @brief Log a warning message with specified logger and source location
#define LSWARN(logger, file, line, func, ...) \
  __LOG_LS(logger, file, line, func, spdlog::level::warn, __VA_ARGS__)
#else
#define LLWARN(...)
#define LSLWARN(file, line, function, ...)
#define LWARN(logger, ...)
#define LSWARN(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
/// @brief Log a error message using default values
#define LLERROR(...) __LOG(spdlog::level::err, __VA_ARGS__)
/// @brief Log a error message with specified source location
#define LSLERROR(file, line, function, ...) \
  __LOG_S(file, line, function, spdlog::level::err, __VA_ARGS__)
/// @brief Log a error message with specified logger
#define LERROR(logger, ...) __LOG_L(logger, spdlog::level::err, __VA_ARGS__)
/// @brief Log a error message with specified logger and source location
#define LSERROR(logger, file, line, func, ...) \
  __LOG_LS(logger, file, line, func, spdlog::level::err, __VA_ARGS__)
#else
#define LLERROR(...)
#define LSLERROR(file, line, function, ...)
#define LERROR(logger, ...)
#define LSERROR(logger, file, line, func, ...)
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
/// @brief Log a critical message using default values
#define LLCRITICAL(...) __LOG(spdlog::level::critical, __VA_ARGS__)
/// @brief Log a critical message with specified source location
#define LSLCRITICAL(file, line, function, ...) \
  __LOG_S(file, line, function, spdlog::level::critical, __VA_ARGS__)
/// @brief Log a critical message with specified logger
#define LCRITICAL(logger, ...) \
  __LOG_L(logger, spdlog::level::critical, __VA_ARGS__)
/// @brief Log a critical message with specified logger and source location
#define LSCRITICAL(logger, file, line, func, ...) \
  __LOG_LS(logger, file, line, func, spdlog::level::critical, __VA_ARGS__)
#else
#define LLCRITICAL(...)
#define LSLCRITICAL(file, line, function, ...)
#define LCRITICAL(logger, ...)
#define LSCRITICAL(logger, file, line, func, ...)
#endif

namespace specula::logging {
/** @brief Primary logging sink for all log messages
 *
 * All log messages are sent exclusivly to this primary sink. This sink is then
 * responsible to distributing the log messages to the other configured sinks
 * (commonly handled in the client code).
 */
extern std::shared_ptr<spdlog::sinks::dist_sink_mt> sink;

/** @brief Helper for fetching/constructing new loggers
 *
 * This is a wrapper function, used for fetching and returning new loggers. If a
 * logger with the name `name` already exists, a pointer to that existing logger
 * is returned, otherwise a new logger is created with that name, and is
 * returned.
 * @param name The name of the logger to fetch/create
 * @returns A pointer to a spdlog logger
 */
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
}  // namespace specula::logging

#endif
