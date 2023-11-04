//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Logging.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace nebula::logging {

    Reference<spdlog::logger> core_logger;
    Reference<spdlog::logger> client_logger;

    void initCore()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        core_logger = spdlog::stdout_color_mt("CORE");
        core_logger->set_level(spdlog::level::trace);
    }

    void initClient(const std::string& name)
    {
        client_logger = spdlog::stdout_color_mt(name);
        client_logger->set_level(spdlog::level::trace);
    }

    void shutdown() {}

    Reference<spdlog::logger> getCoreLogger()
    {
        return core_logger;
    }

    Reference<spdlog::logger> getClientLogger()
    {
        return client_logger;
    }

}
