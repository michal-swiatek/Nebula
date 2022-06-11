//
// Created by michal-swiatek on 11.06.2022.
// Github: https://github.com/michal-swiatek/
//

#ifndef NEBULA_LOGGER_H
#define NEBULA_LOGGER_H

#include <memory>

#include "spdlog/fwd.h"
#include "spdlog/spdlog.h"

#include "Engine/NebulaAPI.h"

namespace nebula {

    class NEBULA_API Logger
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return core_logger_; };
        inline static std::shared_ptr<spdlog::logger> GetApplicationLogger() { return application_logger_; };

    private:
        static std::shared_ptr<spdlog::logger> core_logger_;
        static std::shared_ptr<spdlog::logger> application_logger_;
    };

}

//  TODO: Remove in final build
#if !defined(NDEBUG) || defined(RELEASE_LOGS)
    //  Core logs
    #define CORE_LOGGER_TRACE(...)     ::nebula::Logger::GetCoreLogger()->trace(__VA_ARGS__)
    #define CORE_LOGGER_DEBUG(...)     ::nebula::Logger::GetCoreLogger()->debug(__VA_ARGS__)
    #define CORE_LOGGER_INFO(...)      ::nebula::Logger::GetCoreLogger()->info(__VA_ARGS__)
    #define CORE_LOGGER_WARN(...)      ::nebula::Logger::GetCoreLogger()->warn(__VA_ARGS__)
    #define CORE_LOGGER_ERROR(...)     ::nebula::Logger::GetCoreLogger()->error(__VA_ARGS__)
    #define CORE_LOGGER_CRITICAL(...)  ::nebula::Logger::GetCoreLogger()->critical(__VA_ARGS__)

    //  Application logs
    #define APP_LOGGER_TRACE(...)      ::nebula::Logger::GetApplicationLogger()->trace(__VA_ARGS__)
    #define APP_LOGGER_DEBUG(...)      ::nebula::Logger::GetApplicationLogger()->debug(__VA_ARGS__)
    #define APP_LOGGER_INFO(...)       ::nebula::Logger::GetApplicationLogger()->info(__VA_ARGS__)
    #define APP_LOGGER_WARN(...)       ::nebula::Logger::GetApplicationLogger()->warn(__VA_ARGS__)
    #define APP_LOGGER_ERROR(...)      ::nebula::Logger::GetApplicationLogger()->error(__VA_ARGS__)
    #define APP_LOGGER_CRITICAL(...)   ::nebula::Logger::GetApplicationLogger()->critical(__VA_ARGS__)
#else
    //  Core logs
    #define CORE_LOGGER_TRACE(...)     (void)0
    #define CORE_LOGGER_DEBUG(...)     (void)0
    #define CORE_LOGGER_INFO(...)      (void)0
    #define CORE_LOGGER_WARN(...)      (void)0
    #define CORE_LOGGER_ERROR(...)     (void)0
    #define CORE_LOGGER_CRITICAL(...)  (void)0

    //  Application logs
    #define APP_LOGGER_TRACE(...)      (void)0
    #define APP_LOGGER_DEBUG(...)      (void)0
    #define APP_LOGGER_INFO(...)       (void)0
    #define APP_LOGGER_WARN(...)       (void)0
    #define APP_LOGGER_ERROR(...)      (void)0
    #define APP_LOGGER_CRITICAL(...)   (void)0
#endif

#endif //NEBULA_LOGGER_H
