#include "logging.hpp"

#include <memory>
#include <spdlog/common.h>
#include <spdlog/sinks/dist_sink.h>

namespace specula::logging {
std::shared_ptr<spdlog::sinks::dist_sink_mt> sink = nullptr;
}
