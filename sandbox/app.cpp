//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#include "Nebula.h"


class App : public nebula::Application
{
public:
    App() : nebula::Application()
    {
#ifndef SPDLOG_HEADER_ONLY
        APP_LOGGER_INFO("Creating {} application", "sandbox");
#endif
    }

    ~App() override
    {
        APP_LOGGER_TRACE("Exiting {} application", "sandbox");
    }
};

nebula::Application *nebula::CreateApplication()
{
    return new App();
}
