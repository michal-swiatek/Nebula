//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_LOGGING_H
#define NEBULAENGINE_LOGGING_H

#include <thread>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <spdlog/pattern_formatter.h>

#include "Core.h"
#include "Types.h"

namespace nebula::logging {

    class ThreadFormatterFlag final : public spdlog::custom_flag_formatter
    {
    public:
        void format(const spdlog::details::log_msg& msg, const std::tm& tm_time, spdlog::memory_buf_t& dest) override;
        [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override;

        static void addThreadName(std::thread::id thread_id, const std::string& name);

    private:
        static std::unordered_map<std::thread::id, std::string> s_thread_names;

        static constexpr std::size_t s_max_thread_name_width = 15;
        static const std::string& getThreadName();
    };

    NEBULA_API void initCore();
    NEBULA_API void initClient(const std::string& name);
    NEBULA_API void shutdown();

    NEBULA_API Reference<spdlog::logger> getCoreLogger();
    NEBULA_API Reference<spdlog::logger> getClientLogger();

}

#ifdef NB_DEBUG_BUILD
    #define NB_CORE_TRACE(...)  ::nebula::logging::getCoreLogger()->trace(__VA_ARGS__)
    #define NB_CORE_INFO(...)   ::nebula::logging::getCoreLogger()->info(__VA_ARGS__)
    #define NB_CORE_WARN(...)   ::nebula::logging::getCoreLogger()->warn(__VA_ARGS__)
    #define NB_CORE_ERROR(...)  ::nebula::logging::getCoreLogger()->error(__VA_ARGS__)
    #define NB_CORE_FATAL(...)  ::nebula::logging::getCoreLogger()->critical(__VA_ARGS__)

    #define NB_TRACE(...)  ::nebula::logging::getClientLogger()->trace(__VA_ARGS__)
    #define NB_INFO(...)   ::nebula::logging::getClientLogger()->info(__VA_ARGS__)
    #define NB_WARN(...)   ::nebula::logging::getClientLogger()->warn(__VA_ARGS__)
    #define NB_ERROR(...)  ::nebula::logging::getClientLogger()->error(__VA_ARGS__)
    #define NB_FATAL(...)  ::nebula::logging::getClientLogger()->critical(__VA_ARGS__)
#else
    #define NB_CORE_TRACE(...)
    #define NB_CORE_INFO(...)
    #define NB_CORE_WARN(...)
    #define NB_CORE_ERROR(...)
    #define NB_CORE_FATAL(...)

    #define NB_TRACE(...)
    #define NB_INFO(...)
    #define NB_WARN(...)
    #define NB_ERROR(...)
    #define NB_FATAL(...)
#endif

#endif //NEBULAENGINE_LOGGING_H
