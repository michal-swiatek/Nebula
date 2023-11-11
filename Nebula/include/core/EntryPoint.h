//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_ENTRYPOINT_H
#define NEBULAENGINE_ENTRYPOINT_H

#include "Types.h"
#include "Logging.h"
#include "Application.h"
#include "memory/MemoryManager.h"
#include "Config.h"

void initSubsystems(nebula::Config& config);
void shutdownSubsystems();

#ifdef NB_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    nebula::Config engine_config;

    initSubsystems(engine_config);

    {
        auto app = nebula::Scope<nebula::Application>(nebula::createApplication(argc, argv));
        app->run();
    }

    shutdownSubsystems();

    engine_config.save();

    return 0;
}

#endif

inline void initSubsystems(nebula::Config& config)
{
    auto engine_config = config.getConfig();

    auto memory_block_size = engine_config["memory"]["block_size"].as<int>();

    nebula::logging::initCore();
    nebula::memory::MemoryManager::init(memory_block_size);
}

inline void shutdownSubsystems()
{
    nebula::logging::shutdown();
    nebula::memory::MemoryManager::shutdown();
}

#endif //NEBULAENGINE_ENTRYPOINT_H
