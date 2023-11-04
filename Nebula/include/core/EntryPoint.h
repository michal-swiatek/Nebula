//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_ENTRYPOINT_H
#define NEBULAENGINE_ENTRYPOINT_H

#include <memory>

#include "Types.h"
#include "Logging.h"
#include "Application.h"

void initSubsystems();
void shutdownSubsystems();

#if defined(_WIN32) || defined(WIN32)

int main(int argc, char** argv)
{
    initSubsystems();

    auto app = nebula::Scope<nebula::Application>(nebula::createApplication(argc, argv));
    app->run();

    shutdownSubsystems();

    return 0;
}

#endif

void initSubsystems()
{
    nebula::logging::initCore();
}

void shutdownSubsystems()
{
    nebula::logging::shutdown();
}

#endif //NEBULAENGINE_ENTRYPOINT_H
