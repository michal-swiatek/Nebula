//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Config.h"

#include <sstream>
#include <fstream>

#include "memory/Types.h"
#include "core/Assert.h"

using namespace nebula::literals;

namespace nebula {

    Config* Config::s_instance = nullptr;

    void Config::setEngineConfig(Config& config)
    {
        NB_CORE_ASSERT(!s_instance, "Can't create two instances of engine config!");
        if (config.isEmpty())
            config.getConfig() = defaultEngineConfig();
        s_instance = &config;
    }

    Config::Config(const std::string& path)
    {
        std::ifstream file(path);
        if (file.good())
        {
            std::stringstream sstr;
            sstr << file.rdbuf();
            m_config = YAML::Load(sstr.str());
        }
        else
            m_config = YAML::Node();
    }

    void Config::save(const std::string& path) const
    {
        std::ofstream file(path);
        file << m_config;
    }

    YAML::Node Config::defaultEngineConfig()
    {
        YAML::Node node;

        auto memory_section = YAML::Node();
        memory_section["event_queue_size"] = 1_Mb;
        memory_section["render_command_buffer_size"] = 100_Kb;

        node["memory"] = memory_section;

        return node;
    }

}
