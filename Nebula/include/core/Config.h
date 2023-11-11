//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef CONFIG_H
#define CONFIG_H

#include <yaml-cpp/yaml.h>

#include "Core.h"

namespace nebula {

    class NEBULA_API Config
    {
    public:
        explicit Config(const std::string& path = "engine_config.yaml");

        void save(const std::string& path = "engine_config.yaml");
        YAML::Node& getConfig() { return m_config; }

        static YAML::Node defaultConfig();

    private:
        YAML::Node m_config;
    };

}

#endif //CONFIG_H
