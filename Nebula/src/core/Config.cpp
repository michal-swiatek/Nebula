//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Config.h"

#include <sstream>
#include <fstream>

#include "memory/Types.h"

using namespace nebula::literals;

namespace nebula {

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
            m_config = defaultConfig();
    }

    void Config::save(const std::string& path)
    {
        std::ofstream file(path);
        file << m_config;
    }

    YAML::Node Config::defaultConfig()
    {
        YAML::Node node;

        auto memory_section = YAML::Node();
        memory_section["block_size"] = 100_Mb;

        node["memory"] = memory_section;

        return node;
    }

}
