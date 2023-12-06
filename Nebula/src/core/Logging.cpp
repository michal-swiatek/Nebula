//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Logging.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace nebula::logging {

    Reference<spdlog::logger> core_logger;
    Reference<spdlog::logger> client_logger;
    std::unordered_map<std::thread::id, std::string> ThreadFormatterFlag::s_thread_names{};

    void ThreadFormatterFlag::addThreadName(const std::thread::id thread_id, const std::string& name)
    {
        s_thread_names[thread_id] = name;
    }

    const std::string& ThreadFormatterFlag::getThreadName()
    {
        const auto thread_id = std::this_thread::get_id();
        if (!s_thread_names.contains(thread_id))
            s_thread_names[thread_id] = std::format("{}", thread_id);

        return s_thread_names.at(thread_id);
    }

    void ThreadFormatterFlag::format(const spdlog::details::log_msg& msg, const std::tm& tm_time, spdlog::memory_buf_t& dest)
    {
        const std::string& thread_name = getThreadName();
        spdlog::details::scoped_padder padder(thread_name.size(), padinfo_, dest);
        dest.append(thread_name.data(), thread_name.data() + thread_name.size());
    }

    std::unique_ptr<spdlog::custom_flag_formatter> ThreadFormatterFlag::clone() const
    {
        return spdlog::details::make_unique<ThreadFormatterFlag>();
    }

    void initCore()
    {
        ThreadFormatterFlag::addThreadName(std::this_thread::get_id(), "MainThread");

        auto formatter = std::make_unique<spdlog::pattern_formatter>();
        formatter->add_flag<ThreadFormatterFlag>('t').set_pattern("%^[%T] |%=16!t| %8!n: %v%$");
        set_formatter(std::move(formatter));

        core_logger = spdlog::stdout_color_mt("NEBULA");
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
