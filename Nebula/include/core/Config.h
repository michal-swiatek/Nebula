//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef CONFIG_H
#define CONFIG_H

#include <yaml-cpp/yaml.h>

#include "Core.h"

int main(int argc, char** argv);

namespace nebula {

    class NEBULA_API Config
    {
    public:
        explicit Config(const std::string& path);

        void save(const std::string& path) const;
        bool isEmpty() const { return m_config.Type() == YAML::NodeType::Null; }

        YAML::Node& getConfig() { return m_config; }
        static const YAML::Node& getEngineConfig() { return s_instance->getConfig(); }

    private:
        YAML::Node m_config;

        static void setEngineConfig(Config& config);
        static YAML::Node defaultEngineConfig();

        static Config* s_instance;
        friend int ::main(int argc, char** argv);
    };

}

#endif //CONFIG_H
