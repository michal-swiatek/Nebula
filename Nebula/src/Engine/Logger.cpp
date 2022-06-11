//
// Created by michal-swiatek on 11.06.2022.
// Github: https://github.com/michal-swiatek/
//

#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace nebula {

    std::shared_ptr<spdlog::logger> Logger::core_logger_;
    std::shared_ptr<spdlog::logger> Logger::application_logger_;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%H:%M:%S] %n: %v%$");

        core_logger_ = spdlog::stderr_color_mt("NEBULA");
        application_logger_ = spdlog::stderr_color_mt("APP");

        core_logger_->set_level(spdlog::level::trace);
        application_logger_->set_level(spdlog::level::trace);
    }

}
