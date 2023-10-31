//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_ENTRYPOINT_H
#define NEBULAENGINE_ENTRYPOINT_H

#include <memory>
#include "Application.h"

#if defined(_WIN32) || defined(WIN32)

int main(int argc, char** argv)
{
    auto app = std::unique_ptr<nebula::Application>(nebula::createApplication());
    app->run();

    return 0;
}

#endif

#endif //NEBULAENGINE_ENTRYPOINT_H
