#include "logging.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/dist_sink.h>

#include <memory>

namespace specula::logging {
std::shared_ptr<spdlog::sinks::dist_sink_mt> sink = nullptr;
}
