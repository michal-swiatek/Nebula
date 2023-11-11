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

void initSubsystems();
void shutdownSubsystems();

#ifdef NB_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    initSubsystems();

    {
        auto app = nebula::Scope<nebula::Application>(nebula::createApplication(argc, argv));
        app->run();
    }

    shutdownSubsystems();

    return 0;
}

#endif

inline void initSubsystems()
{
    nebula::memory::MemoryManager::init();
    nebula::logging::initCore();
}

inline void shutdownSubsystems()
{
    nebula::logging::shutdown();
    nebula::memory::MemoryManager::shutdown();
}

#endif //NEBULAENGINE_ENTRYPOINT_H
