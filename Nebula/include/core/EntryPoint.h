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

void initSubsystems();
void shutdownSubsystems();

#ifdef NB_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    nebula::Config engine_config("engine_config.yaml");
    nebula::Config::setEngineConfig(engine_config);

    initSubsystems();

    {
        auto app = nebula::Scope<nebula::Application>(nebula::createApplication(argc, argv));
        app->run();
    }

    shutdownSubsystems();

    engine_config.save("engine_config.yaml");

    return 0;
}

#endif

inline void initSubsystems()
{
    nebula::logging::initCore();
    nebula::memory::MemoryManager::init();
}

inline void shutdownSubsystems()
{
    nebula::logging::shutdown();
    nebula::memory::MemoryManager::shutdown();
}

#endif //NEBULAENGINE_ENTRYPOINT_H
